# 0x00、导读



# 0x01、简介

1. 物理内存分为三个层级：节点(node)、区域(zone)、页面(page) ，类似国家的省、县、乡。

2. 提供不同的内存获取：buddy system、slab allocator和kmalloc。就像超市、批发部、小卖店去进货一样。

3. 物理内存有分配也有释放，内存不够的时候就要进行内存回收。  
    - 内存规整，也就是内存碎片整理。  
    - 页帧( page frame )回收。
        - 内核的物理内存是不换页的，所以内核只会进行缓存回收。
        - 用户空间的物理内存是可以换页的，所以会对用户空间的物理内存进行换页以便回收其物理内存。用户空间的物理内存分为文件页和匿名页。对于文件页，如果其是 clean 的，可以直接丢弃内容，回收其物理内存，如果其是 dirty 的，则会先把其内容写回到文件，然后再回收内存。对于匿名页，如果系统配置的有 swap 区的话，则会把其内容先写入 swap 区，然后再回收，如果系统没有 swap 区的话则不会进行回收。
    
    把进程占用的但是当前并不在使用的物理内存进行回收，并分配给新的进程来使用的过程就叫做换页。进程被换页的物理内存后面如果再被使用到的话，还会通过缺页异常再换入内存。
    -  OOM Killer  
        OOM Killer会按照一定的规则选择一个进程将其杀死，然后其物理内存就被释放了。

4. 内核页表建立好了之后就不会再取消了，所以内核是不换页的，用户页表建立之后可能会因为内存回收而取消，所以用户空间是换页的。  
内核页表是在内核启动时建立的，所以内核空间的映射是线性映射，用户空间的页表是在运行时动态创建的，不可能做到线性映射，所以是随机映射。

5. 进程的虚拟地址空间是由用户空间和内核空间组成，底部 0~3G 为用户空间，顶部 3~4G 为内核空间。
虽然（在两个用户进程之间的）上下文切换期间会改变下半部分，但虚拟地址空间的内核部分总是保持不变。

# 0x02、层级

## 2.1 NODE

UMA 只有一个节点， NUMA 可以有多个节点。
`ls /sys/devices/system/node/node*`

## 2.2 ZONE

物理内存划分了六个 ZONE 。
`cat /proc/zoneinfo | grep zone`

    除了第一个节点能包含所有的区域类型之外，其它的节点并不能包含所有的区域类型，因为有些区域类型( DMA、DMA32 )必须从物理内存的起点开始。

1. ZONE_DMA  
    - 由配置项 CONFIG_ZONE_DMA 决定是否存在。
    - 在 x86 上 DMA 内存区域是物理内存的前 16M ，这是因为早期的 ISA 总线上的 DMA 控制器只有 24 根地址总线，只能访问 16M 物理内存。为了兼容这些老的设备，所以需要专门开辟前 16M 物理内存作为一个区域供这些设备进行DMA操作时去分配物理内存。

2. ZONE_DMA32   
    - 由配置项 CONFIG_ZONE_DMA32 决定是否存在。
    - 后来的 DMA 控制器有 32 根地址总线，可以访问 4G 物理内存了。但是在 32 位的系统上最多只支持 4G 物理内存，所以没必要专门划分一个区域。但是到了 64 位系统时候，很多CPU能支持 48 位到 52 位的物理内存，于是此时就有必要专门开个区域给 32 位的 DMA 控制器使用了。

3. ZONE_NORMAL   
    - 常规内存，无配置项控制，必然存在，除了其它几个内存区域之外的内存都是常规内存 ZONE_NORMAL 。

4. ZONE_HIGHMEM   
    - 高端内存，由配置项 CONFIG_HIGHMEM 决定是否存在。
    - 只在 32 位系统上有，因为内核的虚拟地址空间只有 1GB ，但它需要访问整个 4GB 的物理空间:
        - 从物理地址 0~896MB 的部分（ZONE_DMA+ZONE_NORMAL) ，直接加上 3GB 的偏移（在 Linux 中用 PAGE_OFFSET 表示），就得到了对应的虚拟地址，这种映射方式被称为线性/直接映射 (Direct Map) 。

        - 而 896M~4GB 的物理地址部分 (ZONE_HIGHMEM) 需要映射到 (3G+896M) ~ 4GB 这 128MB 的虚拟地址空间，显然也按线性映射是不行的。
        采用的是做法是，ZONE_HIGHMEM 中的某段物理内存和这 128M 中的某段虚拟空间建立映射，完成所需操作后，需要断开与这部分虚拟空间的映射关系，以便 ZONE_HIGHMEM 中其他的物理内存可以继续往这个区域映射，即动态映射的方式。

