# 0x00. 导读

介绍 Slab Allocator.

# 0x01. 简介

# 0x02. Slab Allocator

Linux 中的 buddy 分配器是以 page frame 为最小粒度的，而现实的应用多是以内核 objects （比如描述文件的 struct inode ）的大小来申请和释放内存的，这些内核 objects 的大小通常从几十字节到几百字节不等，远远小于一个 page 的大小。

那可不可以把一个 page frame 再按照 buddy 的原理，以更小的尺寸（比如 128 字节，256 字节）组织起来，形成一个二级分配系统呢？这就是 slab 分配器。

一个 Slab 包含一个或多个连续的物理页面（一般是一个），然后将这些页面均分成固定的大小的各等份，每一等份就是一个对象，各对象通过链表串起来。

1. slab 机制的核心思想是以对象的观点来管理内存，slab 也是内核中一种内存分配器，slab 分配器基于对象进行管理的，所谓的对象就是内核中的数据结构(例如：task_struct,file_struct 等)。相同类型的对象归为一类，每当要申请这样一个对象时，slab 分配器就从一个 slab 列表中分配一个这样大小的单元出去，而当要释放时，将其重新保存在该列表中，而不是直接返回给伙伴系统，从而避免内部碎片。

2. 可以通过 `slabinfo` 或者 `slabtop`(以实时的方式显示 slab 信息。) 或者 `/proc/slabinfo` 查看。

## 2.1 kmalloc

与 libc 提供的内存申请 API (malloc 和 free) 类似，Slab 分配器提供的 API 为 kmalloc()和 kfree()。

1. 内存中还有一些偶发的零碎的内存分配需求，一个模块如果仅仅为了分配一次 5 字节的内存，就去创建一个 slab ，那显然不划算。  
为此内核创建了一个统一的零碎内存分配器 kmalloc ，用户可以直接请求 kmalloc 分配若干个字节的内存。  
Kmalloc 底层用的还是 slab 机制， kmalloc 在启动的时候会预先创建一些不同大小的 slab ，用户请求分配任意大小的内存， kmalloc 都会去大小刚刚满足的 slab 中去分配内存。

2. 特点：内核空间使用，物理地址连续。

可以使用 `sudo cat /proc/slabinfo | grep -i kmalloc`

### 2.1.1 kzalloc

`kzalloc()` 等价于先用 `kmalloc()` 申请空间， 再用 `memset()` 来初始化，所有申请的元素都被初始化为 0 。