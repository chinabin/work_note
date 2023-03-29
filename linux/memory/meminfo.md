```bash
# cat /sys/devices/system/node/node*/meminfo
# -w buffers/cache 分开显示， -k 用 KB单位
$ sudo free -w -k && cat /proc/meminfo
              total        used        free      shared     buffers       cache   available
Mem:       65696924     4510608     2818720     4669880          20    58367576    56161268
Swap:      67108860       15380    67093480
MemTotal:       65696924 kB
MemFree:         2818720 kB
MemAvailable:   56161268 kB
Buffers:              20 kB
Cached:         57200576 kB
SwapCached:         2768 kB
Active:         36134384 kB
Inactive:       24784608 kB
Active(anon):    6309440 kB
Inactive(anon):  2078948 kB
Active(file):   29824944 kB
Inactive(file): 22705660 kB
Unevictable:           0 kB
Mlocked:               0 kB
SwapTotal:      67108860 kB
SwapFree:       67093480 kB
Dirty:                 0 kB
Writeback:             0 kB
AnonPages:       3716560 kB
Mapped:           113508 kB
Shmem:           4669880 kB
Slab:            1167000 kB
SReclaimable:    1078292 kB
SUnreclaim:        88708 kB
KernelStack:        7696 kB
PageTables:        15348 kB
NFS_Unstable:          0 kB
Bounce:                0 kB
WritebackTmp:          0 kB
CommitLimit:    99957320 kB
Committed_AS:    8719072 kB
VmallocTotal:   34359738367 kB
VmallocUsed:      420696 kB
VmallocChunk:   34325397500 kB
HardwareCorrupted:     0 kB
AnonHugePages:         0 kB
HugePages_Total:       0
HugePages_Free:        0
HugePages_Rsvd:        0
HugePages_Surp:        0
Hugepagesize:       2048 kB
DirectMap4k:      264312 kB
DirectMap2M:    22693888 kB
DirectMap1G:    46137344 kB
```

free | meminfo
---- | ---
 total |	MemTotal
 used |	MemTotal - MemFree - Buffers - Cached - Slab
 free |	MemFree
 shared |	Shmem
 buff/cache |	Buffers + Cached + Slab
available |	MemAvailable
 total |	SwapTotal
 used |	SwapTotal - SwapFree
 free |	SwapFree

内核使用的:    
- slab
- VmallocUser
- PageTables
- KernelStack
- HardwareCorrupted
- Bounce

用户使用的:
- Active
- Inactive
- Unevictable
- HugePages


Q: 
1. /proc/[pid]/smap  
2. /proc/[pid]/status
3. /proc/buddyinfo
4. /proc/pagetypeinfo
5. /proc/vmstat 文件显示的是从内核导出的虚拟内存的统计信息。
6. /proc/vmallocinfo
7. /proc/pid/statm, 可以用 self 代替自己的 pid
8. /proc/pagetypeinfo
9. /proc/swaps
10. /sys/devices/system/node/node*/meminfo

