# 0x00. 导读

# 0x01. 简介

用于列出所有可用块设备的信息，而且还能显示他们之间的依赖关系，但是它不会列出RAM盘的信息。块设备有硬盘，闪存盘，cd-ROM等等。

# 0x02. 命令

```
-a, --all            # 显示所有设备。
-b, --bytes          # 以bytes方式显示设备大小。
-p                   # 打印路径
-f, --fs             # 显示文件系统信息。
-i, --ascii          # use ascii characters only。
-m, --perms          # 显示权限信息。
-l, --list           # 使用列表格式显示。
-r, --raw            # 使用原始格式显示。
-t, --topology       # 显示拓扑结构信息。
```

```
$ lsblk -p

NAME                        MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
/dev/vda                    252:0    0   80G  0 disk 
├─/dev/vda1                 252:1    0  500M  0 part /boot
└─/dev/vda2                 252:2    0 39.5G  0 part 
  ├─/dev/mapper/centos-swap 253:0    0    2G  0 lvm  [SWAP]
  └─/dev/mapper/centos-root 253:1    0 37.5G  0 lvm  /
/dev/vdb                    252:16   0  200G  0 disk 
├─/dev/vdb1                 252:17   0 1021M  0 part 
└─/dev/vdb2                 252:18   0  199G  0 part /data
```

- NAME ：这是块设备名。
- MAJ:MIN ：本栏显示主要和次要设备号。
- RM ：本栏显示设备是否可移动设备。等于 1 说明是可移动设备。
- SIZE ：本栏列出设备的容量大小信息。例如298.1G表明该设备大小为298.1GB，而1K表明该设备大小为1KB。
- RO ：该项表明设备是否为只读。在本案例中，所有设备的RO值为0，表明他们不是只读的。
- TYPE ：本栏显示块设备是否是磁盘或磁盘上的一个分区。disk=磁盘，part=分区，lvm=逻辑卷，rom=只读存储
- MOUNTPOINT ：本栏指出设备挂载的挂载点。