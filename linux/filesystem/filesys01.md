# 0x00. 导读

# 0x01. 简介

没有文件系统，访问磁盘上的数据就需要直接读写磁盘的 sector ，而文件系统存在的意义，就是能更有效的组织、管理和使用磁盘上的 raw data 。

文件系统，定义并实现了数据在存储介质（如硬盘等）上的存储方式和结构，以及其是如何被访问的，如索引、读取等。操作系统里，硬盘中的数据被抽象为文件的形式，并对其进行管理，比如为一块或多块数据关联一个文件名等，因此，我们称这些用于管理文件的数据结构（以及其对应的规则）为文件系统。

Linux 中的 **VFS(Virtural File System 或 Virtual FS Switch)** 作为具体的文件系统的抽象层，是非常重要的。它为各类文件系统提供了一个一致的接口，用户态的应用无需关注底层具体文件系统的区别，通过相同的系统调用请求内核即可。

![Alt text](../../pic/linux/filesystem/filesystem_overview.png)

# 0x02. 文件系统类型

## 2.1 Ext4

Ext4(ext4日志文件系统、第四代扩展文件系统) 文件系统是 Linux 用途最广泛的日志文件系统。Ext4 可向前兼容 Ext2/Ext3 。Ext4 是很多发行版如 Debian、Ubuntu 等的默认文件系统。使用 `dumpe2fs` 命令可以查看 Ext2/3/4 文件系统详情。

什么是日志（Journaling）文件系统？

就是在数据更改正式提交至硬盘之前，先在日志区域（也是存在文件系统上的）记录变更日志，这样可以在系统崩溃或掉电后能快速恢复。现代的文件系统，甚至各类可靠的存储系统，日志存储都是必须的。而 ext3 就是在 ext2 的基础上增加了这种日志功能。

## 2.2 XFS

XFS 是 1993 年由 SiliconGraphic Inc(SGI) 公司创建的高性能的 64 位文件系统。2001年进入 Linux 内核，如今已被大多数 Linux 发行版支持。最支持的要数红帽公司，其下主打的操作系统 RHEL(Red Hat Enterprise Linux) 7 和 8 都选 XFS 为默认的文件系统，红帽的很多工程师也深度参与了 XFS 的开发和维护。使用 `xfs_info` 命令查看 XFS 文件系统详情。

# 0x03. 文件系统的组成

因为磁盘上的数据要和内存交互，而内存通常是以 4KB 为单位管理的，所以把磁盘按照 4KB 划分比较方便（称为一个 **block**）。

## 3.1 文件

文件系统的基础要素自然是文件，而文件作为一个数据容器的逻辑概念，本质上是字节构成的集合，这些字节就是文件的 user data.

## 3.2 bitmap

需要追踪这些 inodes 和 data blocks 的分配和释放情况，判断哪些是已用的，哪些是空闲的。最简单的办法就是使用 bitmap ，包括记录 inode 使用情况的 bitmap ，和记录 data block 使用情况的 bitmap 。

## 3.3 meta data

除了文件本身包含的数据，还有文件的访问权限、大小和创建时间等控制信息，这些信息被称为文件的 **meta data** 。这些 meta data 的数据结构就是 **inode**.

## 3.4 superblock

**superblock** 包含了一个文件系统所有的控制信息，比如文件系统中有多少个 inodes 和 data blocks ， inode 的信息起始于哪个 block ，可能还有一个区别不同文件系统类型的 magic number 。因此， superblock 可理解为是文件系统的 meta data 。

# 0x04. 文件寻址

## 4.1 寻址过程

先找对应的 inode ，找到 inode 后， inode 里就有指针指向保持文件数据的 data block 就查找到了文件。

两种寻址方式：

- inode 里通过指针指向一个 block ，假设一个 inode 最多能包含 12 个指针，那么文件的大小不能超过 48KB 。那如果超过了怎么办？可由 inode 先指向一个中间 block ，这个 block 再指向分散的 data block ，这种方法称为 **multi-level index** 。假设一个指针占据 4 个字节，那么一个中间 block 可存储 1024 个指针，二级 index 的寻址范围就可超过 4MB ，三级 index 则可超过 4GB 。

    这种只使用 block 指针的方式（可被称为"pointer-based"）被 ext2 和 ext3 文件系统所采用，但它存在一个问题，对于占据多个 data block 的文件，需要较多的 meta data 。

- 另一种实现是使用一个 block 指针加上一个 length 来表示一组物理上连续的 blocks （称为一个 extent ，其中 length 以 block 为单位计），一个文件则由若干个 extents 构成。这种 "extent-based" 的方式被后来的 ext4 文件系统所采用。
    ```c
    struct ext4_extent {
        __le32  ee_block;   /* first logical block extent covers */
        __le16  ee_len;     /* number of blocks covered by extent */
        ...
    };
    ```

# 0x05. 目录和路径

各级目录构成了访问文件的路径，从抽象的角度，目录也可视作一种文件，只是这种文件比较特殊，它的 user data 存储的是该路径下的普通文件的 inode 编号。

每个文件都有三种 timestamp: 文件上次被访问的时间 (access time, 简称 atime) 、文件上次被修改的时间 (modification time, 简称 mtime) 和文件属性上次被修改的时间 (change time, 简称 ctime)
```c
struct timespec64   i_atime;
struct timespec64   i_mtime;
struct timespec64   i_ctime;
```
mtime 针对的是文件的内容（即 user data ），而 ctime 针对的是 inode 结构自身（即 meta data ）。

此外，设备在Linux中也被视作文件，一个设备可以是block device（即"i_bdev"），也可以是character device（即"i_cdev"），而且设备还具有主设备号和从设备号（即"i_rdev"）。



这一切的原因还得从hard link和soft link的属性说起。

当创建一个文件时，我们需要选择一个路径（pathname），并为文件设置一个字符串形式的名称（symbol）。这其实做了两件事，一是生成一个inode结构体，用于记录这个文件的所有相关信息，包括大小、在磁盘上占据的blocks数目等，二是将生成的inode关联（link）了这个路径和名称。

一个文件的hard link增加的是对这个inode结构体的关联/指向，并不是一个新的文件。而soft link本身就是一个文件，就像directory这种特殊文件里存放的是该目录下包含哪些文件，soft link这种文件里存放的则是指向原inode的路径，路径越长，soft link的大小就越大。这就是为什么hard link和原文件的inode号相同，而soft link不同。

因此，当我们用"rm"命令“删除”原文件时，删除的只是原文件的路径和inode之间的关联，而不是这个inode本身，文件的内容依然存在于磁盘中，因而只能算是"unlink"。所以直接关联inode的hard link不受影响，而关联原文件路径的soft link此时相当于是一个dangling reference。

一个inode被link的数目由"i_nlink"表示（这就是前面"ls -l"命令输出中第二列数值的含义）：

相比起soft link，hard link在使用的时候有个限制，就是必须和原文件位于相同的文件系统，原因还是和inode编号有关。因为一个inode编号只在文件所属的superblock中是唯一的，而hard link使用和原文件相同的inode编号，如果hard link跑到其他文件系统，就可能和这些文件系统中既有的文件inode编号冲突。