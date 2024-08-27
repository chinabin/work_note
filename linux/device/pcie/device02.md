# 0x00. 导读

file_operations 详解

# 0x01. 简介

```c
struct file_operations {
 struct module *owner;
 loff_t (*llseek) (struct file *, loff_t, int);
 ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
 ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
 ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
 ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
 int (*iterate) (struct file *, struct dir_context *);
 int (*iterate_shared) (struct file *, struct dir_context *);
 unsigned int (*poll) (struct file *, struct poll_table_struct *);
 long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
 long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
 int (*mmap) (struct file *, struct vm_area_struct *);
 int (*open) (struct inode *, struct file *);
 int (*flush) (struct file *, fl_owner_t id);
 int (*release) (struct inode *, struct file *);
 int (*fsync) (struct file *, loff_t, loff_t, int datasync);
 int (*fasync) (int, struct file *, int);
 int (*lock) (struct file *, int, struct file_lock *);
 ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
 unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
 int (*check_flags)(int);
 int (*flock) (struct file *, int, struct file_lock *);
 ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
 ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
 int (*setlease)(struct file *, long, struct file_lock **, void **);
 long (*fallocate)(struct file *file, int mode, loff_t offset,loff_t len);
 void (*show_fdinfo)(struct seq_file *m, struct file *f);
#ifndef CONFIG_MMU
 unsigned (*mmap_capabilities)(struct file *);
#endif
 ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,loff_t, size_t, unsigned int);
 int (*clone_file_range)(struct file *, loff_t, struct file *, loff_t,u64);
 ssize_t (*dedupe_file_range)(struct file *, u64, u64, struct file *,u64);
} __randomize_layout;
```

结构体 file_operations 在头文件 linux/fs.h 中定义，用来存储驱动内核模块提供的对设备进行各种操作的函数的指针。该结构体的每个域都对应着驱动内核模块用来处理某个被请求的事务的函数的地址。

驱动内核模块是不需要实现每个函数的。像视频卡的驱动就不需要从目录的结构中读取数据。那么，相对应的 file_operations 项就为 NULL。

```c
struct file_operations fops = {
 .read = device_read,
 .write = device_write,
 .open = device_open,
 .release = device_release
};
```

这种语法很清晰，你也必须清楚的意识到没有显示声明的结构体成员都被 gcc 初始化为 NULL。

指向结构体 struct file_operations 的指针通常命名为 fops。

# 0x02. 关于 file 结构体

每一个设备文件都代表着内核中的一个 file 结构体。该结构体在头文件 linux/fs.h 定义。注意，file 结构体是内核空间的结构体，这意味着它不会在用户程序的代码中出现。它绝对不是在 glibc 中定义的 FILE. FILE 自己也从不在内核空间的函数中出现。它的名字确实挺让人迷惑的。它代表着一个抽象的打开的文件，但不是那种在磁盘上用结构体 inode 表示的文件。

**设备驱动模块并不自己直接填充结构体 file: 它们只是使用在别处建立的结构体 file 中的数据。**

**实际上，大多数 Linux 驱动遵循一个“潜规则”：将文件的私有数据 private_data 指向设备结构体，再用 read()、write()、ioctl()、llseek() 等函数通过 private_data 访问设备结构体。私有数据的概念在 Linux 驱动的各个子系统中广泛存在，实际上体现了 Linux 的面向对象的设计思想。**

内核使用 inode 结构体在内核内部表示一个文件。因此，它与表示一个已经打开的文件描述符的结构体(即 file 文件结构)是不同的，我们可以使用多个 file 文件结构表示同一个文件的多个文件描述符，但此时，所有的这些 file 文件结构全部都必须只能指向一个 inode 结构体。

inode 结构体包含了一大堆文件相关的信息，但是就针对驱动代码来说，我们只要关心其中的两个域即可：

- dev_t i_rdev;     表示设备文件的结点，这个域实际上包含了设备号。
- struct cdev *i_cdev;　　struct cdev 是内核的一个内部结构，它是用来表示字符设备的，当 inode 结点指向一个字符设备文件时，此域为一个指向 inode 结构的指针。

从 inode 中取出 major 和 minor，分别使用 `imajor` 和 `iminor`，为了保持兼容，别自己去解析。他们俩最终会分别调用 MAJOR 和 MINOR.

# 0x03. 函数

```c
用来从设备中读取数据

ssize_t (*read) (struct file *filp, char __user *buffer, size_t size, loff_t *p);

指针参数 filp 为进行读取信息的目标文件，指针参数 buffer 为对应放置信息的缓冲区（即用户空间内存地址），参数 size 为要读取的信息长度，参数 p 为读的位置相对于文件开头的偏移，在读取信息后，这个指针一般都会移动，移动的值为要读取信息的长度值。

这个函数用来从设备中获取数据。在这个位置的一个空指针导致 read 系统调用以 -EINVAL("Invalid argument") 失败。一个非负返回值代表了成功读取的字节数(返回值是一个 "signed size" 类型, 常常是目标平台本地的整数类型)。
```

```c
向设备发送数据

ssize_t (*write) (struct file *filp, const char __user *buffer, size_t count, loff_t *ppos);

参数 filp 为目标文件结构体指针，buffer 为要写入文件的信息缓冲区，count 为要写入信息的长度，ppos 为当前的偏移位置，这个值通常是用来判断写文件是否越界。函数的返回值代表成功写的字节数。
```

```c
提供设备相关控制命令的实现，当调用成功时，返回给调用程序一个非负值。它与用户空间应用程序调用的 fcntl() 和 ioctl() 对应。

long (*unlocked_ioctl) (struct file *filp, unsigned int cmd, unsigned long aeg);
```

```c
将设备内存映射到进程的虚拟地址空间中，如果设备驱动未实现此函数，用户进行 mmap() 函数调用时将获得 -ENODEV 返回值。这个函数对于帧缓冲等设备特别有意义，帧缓冲被映射到用户空间后，应用程序可以直接访问它而无须在内核和应用间进行内存复制。它与用户空间应用程序中的 mmap() 函数对应。

int (*mmap) (struct file *, struct vm_area_struct *);
```

```c
上层 app 会先打开设备节点，调用 open 函数，那么驱动中就会调用这个 open 函数

int (*open) (struct inode *inode, struct file *filp);

inode 为文件节点,这个节点只有一个，无论用户打开多少个文件，都只是对应着一个 inode 结构；但是 filp 只要打开一个文件，就对应着一个 file 结构体， file 结构体通常用来追踪文件在运行时的状态信息
```

MKDEV(ma,mi) 就是先将主设备号左移 20 位，然后与次设备号相加得到 设备号。