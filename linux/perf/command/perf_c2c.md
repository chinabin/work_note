# 0x00. 导读

[c2c主要贡献者Joe的博文：c2c-blog](https://joemario.github.io/blog/2016/09/01/c2c-blog/)

perf c2c 的主要功能：

- 发现伪共享的 Cache Line
- 谁在读写上述的 Cache Line，以及访问发生处的 Cache Line 的内部偏移
- 这些读者和写者分别的 pid, tid, 指令地址，函数名，二进制文件
- 每个读者和写者的源代码文件，代码行号
- 这些热点 Cache Line 上的，load 操作的平均延迟
- 这些 Cache Line 的样本来自哪些 NUMA 节点， 由哪些 CPU 参与了读写

使用准则：
1. 先用 perf c2c record 通过采样，收集性能数据  
2. 再用 perf c2c report 基于采样数据，生成报告


Joe 总结了他使用 c2c 优化应用时，学到的东西，

- perf c2c 采样时间不宜过长。Joe 建议运行 perf c2c 3 秒、5 秒或 10 秒。运行更久，观测到的可能就不是并发的伪共享，而是时间错开的 Cache Line 访问。
- 如果对内核样本没有兴趣，只想看用户态的样本，可以指定 --all-user。反之使用 --all-kernel。
CPU 很多的系统上（如 >148 个），设置 -ldlat 为一个较大的值（50 甚至 70），perf 可能能产生更丰富的C2C样本。
- 读最上面那个具有概括性的 Trace Event 表，尤其是 LLC Misses to Remote cache HITM 的数字。只要不是接近 0，就可能有值得追究的伪共享。
- 看 Pareto 表时，需要关注的，多半只是最热的两三个 Cache Line。
- 有的时候，一段代码，它不在某一行 Cache Line 上竞争严重，但是它却在很多 Cache Line 上竞争，这样的代码段也是很值得优化的。同理还有多进程程序访问共享内存时的情况。
- 在 Pareto 表里，如果发现很长的 load 操作平均延迟，常常就表明存在严重的伪共享，影响了性能。
- 接下来去看样本采样自哪些节点和 CPU，据此进行优化，将哪些内存或 Task 进行 NUMA 节点锁存。

最后，Pareto 表还能对怎么解决对齐得很不好的Cache Line，提供灵感。 例如：

- 很容易定位到：写地很频繁的变量，这些变量应该在自己独立的 Cache Line。可以据此进行对齐调整，让他们不那么竞争，运行更快，也能让其它的共享了该 Cache Line 的变量不被拖慢。
- 很容易定位到：没有 Cache Line 对齐的，跨越了多个 Cache Line 的热的 Lock 或 Mutex。
- 很容易定位到：读多写少的变量，可以将这些变量组合到相同或相邻的 Cache Line。

可以用 [false_sharing_example.c](./false_sharing_example.c) 练习一下。

# 0x01. 简介

shared data c2c/HITM 分析。

cache to cache：

缓存的命中率，是 CPU 性能的一个关键性能指标。我们知道，CPU 里面有好几级缓存（Cache），每一级缓存都比后面一级缓存访问速度快。最后一级缓存叫 LLC（Last Level Cache）；LLC 的后面就是内存。

当 CPU 需要访问一块数据或者指令时，它会首先查看最靠近的一级缓存（L1）；如果数据存在，那么就是缓存命中（Cache Hit），否则就是不命中（Cache Miss），需要继续查询下一级缓存。

c2c 用来检测 cache 共享命中失败，一个处理器修改了某个 cache line 中的数据，另一个处理器访问该 cache line 数据时需要 refresh 该 cache line， perf c2c 命令就用来调试这里问题。


HITM：  
Hit In The Modified, 代表 CPU 在 load 操作命中了一条标记为Modified状态的 cacheline. 这就是 false sharing 产生的地方。

Remote HITMs：  
跨 NUMA 节点的 HITM，这个是所有 load 操作里代价最高的情况，尤其在读者和写者非常多的情况下，这个代价会变得非常的高


通常观察一下 `Load LLC Misses`、`Store L1D Miss`、`LLC Misses to Remote cache (HITM)` 是否过高，至于这几个数据多高才算有问题，得根据实际经验来分析，没有硬性指标。尤其是LLC Misses to Remote cache (HITM)的数字。只要不是接近 0，就可能有值得追究的伪共享。

**建议运行 perf c2c 3 秒、5 秒或 10 秒。运行更久，观测到的可能就不是并发的伪共享，而是时间错开的 cacheline 访问。**

# 0x02. 选项和输出

**注意拉开屏幕！！！**，完整输出查看 [perf_c2c_report](./perf_c2c_report)

```bash
$ perf c2c record -F 60000 -- -g -a sleep 5          # 记录数据
$ perf c2c report --full-symbols -g --stdio          # 分析展示数据
```

如果出现 `failed: memory events not supported` 一般是因为 `/sys/devices/cpu/events/` 下面缺少 `mem-loads` 和 `mem-stores` 文件。例如：
```bash
# 行
$ ls /sys/devices/cpu/events/
branch-instructions  bus-cycles    cache-references  cycles-ct  el-abort     el-commit    el-start      mem-loads   ref-cycles  tx-capacity  tx-conflict
branch-misses        cache-misses  cpu-cycles        cycles-t   el-capacity  el-conflict  instructions  mem-stores  tx-abort    tx-commit    tx-start

# 不行
$ ls /sys/devices/cpu/events/
branch-instructions  branch-misses  bus-cycles  cache-misses  cache-references  cpu-cycles  instructions  ref-cycles
```

```
perf c2c --help 多看看

perf c2c record [<options>] <command>
perf c2c record [<options>] -— [<record command options>] <command>

perf c2c report [<options>]
```
RECORD 常用参数
```
-e, events
-l n, --ldlat 只采集 load 延迟大于 n 个（默认是 30 个）机器周期的样本
-k, --all-kernel 配置是否记录内核空间发生的事件
-u, --all-user 配置是否记录用户空间的事件
```

REPORT 常用参数
```
--stdio
-g, --call-graph
--stats 只打印统计信息
-c tid,pid,iaddr,dso, 排序，默认是用 pid,iaddr
```


perf c2c report --stats 输出含义：

```
=================================================
            Trace Event Information
=================================================
  Total records                     :     290513  >> 采样到的 CPU load 和 store 的样本总数
  Locked Load/Store Operations      :      12198  >> LOCK指令下的load/store操作数，Read-Modify-Write操作会使用到LOCK执行，比如atomic_add等
  Load Operations                   :      75398  >> CPU load 操作的样本总数
  Loads - uncacheable               :          0
  Loads - IO                        :          0
  Loads - Miss                      :        282
  Loads - no mapping                :          2
  Load Fill Buffer Hit              :      32203  >> Load 操作没有命中 L1 Cache，但命中了 L1 Cache 的 Fill Buffer 的次数
  Load L1D hit                      :      33827  >> Load 操作命中 L1 Dcache 的次数
  Load L2D hit                      :       3645  >> Load 操作命中 L2 Dcache 的次数
  Load LLC hit                      :       3147  >> Load 操作命中最后一级 (LLC) Cache (通常 LLC 是 L3) 的次数
  Load Local HITM                   :        133  >> Load 操作命中了本地 NUMA 节点的修改过的 Cache 的次数
  Load Remote HITM                  :       1403  >> Load 操作命中了远程 NUMA 节点的修改过的 Cache 的次数
  Load Remote HIT                   :         52  >> Load 操作命中了远程未修改的 Clean Cache 的次数
  Load Local DRAM                   :        483  >> Load 操作命中了本地 NUMA 节点的内存的次数，其实这就是 Cache Miss
  Load Remote DRAM                  :       1809  >> Load 操作命中了远程 NUMA 节点的内存的次数，其实这是比 Load Local DRAM 更严重的 Cache Miss
  Load MESI State Exclusive         :       2240  >> Load 操作命中 MESI 状态中，处于 Exclusive 状态的 Cache 的次数
  Load MESI State Shared            :         52  >> Load 操作命中 MESI 状态中，处于 Shared 状态的 Cache 的次数
  Load LLC Misses                   :       3747  >> Load 操作产生的本地 NUMA 节点 LLC Cache Miss 的次数，是 Load Remote HITM，Load Remote HIT，Load Local DRAM，Load Remote DRAM 之和
  LLC Misses to Local DRAM          :       12.9% >> Load 操作产生的 LLC Cache Miss 中，从本地 NUMA 节点拿到内存的样本占 Load LLC Misses 总样本的百分比
  LLC Misses to Remote DRAM         :       48.3% >> Load 操作产生的 LLC Cache Miss 中，从远程 NUMA 节点拿到内存的样本占 Load LLC Misses 总样本的百分比
  LLC Misses to Remote cache (HIT)  :        1.4% >> Load 操作产生的 LLC Cache Miss 中，从远程 NUMA 节点拿到 Clean Cache 的样本占 Load LLC Misses 总样本的百分比
  LLC Misses to Remote cache (HITM) :       37.4% >> Load 操作产生的 LLC Cache Miss 中，从远程 NUMA 节点拿到被修改过的 Cache 的样本占 Load LLC Misses 总样本的百分比，这是代价最高的伪共享
  Store Operations                  :     215115
  Store - uncacheable               :          0
  Store - no mapping                :          0
  Store L1D Hit                     :     214826  >> Store 操作命中 L1 Dcache 的次数
  Store L1D Miss                    :        289  >> Store 操作命中 L1 Dcache Miss 的次数
  No Page Map Rejects               :        134
  Unable to parse data source       :          0

=================================================
    Global Shared Cache Line Event Information
=================================================
  Total Shared Cache Lines          :        115
  Load HITs on shared lines         :      52541
  Fill Buffer Hits on shared lines  :      31867
  L1D hits on shared lines          :      11791
  L2D hits on shared lines          :       3438
  LLC hits on shared lines          :       2876
  Locked Access on shared lines     :      12181
  Store HITs on shared lines        :      14235
  Store L1D hits on shared lines    :      14144
  Total Merged records              :      15771

=================================================
                 c2c details
=================================================
  Events                            : cpu/mem-loads,ldlat=30/P
                                    : cpu/mem-stores/P
  Cachelines sort on                : Total HITMs
  Cacheline data grouping           : offset,pid,iaddr
```

```
=================================================
           Shared Data Cache Line Table          
=================================================
#
#        ----------- Cacheline ----------    Total      Tot  ----- LLC Load Hitm -----  ---- Store Reference ----  --- Load Dram ----      LLC    Total  ----- Core Load Hit -----  -- LLC Load Hit --
# Index             Address  Node  PA cnt  records     Hitm    Total      Lcl      Rmt    Total    L1Hit   L1Miss       Lcl       Rmt  Ld Miss    Loads       FB       L1       L2       Llc       Rmt
# .....  ..................  ....  ......  .......  .......  .......  .......  .......  .......  .......  .......  ........  ........  .......  .......  .......  .......  .......  ........  ........
#
      0          0x7e443c80     0      15      819   70.87%      803      803        0        0        0        0         0         0        0      819       13        0        0         3         0
      1          0x7e443500     0       1       38    3.35%       38       38        0        0        0        0         0         0        0       38        0        0        0         0         0
      2          0x7e4439c0     0       1       32    2.82%       32       32        0        0        0        0         0         0        0       32        0        0        0         0         0
      3          0x7e443800     0       3       31    2.65%       30       30        0        0        0        0         0         0        0       31        1        0        0         0         0

shared data cache line table (perf c2c report --stdio 查看) 显示了 fasle sharing 发生的地方，它列表是按照 HITMs 数量从高到低排列显示的

Cacheline: cacheline address (hex number), PA cnt 别管，大概是物理地址变化次数之类的
Total records: 所有 cacheline 的访问次数

LLC Load Hitm - Total, Lcl, Rmt: count of Total/Local/Remote load HITMs

Store Reference - Total, L1Hit, L1Miss
             Total - all store accesses
             L1Hit - store accesses that hit L1
             L1Hit - store accesses that missed L1

Load Dram: count of local and remote DRAM accesses

LLC Ld Miss: count of all accesses that missed LLC

Total Loads: sum of all load accesses

Core Load Hit - FB, L1, L2
           - count of load hits in FB (Fill Buffer), L1 and L2 cache
```

```

=================================================
      Shared Cache Line Distribution Pareto      
=================================================
#
#        ----- HITM -----  -- Store Refs --  --------- Data address ---------                               ---------- cycles ----------    Total       cpu                                                      Shared                                      
#   Num      Rmt      Lcl   L1 Hit  L1 Miss              Offset  Node  PA cnt      Pid        Code address  rmt hitm  lcl hitm      load  records       cnt                          Symbol                      Object                     Source:Line  Node
# .....  .......  .......  .......  .......  ..................  ....  ......  .......  ..................  ........  ........  ........  .......  ........  ..............................  ..........................  ..............................  ....
#
  -------------------------------------------------------------
      0        0      803        0        0          0x7e443c80
  -------------------------------------------------------------
           0.00%  100.00%    0.00%    0.00%                 0x8     0       1   404003            0x5f3e46         0       226       214      812         1  [.] AegisSmartAggAppendOrderMi  AegisV.2024-09-18_09.42.48  AegisSmartAggAppendOrderMissil   0
            |
            ---AegisSmartAggAppendOrderMissile<0>::handle_idle
               aegis::AegisMissileReactor::one_loop
               aegis::AegisMissileReactor::Run
               execute_native_thread_routine
               start_thread
               __clone


  -------------------------------------------------------------
      1        0       38        0        0          0x7e443500
  -------------------------------------------------------------
           0.00%  100.00%    0.00%    0.00%                0x28     0       1   404003            0x5f3878         0       182         0       38         1  [.] AegisSmartAggAppendOrderMi  AegisV.2024-09-18_09.42.48  AegisSmartAggAppendOrderMissil   0
            |
            ---AegisSmartAggAppendOrderMissile<0>::handle_idle
               aegis::AegisMissileReactor::one_loop
               aegis::AegisMissileReactor::Run
               execute_native_thread_routine
               start_thread
               __clone


  -------------------------------------------------------------
      2        0       32        0        0          0x7e4439c0
  -------------------------------------------------------------
           0.00%  100.00%    0.00%    0.00%                 0x0     0       1   404003            0x56eb40         0       198         0       32         1  [.] RollingMean<double>::mean  AegisV.2024-09-18_09.42.48  RollingMean<double>::mean+0   0
            |
            ---RollingMean<double>::mean
               AegisSmartAggAppendOrderMissile<0>::handle_idle
               aegis::AegisMissileReactor::one_loop
               aegis::AegisMissileReactor::Run
               execute_native_thread_routine
               start_thread
               __clone


  -------------------------------------------------------------
      3        0       30        0        0          0x7e443800
  -------------------------------------------------------------
           0.00%  100.00%    0.00%    0.00%                 0x8     0       1   404003            0x50cced         0       217         0       30         1  [.] aegis::AegisMissileReactor  AegisV.2024-09-18_09.42.48  aegis::AegisMissileReactor::on   0
            |
            ---aegis::AegisMissileReactor::one_loop
               aegis::AegisMissileReactor::Run
               execute_native_thread_routine
               start_thread
               __clone


Shared Cache Line Distribution Pareto (perf c2c report --stdio 查看) 是最重要的表，它展示了每个冲突竞争的 cacheline 的详细信息。

HITM - Rmt, Lcl
    - % of Remote/Local HITM accesses for given offset within cacheline
    在这个 cacheline 中，Remote/Local HITM 的百分比

Store Refs - L1 Hit, L1 Miss
    - % of store accesses that hit/missed L1 for given offset within cacheline
    在这个 cacheline 中，store hit/missed 的百分比

Data address - Offset
    - 上面是地址，下面是偏移量

Pid Tid Code address
    - 访问对应的 process id 、thread id、 code address

cycles - rmt hitm, lcl hitm, load
    - sum of cycles for given accesses - Remote/Local HITM and generic load
    周期数

cpu cnt
    - number of cpus that participated on the access

```