[linux github](https://github.com/torvalds/linux/blob/master/Documentation/filesystems/proc.rst)

----------

# VSS USS RSS PSS

Linux 使用的是虚拟内存 (virtual memory)，因此要准确的计算一个进程实际使用的物理内存就比较麻烦，如下是在计算内存使用率时比较重要的指标：

**VSS** (Virtual Set Size): 进程通过 malloc() 或者 mmap() 向内存申请内存之后(这部分内存大小称为 VSS )，内核并不会立刻为其分配实际的物理内存。   
等到进程真正使用到内存时(比如调用了 memset() 函数)，内核才会为这个进程分配物理内存，并建立虚拟地址和物理地址之间的映射。

问题来了，动态链接库本身也是要占据物理内存的，那这部分内存应该算在哪个进程头上呢？

为此就出现了三种不同的计算方法：
1. 一种是只计算进程自身占用的物理内存，完全不包含共享库所占用内存的 **USS** (Unique Set Size) 。
2. 一种是把共享库占用的内存直接加到每个进程头上的 **RSS** (Resident Set Size) 。
3. 最后一种是把一个共享库占用的内存，分摊到使用了这个共享库的各个进程头上，称为 **PSS** (Proportional Set Size) 。   
假如一个 so 大小是 3M ，现在三个进程共用，则一个是 1M ，假如一个进程被 kill 掉了，则一个是 1.5M 。

使用 `ps -e u | head` 可以看到 VSZ(VSS) 和 RSS 。
```bash
$ ps u | head
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
centos     959  0.0  0.0 157192     8 pts/16   Ss+  Feb01   0:13 /usr/bin/zsh -i
centos    1858  0.0  0.0 153700  6696 pts/54   Ss   17:55   0:01 /bin/zsh -i
centos    4320  0.0  0.0 151040  5808 pts/35   Ss+  Feb18   0:00 /bin/zsh -i
centos    4509  0.0  0.0 151040  5808 pts/52   Ss+  Feb18   0:00 /bin/zsh -i
centos    5890  0.0  0.0 153908  6760 pts/53   Ss+  Feb18   0:00 /bin/zsh -i
centos    9705  0.0  0.0 179016   392 pts/7    S+   Jan04   0:02 ssh QMT
centos    9856  0.0  0.0 119720  1532 pts/25   S+   Feb18   0:00 tmux at -t tab
centos   14243  0.0  0.0 160444  2696 pts/11   Ss+  Jan31   0:30 /usr/bin/zsh -i
centos   15025  0.0  0.0 179044  1176 pts/8    S+   Feb15   0:00 ssh HTDG0
```

top，也能显示 VSS 和 RSS 的信息，只不过叫法不一样，分别被称为了 "VIRT" 和 "RES" 。
```bash
   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
  5952 centos    20   0  157184   2880   1176 S   6.2  0.0   0:14.10 sshd                                                      
 20295 centos    20   0 2043856   1.1g  18764 S   6.2  7.2  34:05.71 node   
```

-----------------------

# 内核分配内存

Kernel的动态内存分配通过以下几种接口：

- alloc_pages/__get_free_page: 以页为单位分配
- vmalloc: 以字节为单位分配虚拟地址连续的内存块
- slab allocator
    - kmalloc: 以字节为单位分配物理地址连续的内存块，它是以 slab 为基础的，使用 slab 层的 general caches — 大小为 2^n ，名称是 kmalloc-32 、 kmalloc-64 等。

通过 slab 层分配的内存会被精确统计，可以参见 /proc/meminfo 中的 slab/SReclaimable/SUnreclaim ；

通过 vmalloc 分配的内存也有统计，参见 /proc/meminfo 中的 VmallocUsed 和 /proc/vmallocinfo ；

而通过 alloc_pages 分配的内存不会自动统计，除非调用 alloc_pages 的内核模块或驱动程序主动进行统计，否则我们只能看到 free memory 减少了，但从 /proc/meminfo 中看不出它们具体用到哪里去了。

/proc/meminfo 统计的是系统全局的内存使用状况，单个进程的情况要看 /proc/pid/smaps 等等。

## 1. MemTotal
    系统从加电开始到引导完成，firmware/BIOS 要保留一些内存，kernel 本身要占用一些内存，最后剩下可供 kernel 支配的内存就是 MemTotal 。这个值在系统运行期间一般是固定不变的。

## 2.MemFree
    表示系统尚未使用的内存。[ MemTotal - MemFree ]就是已被用掉的内存。

## 3. MemAvailable
    MemFree 不能代表全部可用的内存，系统中有些内存虽然已被使用但是可以回收的，比如 cache/buffer 、 slab 都有一部分可以回收，所以这部分可回收的内存加上 MemFree 才是系统可用的内存，即 MemAvailable 。MemAvailable 是内核使用特定的算法估算出来的，要注意这是一个估计值，并不精确。

## 4. Buffers
    Buffers 表示块设备 (block device) 所占用的缓存页，包括：直接读写块设备、以及文件系统元数据 (metadata) 比如 SuperBlock 所使用的缓存页。

    展开来说就是，当我们直接读写块设备的时候，会用到缓存页，也就导致了 buffers 增加。当我们需要使用文件系统元数据的时候，也会导致 buffers 的增加。

```bash
# 查看 buff/cache 的变化，会发现几乎不变 
$ watch -n 1 -d 'free'
# 开另外一个 terminal 。会发现 buff/cache 暴增。
$ sudo find / -name abc.def

# 读写块设备
$ watch -n 1 -d 'cat /proc/meminfo | grep "Buffers\|Cached"'
# 开另外一个 terminal 。会发现暴增。
$ dd if=/dev/vda1 of=/dev/null count=20000
```

    Cached 表示普通文件所占用的缓存页。

所有的缓存页(page cache)的总和 = Buffers + Cached + SwapCached

所有缓存页的总数量: `cat /proc/vmstat | grep nr_file_pages` ，单位是 page 个数。

## 5. Cached
    Page Cache 里包括所有 file-backed pages ，统计在 /proc/meminfo 的 Cached 中。

- Cached 是 Mapped 的超集，其不仅包括 mapped ，也包括 unmapped 的页面，当一个文件不再与进程关联之后，原来在 page cache 中的页面并不会立即回收，仍然被计入 Cached ，还留在 LRU 中，但是 Mapped 统计值会减小。   
ummaped = (Cached – Mapped)
- Cached 包含 tmpfs 中的文件， shared memory ，以及 shared anonymous mmap 。（其实  shared memory 和 shared anonymous mmap 在内核中都是基于 tmpfs 实现的）。

Cached 和 SwapCached 两个统计值是互不重叠的。所以，Shared memory 和 tmpfs 在不发生 swap-out 的时候属于 Cached ，而在 swap-out/swap-in 的过程中会被加进 swap cache 中，属于 SwapCached ，一旦进了 SwapCached ，就不再属于 Cached 了。

## 6. SwapCached

## 7. Active Inactive Active(anon) Inactive(anon) Active(file) Inactive(file) Unevictable
    Active = Active(anon) + Active(file)
    Inactive = Inactive(anon) + Inactive(file)

    LRU list 是 Linux kernel 的内存页面回收算法(Page Frame Reclaiming Algorithm)所使用的数据结构，LRU是 Least Recently Used 的缩写词。
    这个算法的核心思想是：回收的页面应该是最近使用得最少的。为了实现这个目标，最理想的情况是每个页面都有一个年龄项，用于记录最近一次访问页面的时间，可惜 x86 CPU 硬件并不支持这个特性， x86 CPU 只能做到在访问页面时设置一个标志位 Access Bit ，无法记录时间，所以 Linux Kernel 使用了一个折衷的方法：它采用了 LRU list 列表，把刚访问过的页面放在列首，越接近列尾的就是越长时间未访问过的页面，这样，虽然不能记录访问时间，但利用页面在 LRU list 中的相对位置也可以轻松找到年龄最长的页面。
    Linux kernel 设计了两种 LRU list: active list 和 inactive list, 刚访问过的页面放进 active list ，长时间未访问过的页面放进 inactive list ，这样从 inactive list 回收页面就变得简单了。内核线程 kswapd 会周期性地把 active list 中符合条件的页面移到 inactive list 中。

    Active/inactive memory 是针对用户进程所占用的内存而言的，内核占用的内存（包括slab）不在其中。

    file 和 anon 分别表示 file-backed pages 和 anonymous pages 。
    用户进程的内存页分为两种：
        1. 与文件关联的内存（比如进程的代码、映射的文件所对应的内存页）
        2. 与文件无关的内存（比如进程的堆栈，用 malloc 申请的内存）
    前者称为 file-backed pages ，后者称为 anonymous pages 。
    File-backed pages 在发生换页( page-in 或 page-out )时，是从它对应的文件读入或写出；
    anonymous pages 在发生换页时( swap-in 或 swap-out )，是对交换区进行读/写操作。

    Unevictable 是 unevictable list 的统计，表示不能 pageout/swapout 的内存页。

    LRU中不包含HugePages_*。
    LRU包含了 Cached 和 AnonPages。

## 14. Mlocked
    统计的是被 mlock() 系统调用锁定的内存大小。被锁定的内存因为不能 pageout/swapout ，会从 Active/Inactive LRU list 移到 Unevictable LRU list 上。
    也就是说，当 Mlocked 增加时， Unevictable 也同步增加，而 Active 或 Inactive 同时减小；
    当 Mlocked 减小的时候， Unevictable 也同步减小，而 Active 或 Inactive 同时增加。

     Mlocked 并不是独立的内存空间，它与以下统计项重叠： LRU Unevictable, AnonPages, Shmem, Mapped 等。

## 15. SwapTotal

## 16. SwapFree

## 17. Dirty
    系统中全部的 dirty pages ，但是不全。
    真的要统计的话应该是 Dirty + NFS_Unstable + Writeback 。
    NFS_Unstable 是发给 NFS server 但尚未写入硬盘的缓存页。NFS_Unstable 的内存被包含在 Slab 中，因为 nfs request 内存是调用 kmem_cache_zalloc() 申请的。

## 18. WriteBack
    正准备回写硬盘的缓存页。

## 19. AnonPages
> 用户进程的内存页分为两种：file-backed pages（与文件对应的内存页），和anonymous pages（匿名页）。

    用户进程的 Anonymous pages(匿名页)的数量统计在 /proc/meminfo 的 AnonPages 中。
## 20. Mapped
> 用户进程的内存页分为两种：file-backed pages（与文件对应的内存页），和anonymous pages（匿名页）。

    用户进程的 file-backed pages 就对应着 /proc/meminfo 中的 Mapped 。

    Page cache 中包含了文件的缓存页：
        1. 有些文件当前已不在使用，page cache仍然可能保留着它们的缓存页面；
        2. 另一些文件正被用户进程关联，比如shared libraries、可执行程序的文件、mmap的文件等
    情况 2 的这些文件的缓存页就称为 mapped 。情况 1 的缓存页会算在 Cached 里面。

    其统计了 page cache 中所有的 mapped 页面。 Mapped 是 Cached 的子集。

    因为进程所占的内存页分为 anonymous pages 和 file-backed pages ，理论上应该有：
    【所有进程的PSS之和】 == 【Mapped + AnonPages】。
    实际测试发现结果非常详尽，但是无法精确相等。

## 21. Shmem

## 22. Slab SReclaimable SUnreclaim
    在 Linux 中，内存是以页为单位管理和分配。但在内核中会有许多小对象，这些对象构造销毁十分频繁，比如i-node，dentry，它们的实际大小可能只有几个字节，这样就非常浪费。
    slab 分配器专为小内存分配而生。slab 分配器分配内存以字节为单位，基于伙伴分配器的大内存进一步细分成小内存分配。换句话说，slab 分配器仍然从 Buddy 分配器中申请内存，之后自己对申请来的内存细分管理。
    可以使用 slabtop 或者 slabinfo 来查看 slab 占用情况。

    slab 主要是两个作用：
    - Slab对小对象进行分配，不用为每个小对象分配一个页，节省了空间。
    - 内核中一些小对象创建析构很频繁，Slab对这些小对象做缓存，可以重复利用一些相同的对象，减少内存分配次数。

    Slab = SReclaimable + SUnreclaim, SReclaimable 表示可回收使用的内存。

- name：slab object对象的名称
- active_objs：活动对象（slab object）个数，即被申请走的对象个数（正在被使用）。
- num_objs：总对象（slab object）个数，slab本身具备缓存作用，所以num_objs可能会比active_objs要大，这是一种合理的现象。
- objsize：每个对象（slab object）大小，以字节为单位。
- objperslab：slab中存放的是对象（slab object），这个指标表示一个slab中包含多少个对象（slab object）。
- pagesperslab : tunables：一个slab占用几个page内存页。可以算一下，一个slab的大小为320*12=3840，小于4096（我这里，内存页为4K），所以一个slab只需占用1个page内存页。另外，cache对象本身需要存储一个额外的管理信息，即有overhead，所以一个xfs_buf slab的大小不止3840。
- active_slabs：活动slab个数。
- num_slabs：总slab个数。

## 25. KernelStack
    每一个用户线/进程都会分配一个 kernel stack （内核栈），内核栈虽然属于线程，但用户态的代码不能访问，只有通过系统调用(syscall)、自陷(trap)或异常(exception)进入内核态的时候才会用到，也就是说内核栈是给 kernel code 使用的。在x86系统上Linux的内核栈大小是固定的8K或16K。

## 26. PageTables
    Page Table 用于将内存的虚拟地址翻译成物理地址，随着内存地址分配得越来越多，Page Table 会增大， PageTables 统计了 Page Table 所占用的内存大小。

## 27. NFS_Unstable

## 28. Bounce
    有些老设备只能访问低端内存，比如 16M 以下的内存，当应用程序发出一个 I/O 请求，DMA 的目的地址却是高端内存时（比如在 16M 以上），内核将在低端内存中分配一个临时 buffer 作为跳转，把位于高端内存的缓存数据复制到此处。这种额外的数据拷贝被称为" bounce buffering "，会降低 I/O 性能。大量分配的 bounce buffers 也会占用额外的内存。

## 29. WritebackTmp

## 30. CommitLimit Committed_AS
    Memory Overcommit 的意思是操作系统承诺给进程的内存大小超过了实际可用的内存。一个保守的操作系统不会允许 memory overcommit ，有多少就分配多少，再申请就没有了，这其实有些浪费内存，因为进程实际使用到的内存往往比申请的内存要少，比如某个进程 malloc() 了 200MB 内存，但实际上只用到了 100MB ，按照 UNIX/Linux 的算法，物理内存页的分配发生在使用的瞬间，而不是在申请的瞬间，也就是说未用到的 100MB 内存根本就没有分配，这 100MB 内存就闲置了。
    
    理解 memory overcommit 的关键： commit (或 overcommit )针对的是内存申请，内存申请不等于内存分配，内存只在实际用到的时候才分配。

    Linux是允许memory overcommit的，只要你来申请内存我就给你，寄希望于进程实际上用不到那么多内存，但万一用到那么多了呢？那就会发生类似“银行挤兑”的危机，现金(内存)不足了。Linux设计了一个OOM killer机制(OOM = out-of-memory)来处理这种危机：挑选一个进程出来杀死，以腾出部分内存，如果还不够就继续杀…也可通过设置内核参数 vm.panic_on_oom 使得发生OOM时自动重启系统。

    CommitLimit 就是 overcommit 的阈值，申请的 [内存总数] 超过 CommitLimit 的话就算是 overcommit 。

    Committed_AS 表示所有进程已经申请的内存总大小，（注意是已经申请的，不是已经分配的），如果 Committed_AS 超过 CommitLimit 就表示发生了 overcommit，超出越多表示 overcommit 越严重。Committed_AS 的含义换一种说法就是，如果要绝对保证不发生OOM (out of memory) 需要多少物理内存。

## 32. VmallocTotal VmallocUsed VmallocChunk
    VmallocTotal: Vmalloc 内存区的大小

    VmallocUsed: 已用 Vmalloc 内存区的大小。通过 vmalloc 分配的内存都统计在这个值中，但是要注意这个值不止包括了分配的物理内存，还统计了 VM_IOREMAP、VM_MAP 等操作的值，譬如 VM_IOREMAP 是把 IO 地址映射到内核空间、并未消耗物理内存，所以我们要把它们排除在外。从物理内存分配的角度，我们只关心 VM_ALLOC 操作

    VmallocChunk: vmalloc 区可用的连续最大快的大小

[Why is VmallocTotal 34359738367 kB?](https://unix.stackexchange.com/questions/700724/why-is-vmalloctotal-34359738367-kb)

[/proc/meminfo says VmallocUsed is 0. So where are my kernel modules stored?](https://unix.stackexchange.com/questions/482772/proc-meminfo-says-vmallocused-is-0-so-where-are-my-kernel-modules-stored)

vmalloc 用于在内核中分配虚拟地址空间连续的内存，/proc/vmallocinfo 展示了整个 vmalloc 区间( VMALLOC_END - VMALLOC_START )中已经分配的虚拟地址空间信息。

`sudo cat /proc/vmallocinfo`
```bash
0xffffa11640000000-0xffffa11641001000 16781312 alloc_large_system_hash+0x188/0x25c pages=4096 vmalloc vpages N0=4096
0xffffa11641001000-0xffffa1164100a000   36864 alloc_large_system_hash+0x188/0x25c pages=8 vmalloc N0=8
0xffffa1164100a000-0xffffa1164180b000 8392704 alloc_large_system_hash+0x188/0x25c pages=2048 vmalloc vpages N0=2048
0xffffa1164180b000-0xffffa11641810000   20480 alloc_large_system_hash+0x188/0x25c pages=4 vmalloc N0=4
0xffffa11641810000-0xffffa11641851000  266240 alloc_large_system_hash+0x188/0x25c pages=64 vmalloc N0=64
0xffffa11641851000-0xffffa11641892000  266240 alloc_large_system_hash+0x188/0x25c pages=64 vmalloc N0=64
0xffffa11641892000-0xffffa11641894000    8192 acpi_os_map_memory+0xfd/0x155 phys=bffe0000 ioremap
0xffffa11641894000-0xffffa11641897000   12288 acpi_os_map_memory+0xfd/0x155 phys=bffe0000 ioremap
0xffffa11641898000-0xffffa1164189a000    8192 acpi_os_map_memory+0xfd/0x155 phys=fed00000 ioremap
0xffffa1164189a000-0xffffa1164189c000    8192 memremap+0xc8/0x1b0 phys=f0000 ioremap
```

每一列的含义分别是：

- 占用的内核虚拟地址范围
- 占用的内存大小 size ，以字节为单位
- 申请 vmalloc 的函数
- 其他 option 信息

option | describe
---- | ---
pages=nr |	number of pages   
phys=addr |	if a physical address was specified   
ioremap |	I/O mapping (ioremap() and friends)   
vmalloc |	vmalloc() area   
vmap |	vmap()ed pages   
user |	VM_USERMAP area   
vpages |	buffer for pages pointers was vmalloced (huge area)   
N<node>=nr |	(Only on NUMA kernels) Number of pages allocated on memory node <node>   

通过 vmalloc 分配了多少内存，可以统计 /proc/vmallocinfo 中的 vmalloc 记录，例如：

`sudo grep vmalloc /proc/vmallocinfo | awk '{total+=$2}; END {print total}'`

## 35. HardwareCorrupted
    当系统检测到内存的硬件故障时，会把有问题的页面删除掉，不再使用，HardwareCorrupted 统计了删除掉的内存页的总大小。

## 36. AnonHugePages
    AnonHugePages 统计的是 Transparent HugePages (THP)

## 37. HugePages_Total HugePages_Free HugePages_Rsvd HugePages_Surp Hugepagesize
    Hugepagesize: huge page 的 size ，普通的 page size 是 4 KB

    HugePages_Total: Hugepage 的页面数量，和 Hugepagesize 相乘后得到所分配的内存大小
    HugePages_Total 对应内核参数 vm.nr_hugepages ，也可以在运行中的系统上直接修改 /proc/sys/vm/nr_hugepages ，修改的结果会立即影响空闲内存 MemFree 的大小，因为 HugePages 在内核中独立管理，只要一经定义，无论是否被使用，都不再属于 free memory 。

    HugePages_Surp: 表示大于 /proc/sys/vm/nr_hugepages 中的值的 HugePages 数量。剩余 HugePages 的最大数量由 /proc/sys/vm/nr_overcommit_hugepages 控制。

> HugePages_Total 是指系统总共预留了多少 HugePages.    
> HugePages_Free 是指当前还有多少 HugePages 未分配 (allocate)   
> HugePages_Rsvd 是指有多少 HugePages 是系统承诺了会分配给程序(commitment to allocate)，但实际并未分配。   
> HugePages_Surp 是指超分的页。

    系统实际分配给了程序多少个 HugePages, 我们可以直接通过 HugePages_Total - HugePages_Free 计算得到。

## 42. DirectMap4K DirectMap2M DirectMap1G 