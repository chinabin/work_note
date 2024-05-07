# 0x00. 导读

# 0x01. 简介

# 0x02. string 实现

string 中比较重要的 3 个字段：
- char *data. 指向存放字符串的首地址（在 SSO 的某些实现方案中可能没有此字段）。
- size_t size. 字符串长度。
- size_t capacity. 字符串容量。capacity >= size. 在字符串相加、reserve 等场景下会用到此字段。

## 2.1 eager copy

这个是最简单、最好理解的一种，在每次拷贝时将原 string 对应的内存以及所持有的动态资源完整地复制一份，即没有任何特殊处理。

优点：
- 实现简单。
- 每个对象互相独立，不用考虑那么多乱七八糟的场景。

缺点：
- 字符串较大时，拷贝浪费空间。

## 2.2 COW

这个也算是计算机里的基本思想了。不同于 eager copy 的每次拷贝都会复制，此种实现方式为写时复制，即 copy-on-write。只有在某个 string 要对共享对象进行修改时，才会真正执行拷贝。

由于存在共享机制，所以需要一个 `std::atomic<size_t>` ，代表被多少对象共享。

优点：
- 字符串空间较大时，减少了分配、复制字符串的时间。

缺点：
- refcount 需要原子操作，性能有损耗。
- 某些情况下会带来意外的开销。比如非 const 成员使用[]，这会触发 COW，因为无法知晓应用程序是否会对返回的字符做修改。

```c++
std::string s("str");
const char* p = s.data();
{
    std::string s2(s);
    (void) s[0];         // 触发COW
}
std::cout << *p << '\n';      // p指向的原有空间已经无效
```

## 2.3 SSO

Small String Optimization. 基于字符串大多数比较短的特点，利用 string 对象本身的栈空间来存储短字符串。而当字符串长度大于某个临界值时，则使用 eager copy 的方式。

SSO 下，string 的数据结构会稍微复杂点，使用 union 来区分短字符串和长字符串的场景：
```c++
class string {
  char *start;
  size_t size;
  static const int kLocalSize = 15;
  union{
    char buffer[kLocalSize+1];      // 满足条件时，用来存放短字符串
    size_t capacity;
  }data;
};
```

优点：
- 短字符串时，无动态内存分配。

缺点：
- string 对象占用空间比 eager copy 和 cow 要大。