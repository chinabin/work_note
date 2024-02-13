# 0x00. 导读

[ldconfig.md](../util/ldconfig.md)

# 0x01. 简介

**LD_PRELOAD**是个环境变量，用于动态库的加载，动态库加载的优先级最高。

在 UNIX 的动态链接库的世界中，LD_PRELOAD 可以影响程序的运行时的链接(Runtime linker)它允许你定义在程序运行前优先加载的动态链接库。这个功能主要就是用来有选择性的载入不同动态链接库中的相同函数。

通过这个环境变量，我们可以在主程序和其动态链接库的中间加载别的动态链接库，甚至覆盖正常的函数库。一方面，我们可以以此功能来使用自己的或是更好的函数(无需别人的源码)，而另一方面，我们也可以以向别人的程序注入恶意程序，从而达到那不可告人的罪恶的目的。

我们知道 Linux 中命令基本都用到了 glibc 库，生成了有一个叫 libc.so.6 的文件，这是几乎所有 Linux 下命令的动态链接中，其中有标准 C 的各种函数。对于 GCC 而言，默认情况下，所编译的程序中对标准 C 函数的链接，都是通过动态链接方式来链接 libc.so.6 这个函数库的。

# 0x02. 

动态库加载顺序

1. LD_PRELOAD
2. LD_LIBRARY_PATH
3. /etc/ld.so.cache
4. /lib
5. /usr/lib

# 0x03. 例子

```c
##############
# random_num.c
##############

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
  srand(time(NULL));
  int i = 10;
  while(i--) printf("%d\n",rand()%100);
  return 0;
}
```

```bash
$ gcc random_num.c -o random_num
$ ./random_nums
56
23
12
...
```

```c
##############
# unrandom.c
##############

int rand(){
    return 42; //the most random number in the universe
}
```

```bash
$ gcc -shared -fPIC unrandom.c -o unrandom.so
$ LD_PRELOAD=$PWD/unrandom.so ./random_nums
42
42
42
...

# 程序random_nums所需要的库的列表
# 这个列表是构建进可执行程序中的并且它是在编译时决定的
$ ldd random_nums
linux-vdso.so.1 => (0x00007fff4bdfe000)
libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f48c03ec000)
/lib64/ld-linux-x86-64.so.2 (0x00007f48c07e3000)

# 可以使用如下命令查看so文件的全部函数列表
$ nm -D /lib/libc.so.6 | grep "rand()"

# 列出我们强加指定库之后的链接库列表
$ LD_PRELOAD=$PWD/unrandom.so ldd random_nums
linux-vdso.so.1 =>  (0x00007fff369dc000)
/some/path/to/unrandom.so (0x00007f262b439000)
libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f262b044000)
/lib64/ld-linux-x86-64.so.2 (0x00007f262b63d000)
```