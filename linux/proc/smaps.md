# 0x00. 导读

# 0x01. 简介

/proc/PID/smaps 文件是基于 /proc/PID/maps 的扩展，他展示了一个进程的内存消耗，比同一目录下的maps文件更为详细。

# 0x02. 解释

```bash
7fb0f4306000-7fb0f4309000 rw-p 0010c000 fc:12 134890217        /lib/libprotobuf.so.9
Size:                 12 kB
Rss:                  12 kB
Pss:                  12 kB
Shared_Clean:          0 kB
Shared_Dirty:          0 kB
Private_Clean:         0 kB
Private_Dirty:        12 kB
Referenced:           12 kB
Anonymous:            12 kB
AnonHugePages:         0 kB
Swap:                  0 kB
KernelPageSize:        4 kB
MMUPageSize:           4 kB
Locked:                0 kB
VmFlags: rd wr mr mp me ac sd
```

第一行： 
- `7fb0f4306000-7fb0f4309000`: 是该虚拟内存段的开始和结束位置  

- `rw-p`: 内存段的权限，分别是可读、可写、可运行、私有或共享，最后一位p代表私有，s代表共享  

- `0010c000`: 该虚拟内存段起始地址在对应的 **映射文件** 中以页为单位的偏移量，对匿名映射，它等于 0 或者 vm_start/PAGE_SIZE  

- `fc:12`: 文件的主设备号和次设备号。对匿名映射来说，因为没有文件在磁盘上，所以没有设备号，始终为00:00。对有名映射来说，是映射的文件所在设备的设备号。  

- `134890217`: 被映射到虚拟内存的文件的索引节点号，通过该节点可以找到对应的文件，对匿名映射来说，因为没有文件在磁盘上，所以没有节点号，始终为 0 。  

- `/lib/libprotobuf.so.9`: 被映射到虚拟内存的文件名称。对有名来说，是映射的文件名。对匿名映射来说，是此段虚拟内存在进程中的角色。[ stack ]表示在进程中作为栈使用，[ heap ]表示堆。其余情况则无显示。

后续：

- Size：拟内存空间大小。但是这个内存值不一定是物理内存实际分配的大小，因为在用户态上，虚拟内存总是延迟分配的。这个值计算也非常简单，就是该VMA的开始位置减结束位置。  
- Rss：实际驻留在RAM的内存大小（包括共享库的大小，不包括已经交换出去的页面）  
- Pss：Rss 的基础上，把共享库的大小均摊给所有被映射的进程后的大小 

- Shared_Clean：
- Shared_Dirty：  
- Private_Clean：  
- Private_Dirty：  
    - dirty/clean：该页面是否被修改过，如果修改过（dirty），在页面被淘汰的时候，就会把该脏页面回写到交换分区（swap out）。有一个标志位用于表示页面是否dirty。
    - share/private：查看该page的引用数，如果引用>1，则归为shared，如果是1，则归为private，同时也查看该page的flag，是否标记为_PAGE_DIRTY，如果不是，则认为干净的。

- Referenced：The amount of memory currently marked as referenced or a mapping associated with a file may contain anonymous pages. 
- Anonymous： 匿名内存的大小  
- AnonHugePages：透明大页内存的大小  
- Swap：Swap 的大小  
- KernelPageSize：内核页大小  
- MMUPageSize：MMU 页大小  
- Locked：被 mlock() 的内存大小，常驻物理内存的大小，这些页不会被换出。
- VmFlags：页的标志位
    ```
    rd  - readable
    wr  - writeable
    ex  - executable
    sh  - shared
    mr  - may read
    mw  - may write
    me  - may execute
    ms  - may share
    gd  - stack segment growns down
    pf  - pure PFN range
    dw  - disabled write to the mapped file
    lo  - pages are locked in memory
    io  - memory mapped I/O area
    sr  - sequential read advise provided
    rr  - random read advise provided
    dc  - do not copy area on fork
    de  - do not expand area on remapping
    ac  - area is accountable
    nr  - swap space is not reserved for the area
    ht  - area uses huge tlb pages
    ar  - architecture specific flag
    dd  - do not include area into core dump
    sd  - soft-dirty flag
    mm  - mixed map area
    hg  - huge page advise flag
    nh  - no-huge page advise flag
    mg  - mergable advise flag
    ```