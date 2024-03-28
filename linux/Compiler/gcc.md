# 0x00. 导读

# 0x01. 简介

# 0x02. 

```bash
-O3 (enables machine-independent optimizations)  
-march (enables optimizations for particular CPU generation)  

-flto (enables inter-procedural optimizations)
# LTO=Link-Time Optimization
# 由于编译器一次只编译优化一个编译单元，所以只是在做局部优化，而利用 LTO，利用链接时的全局视角进行操作，从而得到能够进行更加极致的优化。

-shared 
# 该选项指定生成动态连接库（让连接器生成T类型的导出符号表，有时候也生成弱连接W类型的导出符号），不用该标志外部程序无法连接。相当于一个可执行文件。

-fPIC
# 表示编译为位置独立的代码，不用此选项的话编译后的代码是位置相关的所以动态载入时是通过代码拷贝的方式来满足不同进程的需要，而不能达到真正代码段共享的目的。

-fpic
# Generate position-independent code if possible (small mode)
# 关于 Large mode 和 Small mode, 简单理解为 large 对地址空间没有限制，small 限制地址空间在 2GB 以内


-fPIE
# Generate position-independent code for executables if possible (large mode)

-fpie
# Generate position-independent code for executables if possible (small mode)

-pie
# Create a position independent executable。

-L
# 表示要连接的库在当前目录中。

-I
# 指定头文件include查找目录。

-l
# 指定要使用的库名称，在静态库和动态库同名条件下，优先使用动态库。

-soname
# 指定soname。

-Wl
# 在GCC编译程序时，由于GCC命令不仅能够编译，也能够链接程序，GCC链接程序是通过ld命令实现的，如何将GCC的命令行参数传递给ld命令呢，这就是通过-Wl,来实现的。
# 格式如下：
# gcc -Wl,param1,param2,param3,...
# 在调用ld命令时，会等效于如下：
# ld param1 param2 param3 
# 注意：-Wl, 后的逗号（，）必不可少，如果要传递多个参数，参数间用，分隔，不能有空格
# 例如：-Wl,-Bstatic -la -lb -lc -Wl,-Bdynamic -ld -l e
# 会被解释成 ：ld liba.a libb.a libc.a libd.so libe.so
# -Wl,-rpath -Wl,/data/workroom/libs/lib
# 会被解释成：ld -rpath /data/workroom/libs/lib

-Wl,-Bstatic
# 指定后续ld在处理 -l 参数来链接库文件的链接方式首选静态库

-Wl,-Bdynamic
# 指定后续ld在处理 -l 参数来链接库文件的链接方式首选动态库
```

```bash
# 会列出可选项
g++ -march=native -m32 ... -Q --help=target 

# 显示 O3 用到了哪些优化
gcc -Q -O3 --help=optimizers` 
```

```bash
# 打印系统的 include 默认搜索路径
gcc -print-prog-name=cc1 -v

# 打印系统的 库 默认搜索路径
gcc -print-search-dirs

```

SONAME: 是一组具有兼容 API 的 SO 库所共有的名字，其命名特征是 lib+<库名>+.so.+<数字> 这种形式的，例如 `linux-vdso.so.1` `libtinfo.so.5`

[ld.so linux 动态链接器](https://listenerri.com/2023/10/08/ld-so-linux-%E5%8A%A8%E6%80%81%E8%BF%9E%E6%8E%A5%E5%99%A8%E6%98%AF%E4%BB%80%E4%B9%88/)  
[dynamic-link](https://markrepo.github.io/kernel/2018/08/19/dynamic-link/)

很多现代应用都是通过动态编译链接的，当一个 需要动态链接 的应用被操作系统加载时，系统必须要 定位 然后 加载它所需要的所有动态库文件。 在Linux环境下，这项工作是由ld-linux.so.2来负责完成的

ld.so,ld-linux.so* ：动态链接器/加载器  
可以直接调用，例如 `/lib/ld-linux.so.*  [OPTIONS] [PROGRAM [ARGUMENTS]]`  
ld.so和ld-linux.so*查找并且装载其他程序所依赖的动态链接对象，当装载完毕之后，就开始运行程序

当在 linux 系统中启动一个可执行文件时，首先发挥作用的是程序加载器 (program loader) ，这个加载器也是一个 so 文件，通常具有 ld-linux.so.X 这样的文件名，其中的 X 是版本号。例如 /lib64/ld-linux-x86-64.so.2 。 linux 下的 elf 格式的可执行文件在运行时，首先加载 ld-linux.so ，再由这个加载器去加载其他的 so 文件，当其他 so 文件都已经加载完成之后，我们自己编写的 main 函数才会被执行。


---

rpath: 链接器找不到这个东西，因为它不在系统的搜索路径里。一个办法是使用 LD_LIBRARY_PATH : `$ LD_LIBRARY_PATH=. ldd ./main`  
好一点的做法是在编译链接时把搜索路径写入目标文件里，运行时链接器就知道到哪去找了，这就是 rpath 