# 0x00. 导读

# 0x01. 简介

# 0x02. 设备

设备节点被创建在 `/dev` 下，是连接内核与用户层的枢纽，在 Linux 中，所有设备都以文件的形式存放在 `/dev` 目录下，都是通过文件的方式进行访问，设备节点是Linux内核对设备的抽象，一个设备节点就是一个文件。应用程序通过一组标准化的调用执行访问设备，这些调用独立于任何特定的驱动程序。而驱动程序负责将这些标准调用映射到实际硬件的特有操作。

设备节点，驱动，硬件设备是如何关联到一起的呢？

这是通过设备号实现的，包括主设备号和次设备号。当我们创建一个设备节点时需要指定主设备号和次设备号。应用程序通过名称访问设备，而设备号指定了对应的驱动程序和对应的设备。主设备号标识设备对应的驱动程序，次设备号由内核使用，用于确定设备节点所指设备。

**主设备号告诉你哪些驱动模块是用来操纵哪些硬件设备的。从设备号是驱动模块自己使用来区别它操纵的不同设备，当此驱动模块操纵不只一个设备时。**

设备文件（设备节点），`/dev` 目录下是字符设备文件和块设备文件。网络设备注册在 `/sys/class/net` 目录下。

[Linux 内核中，使用 struct cdev 来描述一个字符设备](https://www.cnblogs.com/chen-farsight/p/6155518.html#unit3.1)

## 2.1 创建字符驱动

linux 中一切皆文件，设备也如此，并且以操作文件即 文件IO 的方式访问设备。

应用程序只能通过库函数中的系统调用来操作硬件，对于每个系统调用，驱动程序中都会有一个与之对应的函数，对于字符设备驱动，这些函数集中在 `file_operations` 结构体中。当应用程序使用系统调用 read、write 等函数访问设备时，最终会调用到 `file_operations` 中的成员，当然一开始 fops 中的只是一大堆函数指针的调用接口，具体的函数就需要我们在驱动中实现，实现对应操作函数后，与 fops 一对接，应用程序最终就能对硬件进行控制了。

那么问题来了，当应用程序使用系统调用访问设备时，linux 系统怎么知道调用哪一个驱动的 fops 中的成员呢？答案是 设备号和驱动相关联。

```c
// 1、注册获取设备号
// 2、初始化设备
// 3、操作设备 file_operations – open release read write ioctl…
// 4、两个宏定义 module_init module_exit 
// 5、注册设备号 register_chrdev_region
// 6、cdev_init 初始化字符设备
// 7、cdev_add 添加字符设备到系统

// or

// 申请一个字符设备。
// 实现 file_operations 结构体中需要使用的函数，并把函数指针赋值给其相应的成员变量。
// 将字符设备与 file_operations 结构体进行绑定。
// 申请设备编号并注册到系统。
// 在 /dev 目录下创建设备。
// 编写应用层程序对该驱动设备进行读写操作。

static int __init drv_init(void)
{
    PRINT("[KERNEL]:%s ------ \n", __FUNCTION__);
    
    s_cdev = cdev_alloc();                                                     /* 申请一个字符设备 */
    cdev_init(s_cdev, &hello_fops);                                            /* 初始化字符设备,与file_operations绑定 */
    alloc_chrdev_region(&s_dev, 0, 1, "hello");                                /* 动态申请一个设备编号 */
    cdev_add(s_cdev, s_dev, 1);                                                /* 添加一个字符设备到系统 */
    
    hello_class = class_create(THIS_MODULE, "hello");                          /* 将本模块创建一个类,并注册到内核 */
    hello_device = device_create(hello_class, NULL, s_dev, NULL, "hello");     /* 创建设备并注册到内核 */
    
    return 0;
}
```

```c
// name 可以在文件 /proc/devices 中看到
int register_chrdev(unsigned int major, const char * name, const struct file_operations * fops)

//参数：
//1、major：主设备号，注意注册并不需要提供从设备号。内核本身并不在意从设备号。
//     设备号(32bit–dev_t)==主设备号(高12bit) + 次设备号(低20bit)
//      主设备号：表示一类设备—（如：camera）
//      次设备号： 表示一类设备中某一个—（如：前置camera/后置camera）
//       0 表示动态分配  ； 给定整数，表示静态指定
//2、name: 描述设备信息，可自定义
//        在目录 /proc/devices 列举出了所有的已经注册的设备
//3、fops: 文件操作对象
//         提供 open， read，write
//返回值：成功-0，失败-负数

// ---------------------------------------------------------
// 在 /sys/class 目录下创建一个本驱动的类的目录，并返回 class 对象。
struct class *class_create(owner, name)//创建一个类

//参数：
//1、owner：THIS_MODULE
//2、name ：字符串名字，自定义
//返回：
//        返回一个class指针

// ---------------------------------------------------------
// 根据 class_create 生成的 class 指针以及主设备号、驱动名在 /dev 目录下创建驱动设备。 
struct device *device_create(struct class * class, struct device * parent, dev_t devt, void * drvdata, const char * fmt,...)

//参数：
//1、class结构体，class_create调用之后的返回值
//2、表示父亲，一般直接填NULL
//3、设备号类型 dev_t
//4、私有数据，一般直接填NULL
//5/6、表示可变参数，字符串，表示设备节点名字，可以在 /dev/ 下面看到的

设备号类型：dev_t devt
        #define MAJOR(dev)    ((unsigned int) ((dev) >> MINORBITS))     //获取主设备号
        #define MINOR(dev)    ((unsigned int) ((dev) & MINORMASK))     //获取次设备号
        #define MKDEV(ma,mi)  (((ma) << MINORBITS) | (mi))      //生成设备号

// ---------------------------------------------------------
// 实现文件IO接口--fops
const struct file_operations my_fops = {
    .open = chr_drv_open,
    .read = chr_drv_read,
    .write = chr_drv_write,
    .release = chr_drv_close,
};
```

应用程序使用
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
int main(int argc, char * argv [ ])
{
    //调用驱动
    int fd;
    int value = 0;  //buf

    fd = open("/dev/chr2", O_RDWR);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

    // 就会用到 file_operations 里面的函数指针了
    read(fd, &value, 4);
    write(fd, &value, 4);
    close(fd);
    
    return 0;
}

chr_test.c
```

## 2.2 更进一步，应用程序控制驱动

应用程序要控制驱动，就涉及用户空间与内核空间的数据交互，如何实现？通过以下函数：
```c
// 将数据从内核空间拷贝到用户空间,一般是在驱动中 chr_drv_read() 用
int copy_to_user(void __user * to, const void * from, unsigned long n)
//参数：
//1：应用驱动中的一个 buffer
//2：内核空间到一个 buffer
//3：个数
//返回值：大于0，表示出错，剩下多少个没有拷贝成功等于0，表示正确


// 将数据从用户空间拷贝到内核空间,一般是在驱动中 chr_drv_write() 用
int copy_from_user(void * to, const void __user * from, unsigned long n)
//参数：
//1：内核驱动中的一个buffer
//2：应用空间到一个buffer
//3：个数
```