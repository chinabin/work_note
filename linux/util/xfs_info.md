# 0x00. 导读

# 0x01. 简介

查看 xfs 文件系统的具体信息

# 0x02. 命令

```
xfs_info 挂载点 | 设备文件名
```

```bash
$ xfs_info /dev/mapper/centos-root
meta-data=/dev/mapper/centos-root isize=256    agcount=4, agsize=2457088 blks
         =                       sectsz=512   attr=2, projid32bit=1
         =                       crc=0        finobt=0 spinodes=0
data     =                       bsize=4096   blocks=9828352, imaxpct=25
         =                       sunit=0      swidth=0 blks
naming   =version 2              bsize=4096   ascii-ci=0 ftype=0
log      =internal               bsize=4096   blocks=4799, version=2
         =                       sectsz=512   sunit=0 blks, lazy-count=1
realtime =none                   extsz=4096   blocks=0, rtextents=0

第 1 行里面的 isize 指的是 inode 的容量，每个有 256Bytes 这么大。至于 agcount 则是储存区群组 (allocation group) 的个数，共有 4 个， agsize 则是指每个储存区群组具有 2457088 个 block 。配合第 4 行的 block 设置为 4K，因此整个文件系统的容 量应该就是 9,828,352 K 这么大!

第 2 行里面 sectsz 指的是逻辑扇区 (sector) 的容量设置为 512Bytes 这么大的意思。

第 4 行里面的 bsize 指的是 block 的容量，每个 block 为 4K 的意思，共有 262144 个 block 在这个文件系统内。

第 5 行里面的 sunit 与 swidth 与磁盘阵列的 stripe 相关性较高。这部份我们下面格式化 的时候会举一个例子来说明。

第 7 行里面的 internal 指的是这个登录区的位置在文件系统内，而不是外部设备的意 思。且占用了 4K * 2560 个 block，总共约 10M 的容量。

第 9 行里面的 realtime 区域，里面的 extent 容量为 4K。不过目前没有使用。
```