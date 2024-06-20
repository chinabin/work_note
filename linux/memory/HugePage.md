# 0x00. 导读

[hugetlbpage.txt](https://www.kernel.org/doc/Documentation/vm/hugetlbpage.txt)

# 0x01. 简介

Linux下的大页分为两种类型：标准大页（Huge Pages）和透明大页（Transparent Huge Pages）。

标准大页管理是预分配的方式，而透明大页管理则是动态分配的方式。

Linux 中的 HugePages 都被锁定在内存中，所以哪怕是在系统内存不足时，它们也不会被 Swap 到磁盘上，这也就能从根源上杜绝了重要内存被频繁换入和换出的可能。

`/proc/sys/vm/nr_hugepages`
`/sys/devices/system/node/node_id/hugepages/hugepages-1048576kB/nr_hugepages`

# 0x02.

## 2.1 前提

内核配置选项： CONFIG_HUGETLBFS, CONFIG_HUGETLB_PAGE
```bash
$ grep HUGETLB /boot/config-$(uname -r)
CONFIG_ARCH_WANT_GENERAL_HUGETLB=y
CONFIG_CGROUP_HUGETLB=y
CONFIG_HUGETLBFS=y
CONFIG_HUGETLB_PAGE=y
```

文件系统支持
```bash
$ cat /proc/filesystems | grep hugetlbfs
nodev   hugetlbfs
```

## 2.2 查看信息

```bash
$ cat /proc/meminfo | grep Huge
AnonHugePages:         0 kB
HugePages_Total:    2048
HugePages_Free:     2046
HugePages_Rsvd:        0
HugePages_Surp:        0
Hugepagesize:       2048 kB


HugePages_Total 预先分配的大页数量
HugePages_Free  空闲大页数量
HugePages_Rsvd  申请的大页数量(但是还没有产生缺页造成实际分配)
HugePages_Surp  多分配的大页数量(由 /proc/sys/vm/nr_overcommit_hugepages 决定)
Hugepagesize    is the default hugepage size (in Kb).
Hugetlb         is the total amount of memory (in kB), consumed by huge
                pages of all sizes.
                If huge pages of different sizes are in use, this number
                will exceed HugePages_Total * Hugepagesize. To get more
                detailed information, please, refer to
                /sys/kernel/mm/hugepages (described below).
```

```bash
$ cat /sys/devices/system/node/node*/meminfo | fgrep Huge
Node 0 AnonHugePages:         0 kB
Node 0 HugePages_Total:     0
Node 0 HugePages_Free:      0
Node 0 HugePages_Surp:      0
Node 1 AnonHugePages:         0 kB
Node 1 HugePages_Total:     0
Node 1 HugePages_Free:      0
Node 1 HugePages_Surp:      0
```

内核的 HugePage:

Pages that are used as huge pages are reserved inside the kernel and cannot be used for other purposes.  Huge pages cannot be swapped out under memory pressure.  
被用做 HugePage 的 page, 会一直保存在内核中，也不会被 swap out.

`/proc/sys/vm/nr_hugepages` indicates the current number of "persistent" huge pages in the kernel's huge page pool.  "Persistent" huge pages will be returned to the huge page pool when freed by a task.  A user with root privileges can dynamically allocate more or free some persistent huge pages by increasing or decreasing the value of 'nr_hugepages'.

**Once a number of huge pages have been pre-allocated to the kernel huge page pool, a user with appropriate privilege can use either the mmap system call or shared memory system calls to use the huge pages.**

When increasing the huge page pool size via nr_hugepages, any existing surplus pages will first be promoted to persistent huge pages.  Then, additional huge pages will be allocated, if necessary and if possible, to fulfill the new persistent huge page pool size.

The root huge page control directory in sysfs is:

	/sys/kernel/mm/hugepages

For each huge page size supported by the running kernel, a subdirectory
will exist, of the form:

	hugepages-${size}kB

Inside each of these directories, the same set of files will exist:

	nr_hugepages
	nr_hugepages_mempolicy
	nr_overcommit_hugepages
	free_hugepages
	resv_hugepages
	surplus_hugepages

## 2.3 操作方法

在启动项里指定 `hugepages=N` 是最可靠的方法。某些平台支持多种 Huge Page 大小。要分配特定大小的 Huge Page ，必须在引导命令参数之前加上 Huge Page 大小选择参数 `hugepagesz=<size>`. size 必须以字节为单位指定，并带有可选的比例后缀 [kKmMgG]。可以使用 `default_hugepagesz=<size>` 引导参数选择默认 Huge Page 大小。

这两种方法也可以，但是不一定有效，因为系统运行起来了，不一定有这么多的连续内存，所以最好还是在启动项设置然后重启系统。
```bash
$ echo 5 > /proc/sys/vm/nr_hugepages 
# or
$ sysctl -w vm.nr_hugepages=5
```

## 2.4 程序中使用 HugePage

Users can use the huge page support in Linux kernel by
- either using the mmap system call
- or standard SYSV shared memory system calls (shmget, shmat).

it is important to note that no such mount command is required if applications are going to use only shmat/shmget system calls or mmap with MAP_HUGETLB.

使用 mmap 的话需要挂载

## 2.5 Internals of large page support

Inside the Linux kernel, large page support is implemented in two parts:
- The first part consists of a global pool of large pages which are allocated and kept reserved for providing large pages support to applications. The global pool of large pages is built by allocating physically contiguous pages (of large page sizes) using normal kernel memory allocation APIs. 
- Second part consists of the kernel itself allocating large pages from this pool to applications that request them.









大页面配置需要连续的内存空间，因此在开机时就分配是最可靠的方式。配置大页面的参数有：

hugepages ：在内核中定义了开机启动时就分配的永久大页面的数量。默认为 0，即不分配。只有当系统有足够的连续可用页时，分配才会成功。由该参数保留的页不能用于其他用途。
hugepagesz： 在内核中定义了开机启动时分配的大页面的大小。可选值为 2MB 和 1GB 。默认是 2MB 。
default_hugepagesz：在内核中定义了开机启动时分配的大页面的默认大小。
要调整页的尺寸，必须将配置以参数格式写入到启动命令中。如要为系统配置 10 个 1GB 的大页面，则启动命令中要包含： default_hugepagesz=1G hugepagesz=1G hugepages=10。配置 1GB 的大页面，CPU 特性需要支持 pdpe1gb ，系统内核也需要支持。

```
1.预留大页
echo 20 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages

2.挂载hugetlb文件系统
mount hugetlbfs /mnt/huge -t hugetlbfs

3.映射hugetbl文件
fd = open(“/mnt/huge/test.txt”, O_CREAT|O_RDWR);

addr = mmap(0, MAP_LENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

```

```
# echo 'vm.nr_hugepages = 25000' >> /etc/sysctl.conf
# sysctl -p
```

工具：
```
yum install libhugetlbfs-utils -y

//列出
hugeadm --pool-list
      Size  Minimum  Current  Maximum  Default
   2097152    12850    12850    12850        *
1073741824        0        0        0

hugeadm --list-all-mounts
Mount Point          Options
/dev/hugepages       rw,relatime,pagesize=2M
```