# 0x00、导读

1. 虚拟地址空间逻辑上划分很多段，每个段内部又会进行功能细分。

# 0x01、简介

    所谓进程地址空间 (process address space ), 就是从进程的视角看到的地址空间，是进程运行时所用到的地址的集合。

    进程的虚拟地址空间是由用户空间和内核空间组成，底部 0~3G 为用户空间，顶部 3~4G 为内核空间。
    虽然（在两个用户进程之间的）上下文切换期间会改变下半部分，但虚拟地址空间的内核部分总是保持不变。

# 0x02、空间分布与映射

![1](../../pic/linux/memory/Linux-Memory-X86-32.jpg)


    1. 32位系统用户进程最大可以访问3GB，内核代码可以访问所有物理内存。

    2. x86架构中物理地址空间划分三部分：ZONE_DMA、ZONE_NORMAL和ZONE_HIGHMEM。
        ZONE_DMA            内存开始的16MB(0x100 0000)
        ZONE_NORMAL         16MB~896MB(0x3800 0000)
        ZONE_HIGHMEM        896MB ~ 4G。

    3. 因为内核的虚拟地址空间只有 1GB ，但它需要访问整个 4GB 的物理空间:
        - 从物理地址 0~896MB 的部分（ZONE_DMA+ZONE_NORMAL) ，直接加上 3GB 的偏移（在 Linux 中用 PAGE_OFFSET 表示），就得到了对应的虚拟地址，这种映射方式被称为线性/直接映射 (Direct Map) 。

        - 而 896M~4GB 的物理地址部分 (ZONE_HIGHMEM) 需要映射到 (3G+896M)~4GB 这 128MB 的虚拟地址空间，显然也按线性映射是不行的。
        采用的是做法是，ZONE_HIGHMEM 中的某段物理内存和这 128M 中的某段虚拟空间建立映射，完成所需操作后，需要断开与这部分虚拟空间的映射关系，以便 ZONE_HIGHMEM 中其他的物理内存可以继续往这个区域映射，即动态映射的方式。

    4. 在64位系统中，内核空间的映射变的简单了，因为这时内核的虚拟地址空间已经足够大了，即便它要访问所有的物理内存，直接映射就是，不再需要 ZONE_HIGHMEM 那种动态映射机制了。

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

