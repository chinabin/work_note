# 0x00. 导读

# 0x01. 简介

# 0x02. 命令

```bash
# 查看已挂载的文件系统
# 输出中的每行代表挂载的一个文件系统，其格式为：
# fs_spec on fs_file type fs_vfstype (fs_mntopts)
# fs_spec：挂载的块设备或远程文件系统
# fs_file：文件系统的挂载点
# fs_vfstype：文件系统的类型
# fs_mntopts：与文件系统相关的更多选项，不同的文件系统其选项也不太一样
# 比如 sysfs on /sys type sysfs (rw,nosuid,nodev,noexec,relatime) 含义为：挂载的设备为 sysfs，挂载点为 /sys，文件系统的类型为 sysfs。括号中的 rw 表示以可读写的方式挂载文件系统，noexec 表示不能在该文件系统上直接运行程序。
$ mount

# 将 /dev/hda1 挂在 /mnt 之下
$ mount /dev/hda1 /mnt

# 将 /dev/hda1 用唯读模式挂在 /mnt 之下。
$ mount -o ro /dev/hda1 /mnt
```