5. ZONE_MOVABLE   
    - 可移动内存，无配置项控制，必然存在，用于可热插拔的内存。
    - 内核启动参数 movablecore 用于指定此区域的大小。内核参数 kernelcore 也可用于指定非可移动内存的大小，剩余的内存都是可移动内存。如果两者同时指定的话，则会优先保证非可移动内存的大小至少有 kernelcore 这么大。如果两者都没指定，则可移动内存大小为 0 。

6. ZONE_DEVICE   
    - 设备内存，由配置项 CONFIG_ZONE_DEVICE 决定是否存在，用于放置持久内存(也就是掉电后内容不会消失的内存)。
    - 一般的计算机中没有这种内存，默认的内存分配也不会从这里分配内存。持久内存可用于内核崩溃时保存相关的调试信息。



![1](../../pic/linux/memory/Linux-Memory-X86-32.jpg)

## 2.3 PAGE

物理内存页面也叫做页帧( page frame )。  
物理内存从开始起每 4K 构成一个个页帧，这些页帧的编号依次是 0、1、2、3... 。页帧的编号也叫做 PFN(page frame number)。  
很显然，一个页帧的物理地址和它的 PFN 有一个简单的数学关系，那就是其物理地址除以 4096(4K) 就是其 PFN ，其 PFN 乘以 4096(4K) 就是其物理地址。
```C
// PAGE_SHIFT 是 12
#define PFN_PHYS(x) ((phys_addr_t)(x) << PAGE_SHIFT)
#define PHYS_PFN(x) ((unsigned long)((x) >> PAGE_SHIFT))
```

# 0x03、段( segment )

`0x08048000` 是 `text` 段的起始位置，后面分别跟着 `data` 段、 `bss` 段。

每个 segment 用一个 vm_area_struct (以下简称vma) 结构体表示。 /proc/PID/maps 查看一个进程的所有 vma 在虚拟地址空间的分布情况，其内部实现靠的就是对这个链表的遍历。
```C
struct vm_area_struct 
{ 
    // vma 所指向区域的起始地址和结束地址
	unsigned long vm_start; 
	unsigned long vm_end;  
    // 前后另一个 vma 节点
	struct vm_area_struct *vm_next, *vm_prev; 
	rb_node_t vm_rb; 
    /* 
     * 描述的是vma的属性， flag 可以是 VM_READ 、 VM_WRITE 、 VM_EXEC 、 VM_SHARED ，分别指定 vma 的内容是否可以读、写、执行，或者由几个进程共享。
     * vma 由许多的虚拟 pages 组成，每个虚拟 page 需要经过 page table 的转换才能找到对应的物理页面。这个地方的属性会复制到这个 vma 所含 pages 的 PTE 中。
    */
    unsigned long vm_flags;
    /*
     * 可执行文件和动态链接库的 text 段和 data 段是基于 elf 文件的，
     * mmap 对文件的映射也是对应外部存储介质中这个被映射的文件的，
     * 这两种情况下， vm_file 指向这个被映射的文件，进而可获得该文件的 inode 信息，
     * 而 vm_pgoff 是这个段在该文件内的偏移。

     * 对于 text 段，一般偏移就是 0 。对于 heap 段， stack 段以及 mmap 的匿名映射，
     * 没有与之相对应的文件实体，此时 vm_file 就为 NULL ， vm_pgoff 的值没有意义。
    */
    struct file * vm_file;
    unsigned long vm_pgoff;
	struct mm_struct * vm_mm; 
	...
}
```

# 0x04、内核
    内核也是由一个 elf 文件（比如 vmlinux ）加载启动的，加载后也有 text 段， data 段， bss 段等。

    vmlinux是未经压缩的Linux内核，是编译出来的最原始的文件。

