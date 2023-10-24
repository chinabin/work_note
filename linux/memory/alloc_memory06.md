# 0x00. 导读

kmalloc and vmalloc

# 0x01. 简介

# 0x02. 函数

## 2.1 kmalloc

1. kmalloc 最多只能分配 32 个 page 大小的内存，每个 page=4k ，也就是 128K 大小，其中 16 个字节用来记录页描述结构。
2. kmalloc 分配的是常驻内存，不会被交换到文件中。最小分配单位是 32 或 64 字节。
3. kmalloc 可能使用 kmem_cache_create.
4. kmalloc 分配的内核区域是 LowMem, 物理内存区域是 ZONE_NORMAL.
5. kmalloc() 返回的是虚拟地址，但是分到的内存在物理地址上是连续的（因为是直接映射，在虚拟地址上自然也是连续的）。

    ```c
    // 该函数返回的是一个指向内存块的指针，其内存块大小至少为 size,所分配的内存在物理内存中连续且保持原有的数据(不清零)，虚拟内存也连续
    // 如果申请的空间比较小，那么 kmalloc 将使用 slab 分配器，否则将通过 alloc_pages() 直接使用 buddy 分配器。对于 slab 分配器，划分的界限是 2 个 pages 大小。
    void * kmalloc(size_t size, gfp_t flags)
    ```

    ```c
    // 等价于先用 kmalloc() 申请空间， 再用memset()来初始化，所有申请的元素都被初始化为0。
    kzalloc()
    ```

## 2.2 vmalloc

[非连续页框的管理](https://freeflyingsheep.github.io/posts/kernel/memory/uncontinuous/)

[vmalloc 代码](https://www.cnblogs.com/chaozhu/p/10191575.html)

1. vmalloc 分配的内核区域是 HighMem 中的 VMALLOC 区域，所以大小限制就是 VMALLOC 区域的大小，物理内存区域是 ZONE_HIGH.
2. 在 vmolloc area 中使用 vmalloc() 分配内存，具体的分配过程是：
    - 根据要分配的内存大小，调用 get_vm_area(), 获取 vmlist_lock 锁以扫描 vmlist 链表，在 vmolloc area 中找到一块大小满足要求的空闲内存；
    - 调用 __vmalloc_area_pages() --> alloc_page(), 通过内核的 buddy 系统获得相应大小的物理页面；
    - vmalloc area 中的地址映射不再是简单的 3GB 偏移，因此需要调用 map_vm_area(), 建立虚拟地址和物理页面的映射关系，并添加到内核页表中。vmap() 函数。

    总结这个过程就是，获得虚拟地址，申请物理内存，建立连接。
3. vmalloc() 分配的内存只能保证在虚拟地址上连续，不能保证在物理地址上连续。

## 2.3 kmalloc vs vmalloc   
1. 一般只有硬件驱动才会要求获得的内存是物理连续的，因为它们直接访问物理地址，甚至不知道什么是虚拟地址。

2. 内核一般也不要求获取的内存是物理连续的，因为内核可以使用虚拟地址，但是使用 kmalloc 可以避免使用页表，让物理内存页和虚拟地址直接映射(物理连续即逻辑连续，无需页表保证)，减少性能消耗。所以大部分内核代码使用 kmalloc 而非 vmalloc，除非需要分配比较大的内存空间，这种情况找到连续的大块的物理内存会相对困难。

3. vmalloc 必须配合页表使用，将离散的物理页通过页表映射到连续的虚拟地址上，使得逻辑连续。因为需要为每个页简历一个映射关系，会导致TLB更容易抖动，从而降低性能。

4. 两个函数都可能休眠，不能放在中断上下文或其他不能阻塞的地方。