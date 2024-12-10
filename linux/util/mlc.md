# 0x00. 导读

TODO: 
mlc 原理
mlc 选项

# 0x01. 简介

[下载地址](https://www.intel.com/content/www/us/en/download/736633/736634/intel-memory-latency-checker-intel-mlc.html)

[文档](https://www.intel.com/content/www/us/en/developer/articles/tool/intelr-memory-latency-checker.html)

影响程序性能的两个重要因素：

- 应用程序从处理器缓存和从内存子系统获取数据所消耗的时间，其中存在各种延迟；
- 带宽 b/w, bandwidth. 带宽反映了单位时间的传输速率，马路越宽，越不会堵车。

Intel Memory Latency Checker(Intel MLC) 是一个测试内存延迟和带宽的工具，并且可以测试延迟和带宽随着系统负载增加的变化。这个工具也提供了一些选项用于更好细粒度调查从特定处理器核心到缓存或内存的一系列选项的测试。

## 1.1 原理和前提

- prefetch

    在 Linux 环境中，使用 MLC 需要 root 权限，因为这个工具修改 H/W prefetch control MSR （硬件预取控制寄存器）来 激活/禁用 预取器 用于延迟和带宽测试。因为在新型的 Intel 处理器上精确测试内存延迟是非常困难的，因为它有复杂的硬件预取器。Intel MLC 在测试延迟时会自动禁用这些预取器，并且在测试完成后自动恢复预期器原状态。预期器控制是通过修改 MSR 实现。

- MSR

    系统需要加载 MSR(model-specific registers) 驱动（不在安装包中），通常可以使用 modprobe msr 命令完成。在 RHEL/CentOS 7 系统中，无需使用 modprobe msr ，因为内核编译时已经 built-in 了MSR支持。通过以下命令可以检查使用内核是否支持 MSR
    ```
    grep -i msr /boot/config-`uname -r`
    ```

- 负载生成线程与延迟线程

    Intel MLC 的一个主要功能是测量带宽需求增加时的延迟。为了加快实现，MLC创建了多个线程，线程数等于主机逻辑处理器数量减 1。这些线程用户生成负载（以下，这些线程被引用为负载生成线程或者带宽生成线程）。这个负载生成线程的主要功能是尽可能生成更多的内存引用。此时系统负载类似，剩下的一个CPU（也就是没有用于产生负载的CPU）运行了一个用于测量延迟的线程。这个线程通常运行在cpu#0，被称为延迟线程（latency thread）和分发依赖的读。

    默认情况下，每个负载生成线程会pin在一个逻辑cpu上。例如，在激活了超线程的10核系统上，MLC创建18个负载生成线程并保留物理核0来运行延迟线程。每个负载生成线程可以配置成生成对缓存层级（cache hierarchy）生成不同程度的读和写。每个线程分配一个buffer用于读并且一个独立的buffer用于写（任何线程之间没有共享数据）。通过相应的不同大小缓存，可以确保引用满足任何缓存级别或者由内存提供服务。

    有一些选项可以控制负载生成线程数量，每个线程使用的缓存大小，在哪里分配它们的内存，读写的比例以及顺序存取或随机存取。

# 0x02. 命令

```bash
$ sudo ./mlc --help
```

## 2.1 

当Intel MLC没有使用任何参数来运行时，它会自动检测系统拓扑并按照以下步骤测试：

```bash
$ sudo ./mlc 
# 1. 两个 Numa node 之间的空闲内存访问 延迟矩阵，以 ns 为单位
# 对角线的数值应该大致相同
# 可以用 ./mlc --latency_matrix 单独执行 内存访问延迟矩阵
Intel(R) Memory Latency Checker - v3.10
Measuring idle latencies for sequential access (in ns)...
                Numa node
Numa node            0       1
       0          69.6   141.4
       1         139.4    70.7

# 2. 不同的读写比对应的内存带宽的值
# 一般来说，内存的写速度是略慢于读速度的。随着写比例的上升，如果带宽急剧下降，那么有可能出现了问题。
# mlc --peak_injection_bandwidth
Measuring Peak Injection Memory Bandwidths for the system
Bandwidths are in MB/sec (1 MB/sec = 1,000,000 Bytes/sec)
Using all the threads from each core if Hyper-threading is enabled     
Using traffic with the following read-write ratios
ALL Reads        :      163473.8
3:1 Reads-Writes :      152508.1
2:1 Reads-Writes :      149002.1
1:1 Reads-Writes :      139576.5           
Stream-triad like:      141400.2        

# 3. 内存访问带宽矩阵。和延迟矩阵类似
# 表示从一个 NUMA 节点访问另一个 NUMA 节点时的内存带宽（以 MB/sec 为单位）
# 问题分析：如果副对角线数值相差过大，表明两个 node 相互访问的带宽差距较大
# 解决方法：出现不平衡的时候一般从内存插法、内存是否故障以及 numa 平衡等角度进行排查
# ./mlc --bandwidth_matrix
Measuring Memory Bandwidths between nodes within system 
Bandwidths are in MB/sec (1 MB/sec = 1,000,000 Bytes/sec)
Using all the threads from each core if Hyper-threading is enabled
Using Read-only traffic type
                Numa node          
Numa node            0       1
       0        82329.6 33676.7
       1        33543.9 80647.3

# 4. 不同带宽情况下的延迟
# mlc --loaded_latency            
# Inject Delay (ns)：注入延迟，以纳秒（ns）为单位。这代表了人为引入的延迟，以模拟不同负载条件下的系统性能。
# Latency (ns)：测量的延迟，以纳秒（ns）为单位。
# Bandwidth (MB/sec)：测量的内存带宽，以 MB/秒为单位。

# 第一行是 无注入延迟 (Inject Delay 00000)                                

Measuring Loaded Latencies for the system
Using all the threads from each core if Hyper-threading is enabled
Using Read-only traffic type    
Inject  Latency Bandwidth     
Delay   (ns)    MB/sec     
==========================
 00000  448.15   162864.7
 00002  259.96   163163.8
 00008  404.76   162521.4
 00015  320.26   161416.5
 00050  199.32   156131.6
 00100  302.06   143492.7
 00200  128.80   110359.9
 00300  117.47    83460.5
 00400  184.74    65107.1
 00500   93.19    53571.3             
 00700  123.30    39564.9          
 01000  100.73    28309.8
 01300   85.89    22095.6                     
 01700   91.33    16989.9                   
 02500  123.41    11886.4              
 03500  123.86     8661.7                                
 05000  116.23     6246.3
 09000  109.07     3758.0     
 20000   80.18     2201.6    

# 随着负载增加，延迟逐渐降低。这可能是因为高负载下，内存访问变得更频繁，缓存和其他优化机制开始生效，减少了访问内存的平均延迟。
# 带宽随着负载增加而显著下降，表明系统在高负载下无法维持高效的内存带宽。
 

# 5. 处理器缓存间的延迟
# mlc --c2c_latency
# HITM 意为 Hit In The Modified ，代表 CPU 在 load 操作命中了一条标记为 Modified 状态的 Cache Line。伪共享发生的关键就在于此。一个处理器修改了某个 cache line 中的数据，另一个处理器访问该 cache line 数据时需要 refresh 该 cache line.
# Modified; 表明 cache line 对应的 memory 仅在一个 Cache 中被缓存了,而且其在 Cache 中 的缓存与在内存中的值也是不一致的
# 而 Remote HITM ，意思是跨 NUMA 节点的 HITM，这个是所有 load 操作里代价最高的情况，尤其在读者和写者非常多的情况下，这个代价会变得非常的高。
# Local HITM ，则是本地 NUMA 节点内的 HITM
Measuring cache-to-cache transfer latency (in ns)...      
# 测量 Hit （命中干净的行）的延迟。
Local Socket L2->L2 HIT  latency        58.3             
# 测量 HitM （命中修改状态的行）的延迟。
Local Socket L2->L2 HITM latency        52.6     
# 当数据地址驻留在写入者 Socket 时
# 当写入者和读取者都在 NUMA 节点 0 或者 1 时，不测量该延迟（用 "-" 表示）。
# 当写入者在 NUMA 节点 0，读取者在 NUMA 节点 1 时，延迟为 116.0 ns。
# 当写入者在 NUMA 节点 1，读取者在 NUMA 节点 0 时，延迟为 116.1 ns。        
Remote Socket L2->L2 HITM latency (data address homed in writer socket)
                        Reader Numa Node          
Writer Numa Node     0       1  
            0        -   116.0  
            1    116.1       -  
# 数据地址驻留在读取者 Socket
Remote Socket L2->L2 HITM latency (data address homed in reader socket)
                        Reader Numa Node                  
Writer Numa Node     0       1                           
            0        -   116.0                           
            1    116.1       -                                                                                       
```

## 2.2

`./mlc --help`

```
通用选项：
-r 随机访问，以获得延迟数据。
-e MLC在所有测量中都不会修改硬件预取器。
-X 每个 core 只用一个线程
```


```
-r 参数表示 MLC 会进行随机访问。顺序读写会提高硬件预取器的命中率，所以也会相应地降低访问延时。这里要注意，当采用 -r 参数的时候，stride size 也就是步进也会更改为 128byte，以避免预取器常用的下一行预取的行为。
```

```
-e 参数表示 MLC 在测试时不会修改硬件预取器的相关设定。因为硬件预取对于 cache 和内存的性能有较大的影响，因此 MLC 在 root 权限下运行时，会默认关闭硬件预取器。

因此 - e 表示不修改相关设定，表示 MLC 不会按照默认的方式关闭硬件预取。如果用户也没有修改 BIOS 选项的话，那么对于大多数人来说，-e 表示硬件预取器是打开的状态。

所以这里相关的建议就是：

- 如果你要测试 cache，内存的准确数据，不要使用 - e，请在 root 权限下，由 MLC 关闭硬件预取进行测试；
- 如果你要测试不同数据大小情况下的实际性能，请使用 - e，部分数据会受益于硬件预取的性能提升；
```

```
非通用选项，需要搭配前缀使用，例如：
./mlc --latency_matrix [-a] [-bn] [-Dn] [-e] [-ln] [-L|-h] [-tn] [-r] [-tn] [-xn] [-X]

-a 测试所有可用 CPU 的 idle 延迟

-b 设置每个 CPU 的分配缓存大小（KB），default=100000. 通过选取适当的 buffer size，可以测试 CPU 不同层级的存储延时和带宽。比如选取 100KB 的 buffer size，这个 buffer 的大小可以放进 L2 中，因此就可以测试出 L2 的带宽和延时。

-c 将 **延迟测试的线程** pin 到指定 CPU 。所有内存访问都将从这个特定的CPU发出

-d delay cycles injected between requests to memory (default-0), higher value lowers bandwidth

-i initialize memory from core #n (determines where requested memory resides)

-D 为了减少 TLB 的 miss 情况，随机访问并不是在整个 buffer 大小上进行的，而是把 buffer 又切分成多个 block，-D 参数就是决定了这个 block 的大小，-D 的数字表示 128byte 的倍数。随机访问首先在 block 的范围内随机读取，读完之后再去下一个 block 进行同样的操作。

-l 参数用于指定步进大小（stride size），默认为 64byte。
```