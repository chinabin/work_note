# 0x00. 导读

# 0x01. 简介

# 0x02. 

__builtin_expect(EXP, N)。意思是：EXP == N 的概率很大。那么上面 likely 和 unlikely 这两句的具体含义就是：

- __builtin_expect(!!(x),1) x 为真的可能性更大
- __builtin_expect(!!(x),0) x 为假的可能性更大

```c
// 条件被认为是非常非常有可能满足时
#define likely(x) __builtin_expect(!!(x), 1)

// 条件非常非常不可能或很难满足时
#define unlikely(x) __builtin_expect(!!(x), 0)
```

## 2.1 处理 Unpredictable Branch

```c
int a, b; // a and b are random
// ...
float c = a > b ? 1.1f : 2.0f;
```

改为

```c
float c_lut[2] = {1.1f , 2.0f};
float c = c_lut[a > b];
```