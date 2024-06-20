# 0x00. 导读

# 0x01. 简介

shared data c2c/HITM 分析。

cache to cache：

缓存的命中率，是 CPU 性能的一个关键性能指标。我们知道，CPU 里面有好几级缓存（Cache），每一级缓存都比后面一级缓存访问速度快。最后一级缓存叫 LLC（Last Level Cache）；LLC 的后面就是内存。

当 CPU 需要访问一块数据或者指令时，它会首先查看最靠近的一级缓存（L1）；如果数据存在，那么就是缓存命中（Cache Hit），否则就是不命中（Cache Miss），需要继续查询下一级缓存。

c2c 用来检测 cache 共享命中失败，一个处理器修改了某个 cache line 中的数据，另一个处理器访问该 cache line 数据时需要 refresh 该 cache line， perf c2c 命令就用来调试这里问题。


HITM：  
Notice the term “HITM”, which stands for a load that hit in a modified cacheline. 这就是 false sharing 产生的地方。

Remote HITMs：  
meaning across numa nodes, are the most expensive - especially when there are lots of readers and writers.

## 1.1 

perf c2c report 输出含义：

```
 1  =================================================
 2              Trace Event Information
 3  =================================================
 4    Total records                     :     329219  << Total loads and stores sampled.
 5    Locked Load/Store Operations      :      14654
 6    Load Operations                   :      69679  << Total loads
 7    Loads - uncacheable               :          0
 8    Loads - IO                        :          0
 9    Loads - Miss                      :       3972
10    Loads - no mapping                :          0
11    Load Fill Buffer Hit              :      11958
12    Load L1D hit                      :      17235  << loads that hit in the L1 cache.
13    Load L2D hit                      :         21
14    Load LLC hit                      :      14219  << loads that hit in the last level cache (LLC).
15    Load Local HITM                   :       3402  << loads that hit in a modified cache on the same numa node (local HITM).
16    Load Remote HITM                  :      12757  << loads that hit in a modified cache on a remote numa node (remote HITM).
17    Load Remote HIT                   :       5295
18    Load Local DRAM                   :        976  << loads that hit in the local node's main memory.
19    Load Remote DRAM                  :       3246  << loads that hit in a remote node's main memory.
20    Load MESI State Exclusive         :       4222 
21    Load MESI State Shared            :          0
22    Load LLC Misses                   :      22274  << loads not found in any local node caches.
23    LLC Misses to Local DRAM          :        4.4% << % hitting in local node's main memory.
24    LLC Misses to Remote DRAM         :       14.6% << % hitting in a remote node's main memory.
25    LLC Misses to Remote cache (HIT)  :       23.8% << % hitting in a clean cache in a remote node.
26    LLC Misses to Remote cache (HITM) :       57.3% << % hitting in remote modified cache. (most expensive - false sharing)
27    Store Operations                  :     259539  << store instruction sample count
28    Store - uncacheable               :          0
29    Store - no mapping                :         11
30    Store L1D Hit                     :     256696  << stores that got L1 cache when requested.
31    Store L1D Miss                    :       2832  << stores that couldn't get the L1 cache when requested (L1 miss).
32    No Page Map Rejects               :       2376
33    Unable to parse data source       :          1
```

# 0x02.