# 0x00. 导读

主要参考：

知乎专栏 [高速缓存与一致性专栏](https://zhuanlan.zhihu.com/p/136300660) 。

[CPU-Cache-and-Memory-Ordering.pdf](http://www.valleytalk.org/wp-content/uploads/2013/07/CPU-Cache-and-Memory-Ordering.pdf)

[CPU_cache](https://en.wikipedia.org/wiki/CPU_cache)

[Cache wiki](https://en.wikipedia.org/wiki/Cache_(computing))

[memory-ordering](http://gavinchou.github.io/summary/c++/memory-ordering)

# 0x01. 简介

[cache01](./cache01.md): cache 简介 + cache placement: 直接映射、组相连、全相连  
[cache02](./cache01.md): cache 的更新和替换策略（cache hit 和 cache miss 的 store 指令如何执行，cache 满了该如何腾出位置）  
[cache07](./cache07.md): 源自 超标量处理器设计 2.2 章节，讲的是如何提高 Cache 性能。  
[cache03](./cache03.md): Cache 的组织方式  
[cache04](./cache04.md): 多核之间的 Cache 如何保持一致性  
[cache05](./cache05.md): 编程时候因为 cache 导致的性能问题   
[cache06](./cache06.md): store buffer, Store Forwarding, Invalid Queue, 读写屏障，内存模型   

# 0x02. Cache 四要素

缓存器设计中有四个重要的特性，确定了这四个特性，一个基本的缓存器也就慢慢成型了:

1. 如何在缓存器里放置数据？ (Cache Placement) ，[cache01](./cache01.md)  
    - Direct Mapped 直接映射
    - Fully Associative 全关联
    - Set-Associative 组关联

2. 如何在缓存器里搜寻数据？ (Cache Searching) ，[cache01](./cache01.md)  
    Tag 标签，Address 地址

3. 如何在缓存器里替换数据？ (Cache Replacement)   
    - Random 随机替换
    - Least Recent Used (LRU) 替换最不常用
    - FIFO (First-In First-Out)。

4. 如何通过缓存器向主存写数据？ (Cache Write Policy) ，[cache02](./cache02.md)  
    - Write Through 写穿
    - Write Back 写回
    - Write Allocate 写分配
    - No-Write Allocate 写不分配。  

    展开讲的话得先分写命中和写不命中：
    - 当发生写命中时 (Write Hit)   
        当向缓存器发出写入指令 (Store) ，并且要写的数据恰好在缓存器中，面临着两种选择：

        - 同时更新缓存器和主存。这样做的好处，设计起来很简单，并且，可以保持主存数据是最新的，这一点可以在缓存一致性 (Cache Coherency)  里很重要。这种方法称为 写穿 (Write Through) 。

        - 只更新缓存器，但在这个数据块要被替换的时候，才写回主存。这样做的好处，可以减少从 缓存器写数据进主存的频率，并且可以减少使用主存的带宽，这一点在多核处理器 (Multicore Processor) ，很受欢迎。这样的方法称为 写回 (Write Back) 。

    - 当发生写不命中时 (Write Miss) 

        当向缓存器发出写入指令 (Store) ，并且要写的数据不在缓存器中，面临着两种选择：

        - 写进主存，并把写的数据存放在缓存器中，这样，下次就有几率命中这个数据。这样的方法称为 写分配 (Write Allocate) 。

        - 写进主存，但不把这个数据写进缓存器。这种方法称为 写不分配 (No-Write Allocate) 。

    一般缓存的搭配是这样的：
    - 写穿 & 写不分配 (Write Through & No-Write Allocate) 
    - 写回 & 写分配  (Write Back & Write Allocate) 

# 0x03. perf 检测