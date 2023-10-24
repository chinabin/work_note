# 0x00. 导读

HighMem 映射。

# 0x01. 简介

# 0x02. HighMem 映射

[高端内存页框的内存映射简介](https://freeflyingsheep.github.io/posts/kernel/memory/map/)

对应高端内存的3部分，高端内存映射有三种方式：
- 映射到”内核动态映射空间”（noncontiguous memory allocation）（VMALLOC_START - VMALLOC_END）  
    这种方式很简单，因为通过 vmalloc() ，在”内核动态映射空间”申请内存的时候，就可能从高端内存获得页面（参看 vmalloc 的实现），因此说高端内存有可能映射到”内核动态映射空间”中。  
    使用 vmap() 先在 VMALLOC 区域获取 size 大小的虚拟地址，然后通过 map_kernel_range 函数把虚拟地址和物理地址绑定映射，即是填充 PGD 和 PTE 页表

- 持久内核映射（permanent kernel mapping）  
    通过 kmap()，可以把一个 page 映射到这个空间来。由于这个空间是 4M 大小，最多能同时映射 1024 个 page。因此，对于不使用的的 page，及应该时从这个空间释放掉（也就是解除映射关系），通过 kunmap() ，可以把一个 page 对应的线性地址从这个空间释放出来。

- 临时映射（temporary kernel mapping）  
    内核在 FIXADDR_START 到 FIXADDR_TOP 之间保留了一些线性空间用于特殊需求。这个空间称为”固定映射空间”在这个空间中，有一部分用于高端内存的临时映射。

    这块空间具有如下特点：  
    （1）每个 CPU 占用一块空间  
    （2）在每个 CPU 占用的那块空间中，又分为多个小空间，每个小空间大小是 1 个 page，每个小空间用于一个目的，这些目的定义在 kmap_types.h 中的 km_type 中。

    当要进行一次临时映射的时候，需要指定映射的目的，根据映射目的，可以找到对应的小空间，然后把这个空间的地址作为映射地址。这意味着一次临时映射会导致以前的映射被覆盖。通过 kmap_atomic() 可实现临时映射。

```c
/*
 * Ordering is:
 *
 * FIXADDR_TOP
 *                      fixed_addresses
 * FIXADDR_START
 *                      temp fixed addresses
 * FIXADDR_BOOT_START
 *                      Persistent kmap area
 * PKMAP_BASE
 * VMALLOC_END
 *                      Vmalloc area
 * VMALLOC_START
 * high_memory
 */

```

```c
// 把给定 page 结构映射到内核地址空间；
// 这个函数可以睡眠，因此 kmap() 只能用在进程上下文中。
void *kmap(struct page *page);
// 非高端内存：本来就是一一映射的，所以会直接返回对应的虚拟地址
// 高端内存：会将该 page 映射到专用的永久映射区，返回虚拟地址。（永久映射区是有限的，不可能映射所有高端内存）

// 永久映射的数量有限，应通过 kunmap 及时解除映射
// 只对高端内存有效，非高端内存总是一一映射的，无法取消。
void kunmap(struct page *page)


// 临时映射
// 就如其名字 atomic 一样，它是原子性的，不会阻塞，不能被内核抢占(关内核抢占，不关中断)，可以用于中断上下文
void *kmap_atomic(struct page *page)

// 解除临时映射
// 不会阻塞。因为临时映射会自动覆盖，即使不取消映射也没什么关系。
void __kunmap_atomic(void *kvaddr);
```