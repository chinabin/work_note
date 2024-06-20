# 0x00. 导读

[Linux 操作系统原理-文件系统(1)](https://icefrozen.github.io/article/openerating-learning-vfs-1/)

# 0x01. 简介

![Alt text](../../pic/linux/filesystem/LinuxStorageStackDiagram.png)

Linux 内核 I/O 栈大概分了 5 层，如下：

1. 文件系统层  
提供了 open、read、write 等文件操作的系统调用给应用程序使用，并且 Linux 提供了文件系统的统一抽象 vfs ，使得 Linux 中可适配各种类型的文件系统，如 ext4、btrfs、zfs、fat32、ntfs 等，而我们常说的磁盘格式化，实际上就是在安装文件系统，重新写入相关文件系统的初始元数据信息到磁盘。

2. 卷管理层  
比如我们常听到的 Device Mapper、LVM、soft raid 就属于这一层，用于将多个磁盘合为一个，或将一个磁盘拆分为多个。

3. 块存储层  
这一层主要实现 io 调度逻辑，比如将一个大 io 请求拆分为多个小的 io 请求，将相邻的 io 请求合并，实现各种 io 调度算法等，常见 io 调度算法有 CFQ、NOOP、Deadline、AS 。

4. 存储驱动层  
每一个硬件都需要驱动程序，硬盘也不例外，如常听到的 SCSI、ACHI、NVME 等，且如 /dev/sda 这样的设备抽象文件，也由这一层实现。

5. 硬件层  
硬件层就是具体的硬盘实物了，但我们经常听到 ATA、SATA、SCSI、PCIe、SAS 等这样的名词，这是不同的硬盘接口规范，就像手机充电线有 Type-A、Type-B、Type-C 一样。

# 0x02. 文章汇总

1. [文件系统简介](./filesys01.md)
2. 磁盘的物理组成，新的磁盘如何被系统使用。[硬盘接入系统](./filesys02.md)
3. [VFS](./filesys04.md)
4. 当一个进程发出一个 IO 请求的时候，从上到下经过的地方( [IO_travel](./IO_travel.md) )