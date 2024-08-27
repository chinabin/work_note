# 0x00. 导读

# 0x01. 简介

ioctl 函数通过对文件描述符发送特定的命令来控制文件描述符所代表的设备。

# 0x02. 详解

```c
// 返回值：若出错则返回 -1, 若成功则返回其他值
int ioctl(int fd, int cmd, …)； 
// fd 是用户使用 open 函数返回的文件标示符
// cmd 是用户程序对设备的控制命令，至于后面的省略号，那是一些补充参数（通常只有一个参数，它常常是指向一个变量或结构的指针。）

// ioctl 函数是文件结构中的一个属性分量，就是说如果你的驱动程序提供了对ioctl的支持，用户就可以在用户程序中使用ioctl函数来控制设备的I/O通道。
```

上面 cmd 是应用层与驱动之间的指令“协议”。 为了保证该指令“协议”的唯一性，在Linux中提供了一种统一格式，将32位的 int 型数据划分为四个部分： 

- dir（2 bit，第 31-30 位）： 表示 ioctl 命令对设备的操作类型，2个bit有4中可能，分别为 访问模式（数据传输方向），可以为无、读、写、读写，相应的宏分别为 IOC_NONE、 IOC_READ、IOC_WRITE、 IOC_READ | _IOC_WRITE，定义在kernel\include\uapi\asm-generic\ioctl.h中。

- size（13/14bit，第 29-16位）： ，ARM 架构一般为 14 bit，表示每次读写数据的长度，即 ioctl 函数第三个参数 arg 数据的长度。

- type（8 bit，第 15-8 位）： 表示设备类型，可以为任意 char 型字符，其主要作用是使 ioctl 命令有唯一的设备标识，通常用英文字符 "A" ~ "Z" 或者 "a" ~ "z" 来表示。可以参考kernel/Documentation/ioctl/ioctl-number.txt文件，选取一个还没有别定义的字符进行设置。一些文献译为 “幻数” 或者 “魔数”。

- nr（8bit，第 7-0位）： 命令编号，可以为任意 unsigned char 型数据，取值范围 0~255。一个命令对应一个编号，通常从 0 开始编号递增。

```c
// kernel\include\uapi\asm-generic\ioctl.h
​
#define _IOC(dir,type,nr,size) \
    (((dir)  << _IOC_DIRSHIFT) | \
     ((type) << _IOC_TYPESHIFT) | \
     ((nr)   << _IOC_NRSHIFT) | \
     ((size) << _IOC_SIZESHIFT))
​
#ifndef __KERNEL__
#define _IOC_TYPECHECK(t) (sizeof(t))
#endif
​
/* used to create numbers */
#define _IO(type,nr)        _IOC(_IOC_NONE,(type),(nr),0)
#define _IOR(type,nr,size)  _IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOW(type,nr,size)  _IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOWR(type,nr,size) _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
```

这四个宏的使用格式分别为：

- _IO(魔数， 编号) ，作用在 无数据读写 的命令。
- _IOR(魔数， 编号， 变量类型) ，作用在 读数据 的命令。
- _IOW(魔数， 编号， 变量类型) ，作用在 写数据 的命令。
- _IOWR(魔数， 编号，变量类型) ，作用在 读写数据 的命令。

相对应的 Linux 内核也有四个宏用来解析命令
```c
/* used to decode ioctl numbers.. */
#define _IOC_DIR(nr)        (((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
#define _IOC_TYPE(nr)       (((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
#define _IOC_NR(nr)     (((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
#define _IOC_SIZE(nr)       (((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)
```

这四个宏的使用格式分别为：

- _IOC_DIR(命令) ，解析出数据方向。
- _IOC_TYPE(命令) ，解析出设备类型。
- _IOC_NR(命令) ，解析出命令编号。
- _IOC_SIZE(命令) ，解析出数据长度。

## 2.1 例子详解

[ioctl_example.md](./ioctl_example.md)
