# 0x00. 导读

```
kernel-devel 
kernel 
kernel-header
```

注意，

# 0x01. 简介

`/lib/modules/uname -r/build`: 默认内核源码路径，一般是链接到 `/usr/src/kernels/`  

`/usr/src/kernels/`: kernel-devel 安装位置

```查看本地包
$ rpm -qa | grep kernel-headers-$(uname -r)
$ rpm -qa | grep kernel-devel-$(uname -r)
```

## 1.1 kernel tainted

Out-of-Tree Module 即非 in-tree module。

insmod out-of-tree module，会导致 kernel tainted（污染）。tainted kernel 只是一种 kernel 的状态，在此状态下的 bug 可能得不到社区的支持。module 使用 kernel 不支持的 license，也会导致 kernel 进入 tainted 状态。

```
# 查看当前系统是否处于 tainted 状态，0 表示 not tainted
$ cat /proc/sys/kernel/tainted
```

# 0x02.

内核模块开发需要 内核头文件 和 Makefile 等特定的依赖。 Centos 下为了解决依赖需要安装 kernel-devel 包，需要与内核版本一致，比如内核版本 3.10.0-862.el7.x86_64 ，那么需要安装 kernel-devel-3.10.0-862.el7.x86_64 。

kernel-devel 安装 /usr/src/kernels/ 目录下。但系统在 /lib/modules/uname -r/source 和 /lib/modules/uname -r/build 两个位置有软链到合适版本的 kernel-devel 安装位置。因此需要指定路径时，我会使用/lib/modules/uname -r/build 。

# 0x03. 

kernel-devel 包只包含用于内核开发环境所需的内核头文件以及 Makefile，而 kernel-souce 包含所有内核源代码。

如果仅仅是用于你自己编写的模块开发的话，因为只需引用相应的内核头文件，所以只有 kernel-devel 包即可，如果你要修改现有的内核源代码并重新编译，那必须是 kernel-souce。

关于 kernel source 的有 kernel 和 kernel-devel 两个 rpm ，其中 kernel rpm 包含源文件和头文件，而 kernel-devel 则主要是头文件。