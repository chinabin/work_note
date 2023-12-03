# 0x00. 导读

# 0x01. 简介

在 linux 系统中你使用一些命令时（例如 nmon、iostat ），有可能会看到一些名字为 dm-xx 的设备，那么这些设备到底是什么设备呢，跟磁盘有什么关系呢？ 其实 dm 是 Device Mapper 的缩写，Device Mapper 是 Linux 2.6 内核中提供的一种从逻辑设备到物理设备的映射框架机制，在该机制下，用户可以很方便的根据自己的需要制定实现存储资源的管理策略，当前比较流行的 Linux 下的逻辑卷管理器如 LVM2(Linux Volume Manager 2 version)、EVMS(Enterprise Volume Management System)、dmraid(Device Mapper Raid Tool) 等都是基于该机制实现的。

# 0x02. 

```bash
$ ls -hl /dev/dm*
brw-rw---- 1 root disk 253, 0 Sep 16 12:36 /dev/dm-0
brw-rw---- 1 root disk 253, 1 Sep 16 12:36 /dev/dm-1

# fdisk -l 也可以看到

$ ls -hl /dev/mapper  
total 0
lrwxrwxrwx 1 root root       7 Sep 16 12:36 centos-root -> ../dm-1
lrwxrwxrwx 1 root root       7 Sep 16 12:36 centos-swap -> ../dm-0

$ sudo dmsetup ls
centos-home     (253:2)
centos-swap     (253:1)
centos-root     (253:0)

$ cat /etc/fstab
```