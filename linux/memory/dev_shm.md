# 0x00. 导读

If you want to find a way for access physical memory in Linux there are only two solutions. The first is to develop a module running in kernel space with the correct privileges to access physical memory and the second is to use a special devices called "/dev/mem".

# 0x01. 简介

```
dev == device  
shm == shared memory
```

1. /dev/shm 是一个tmpfs文件系统，临时文件系统，是基于内存的文件系统，也就是说/dev/shm中的文件是直接写入内存的，而不占用硬盘空间。

2. 在Redhat/CentOS等linux发行版中默认大小为物理内存的一半。最大可达到 物理内存+SWAP的大小

3. /dev/shm不是立即占用内存，而是采用需要才占用内存的方法。在上面的例子中，/dev/shm设置的值是20G，我们看到已用写入了9.5G的数据，也就是占用了9.5G的内存。

tmpfs是基于内存的，速度是不用说的，硬盘和它没法比。

```bash
$ df -h | grep tmpfs
devtmpfs                          40G     0   40G   0% /dev
tmpfs                             40G     0   40G   0% /dev/shm
tmpfs                             40G  123M   40G   1% /run
tmpfs                             40G     0   40G   0% /sys/fs/cgroup
```

# 0x02. tmpfs

tmpfs一种基于内存的临时文件系统，tmpfs可以使用RAM，但它也可以使用swap分区来存储。传统的ramdisk是个块设备，要用mkfs来格式化它，才能真正地使用它；tmpfs是一个文件系统，并不是块设备，安装即可以使用。tmpfs是最好的基于RAM的文件系统。

特征：
- 动态文件系统大小
- 存取速度快