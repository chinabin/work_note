# 0x00. 导读

# 0x01. 简介

显示 ext2、ext3、ext4 文件系统的 super block 和 block 信息。

如果出现错误：
`dumpe2fs: Bad magic number in super-block`
原因是 dumpe2fs 只能打开 ext2/ext3/ext4 等文件类型。
```bash
$ sudo blkid /dev/mapper/centos-root
/dev/mapper/centos-root: UUID="bc68a8a2-28c7-4e4d-b83e-e4fd81720ac5" TYPE="xfs"
```
xfs 类型得用 xfs_info.

# 0x02. 命令

dumpe2fs [ -bfhixV ]  [ -o superblock=superblock ]  [ -o blocksize=blocksize ]  device