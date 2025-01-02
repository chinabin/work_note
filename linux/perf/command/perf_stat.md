# 0x00. 导读

# 0x01. 简介

用于收集和显示 **性能计数器** 统计信息。它可以针对整个系统或特定进程收集数据，并显示事件的发生次数、占比等信息。

它在内核上下文中统计软件事件，在使用 PMC 寄存器统计硬件事件。统计的事件可以包括软件事件、硬件事件、kprobe、uprobe（两个动态事件，例如可以使用 perf probe --add do_nanosleep 来跟踪 do_nanosleep 调用。）和 USDT 指针（用户级别的静态定义跟踪，相当于用户空间的 tracepoint ）。

```bash
# 运行 ls 命令的时候发生的事情
$perf stat ls
Performance counter stats for 'ls':

              3.46 msec task-clock                #    0.536 CPUs utilized
               441      context-switches          #    0.128 M/sec
                 0      cpu-migrations            #    0.000 K/sec
               320      page-faults               #    0.093 M/sec
        10,964,840      cycles                    #    3.170 GHz
        14,976,365      instructions              #    1.37  insn per cycle
         2,782,792      branches                  #  804.551 M/sec
            64,805      branch-misses             #    2.33% of all branches

       0.006456243 seconds time elapsed

       0.000852000 seconds user
       0.003408000 seconds sys
```

1. task-clock (msec): cpu 处理 task 所消耗的时间，单位 ms.  
0.808 CPUs utilized 表示 cpu 使用率为 80.8% = 2.164836 / 2.679758.  
该值越高代表程序是 CPU bound 而非 IO bound 类型。

2. instructions: 执行的指令条数。  
insns per cycle: 即 IPC ，每个 cpu 周期执行的指令条数，1.11 = 6100570(instructions) / 5506056(cycles)。现代 CPU 一个 cycle 通常能完成多个指令，如果低于 1 肯定不正常。  
**IPC 比上面的 CPU 使用率更能说明 CPU 的使用情况**，关于 IPC 有一篇 [brendangregg的文章](https://www.brendangregg.com/blog/2017-05-09/cpu-utilization-is-wrong.html)，很好的说明 CPU 使用率不是一个很好的性能分析指标。

3. cpu-migrations: 程序在运行过程中发生的CPU迁移次数，即被调度器从一个 CPU 转移到另外一个 CPU 上运行。

# 0x02. 命令

perf stat [options] [command]
 
-e 或 --event: 指定要收集的事件类型，例如：cache-misses, instructions 等。  
-p 或 --pid: 指定要监视的进程ID。  
-I: 指定 ms 为单位的时间间隔
-t 或 --tid: 指定要监视的线程ID。  
-a 或 --all-cpus: 监视所有CPU，而不仅仅是当前CPU。  
-C 或 --cpu: 指定要监视的CPU列表。  
-r 或 --repeat: 重复运行给定的命令并收集统计信息，指定重复次数。  
-d 或 --detailed: 显示详细的统计信息。
-o 或 --output: 指定将数据写入的文件。  
-A 或 --no-aggr: 不进行聚合统计，为每个硬件事件单独显示结果。

## 2.1 -e 或 --event

```
$ perf stat -e cpu-cycles,cache-references,page-faults ls
 Performance counter stats for 'ls':

         4,872,270      cpu-cycles:u
            11,184      cache-references:u
               309      page-faults:u

       0.003673843 seconds time elapsed

       0.002952000 seconds user
       0.000000000 seconds sys
```

## 2.2 -p 或 --pid, -t 或 --tid

```
$ perf stat -p 123456
```

## 2.3 -C 或 --cpu
```
$ sudo perf stat -C 1,2,3-5
 Performance counter stats for 'CPU(s) 1,2,3-5':

         35,022.65 msec cpu-clock                 #    4.988 CPUs utilized
             2,128      context-switches          #    0.061 K/sec
                14      cpu-migrations            #    0.000 K/sec
               296      page-faults               #    0.008 K/sec
       154,244,208      cycles                    #    0.004 GHz
       163,990,544      instructions              #    1.06  insn per cycle
        33,129,704      branches                  #    0.946 M/sec
           626,766      branch-misses             #    1.89% of all branches

       7.021089473 seconds time elapsed

```

## 2.4 -r 或 --repeat
```
$ perf stat -r 10 ls > /dev/null        # 执行10次程序，给出标准偏差与期望的比值

 Performance counter stats for 'ls' (10 runs):

              1.67 msec task-clock:u              #    0.822 CPUs utilized            ( +-  0.65% )
                 0      context-switches:u        #    0.000 K/sec
                 0      cpu-migrations:u          #    0.000 K/sec
               306      page-faults:u             #    0.183 M/sec                    ( +-  0.04% )
         3,792,140      cycles:u                  #    2.268 GHz                      ( +-  0.25% )
         9,299,050      instructions:u            #    2.45  insn per cycle           ( +-  0.00% )
         1,533,009      branches:u                #  916.863 M/sec                    ( +-  0.00% )
            13,105      branch-misses:u           #    0.85% of all branches          ( +-  0.59% )

         0.0020337 +- 0.0000179 seconds time elapsed  ( +-  0.88% )

```

## 2.5 -A 或 --no-aggr

```
$ sudo perf stat -C 1,2,3-5 -A
 Performance counter stats for 'CPU(s) 1,2,3-5':

CPU1                6,311.19 msec cpu-clock                 #    0.998 CPUs utilized
CPU2                6,311.19 msec cpu-clock                 #    0.998 CPUs utilized
CPU3                6,311.18 msec cpu-clock                 #    0.998 CPUs utilized
CPU4                6,311.18 msec cpu-clock                 #    0.998 CPUs utilized
CPU5                6,311.18 msec cpu-clock                 #    0.998 CPUs utilized
CPU1                     553      context-switches          #    0.088 K/sec
CPU2                     598      context-switches          #    0.095 K/sec
CPU3                      50      context-switches          #    0.008 K/sec
CPU4                     118      context-switches          #    0.019 K/sec
CPU5                      70      context-switches          #    0.011 K/sec
CPU1                       1      cpu-migrations            #    0.000 K/sec
CPU2                       3      cpu-migrations            #    0.000 K/sec
CPU3                       1      cpu-migrations            #    0.000 K/sec
CPU4                       1      cpu-migrations            #    0.000 K/sec
CPU5                       2      cpu-migrations            #    0.000 K/sec
CPU1                       1      page-faults               #    0.000 K/sec
CPU2                     242      page-faults               #    0.038 K/sec
CPU3                       0      page-faults               #    0.000 K/sec
CPU4                      13      page-faults               #    0.002 K/sec
CPU5                       0      page-faults               #    0.000 K/sec
CPU1              35,944,616      cycles                    #    0.006 GHz
CPU2              33,073,143      cycles                    #    0.005 GHz
CPU3               1,847,708      cycles                    #    0.000 GHz
CPU4               7,330,986      cycles                    #    0.001 GHz
CPU5               3,705,764      cycles                    #    0.001 GHz
CPU1              31,344,512      instructions              #    0.87  insn per cycle
CPU2              26,110,831      instructions              #    0.73  insn per cycle
CPU3                 784,895      instructions              #    0.02  insn per cycle
CPU4               3,823,024      instructions              #    0.11  insn per cycle
CPU5                 889,093      instructions              #    0.02  insn per cycle
CPU1               5,993,098      branches                  #    0.950 M/sec
CPU2               5,890,339      branches                  #    0.933 M/sec
CPU3                 155,184      branches                  #    0.025 M/sec
CPU4                 795,714      branches                  #    0.126 M/sec
CPU5                 185,023      branches                  #    0.029 M/sec
CPU1                 146,431      branch-misses             #    2.44% of all branches
CPU2                 128,548      branch-misses             #    2.14% of all branches
CPU3                   8,872      branch-misses             #    0.15% of all branches
CPU4                  21,990      branch-misses             #    0.37% of all branches
CPU5                   3,968      branch-misses             #    0.07% of all branches

       6.326108369 seconds time elapsed

```

# 0x03. 事件

## 3.1 mem_load_retired 

- mem_load_retired.l1_miss:

   描述：记录从 L1 数据缓存加载失败的次数。
   含义：表示加载请求未能在 L1 缓存中命中，导致需要访问更低层次的缓存或内存。

- mem_load_retired.l2_miss:

   描述：记录从 L2 数据缓存加载失败的次数。
   含义：表示加载请求未能在 L2 缓存中命中，需要进一步访问 L3 缓存或主内存。

- mem_load_retired.l3_miss:

   描述：记录从 L3 数据缓存加载失败的次数。
   含义：表示加载请求未能在 L3 缓存中命中，需要访问主内存（DRAM）。

- mem_load_retired.l1_hit:

   描述：记录从 L1 数据缓存加载成功的次数。
   含义：表示加载请求在 L1 缓存中命中。

- mem_load_retired.l2_hit:

   描述：记录从 L2 数据缓存加载成功的次数。
   含义：表示加载请求在 L2 缓存中命中。

- mem_load_retired.l3_hit:

   描述：记录从 L3 数据缓存加载成功的次数。
   含义：表示加载请求在 L3 缓存中命中。

- mem_load_retired.fb_hit

   - 描述：  
      mem_load_retired.fb_hit 记录内存加载操作从填充缓冲区（fill buffer）命中的次数。填充缓冲区是 CPU 中的一种硬件结构，用于在内存加载未命中缓存后，临时存储正在从内存获取的数据。

   - 含义：

      1. 优化的内存访问：

         如果内存加载请求未能在缓存中命中但在填充缓冲区中命中，这意味着正在进行的数据加载操作已经被缓存控制器识别并正在处理。这样可以避免重复访问主内存，从而提高内存访问效率。

      2. 缓存未命中后的性能：

         fb_hit 提供了一个视角，让我们了解缓存未命中后的性能行为。填充缓冲区命中可以减少因为缓存未命中导致的性能损失。

- mem_inst_retired.all_loads

   - 描述：

      mem_inst_retired.all_loads 记录所有已经退休（完成）的内存加载指令。这个计数器包括所有类型的内存加载操作，无论它们是否命中缓存。

   - 含义：

      1. 全面的内存访问统计：

         这个事件提供了程序执行过程中所有内存加载操作的总数，使你能够全面了解程序的内存访问行为。

      2. 基准数据：

         与其他特定事件（例如缓存命中/未命中事件）结合使用时，可以帮助你计算和分析缓存命中率、未命中率以及内存访问延迟等重要性能指标。

## 3.2 cache 相关

A L2 cache hit ratio below 95% is considered to be catastrophic（灾难性的）! (=5% miss), 应该大于 99%

Basic information about your program
- The amount of:
   - instructions executed, 执行的指令数
   - processor cycles spent on the program, 花费的 cycles
   - transactions on the bus, 事务数

- The amount/percentage of:
   - memory loads and stores, 内存存取数
   - floating point operations, 浮点操作数
   - vector operations (SIMD), SIMD 数
   - branch instructions, 分支指令
   - cache misses

Advanced information about your program
- The amount and type of:
   - micro-ops executed, 微指令执行数
   - SIMD instructions executed
   - resource stalls within the CPU, 滞留时间
- Cache access characteristics
   - A rich set on Intel Core CPUs
   - Demand
   - Requests (missed / hit / total / exclusive or shared / store
or read)
   - Lines modified / evicted / prefetched


```
perf stat -e branch-misses,bus-cycles,cache-misses,cache-references,cpu-cycles,instructions,stalled-cycles-backend,stalled-cycles-frontend,alignment-faults,context-switches,cpu-clock,cpu-migrations,emulation-faults,major-faults,minor-faults,page-faults,task-clock,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-store-misses,L1-dcache-stores,L1-icache-load-misses,L1-icache-loads,branch-load-misses,branch-loads,dTLB-load-misses,dTLB-loads,iTLB-load-misses,iTLB-loads ./xxxxx


branch-misses: 
bus-cycles: 见下面
cache-misses: 
cache-references: cache 命中次数
cpu-cycles: 统计 cpu 周期数。cpu 周期：指一条指令的操作时间。
instructions: 执行的指令条数
stalled-cycles-backend: CPU 停滞统计，一种浪费，CPU 必须等待资源（通常是内存）或完成长延迟指令（例如sqrt、倒数、除法等）。
stalled-cycles-frontend: CPU 停滞统计，一种浪费，这意味着前端没有向后端提供 micro-operations。这可能意味着指令缓存中存在缺失，或者 micro-operations 缓存中尚未解码的复杂指令。
alignment-faults: 统计内存对齐错误发生的次数，当访问的非对齐的内存地址时，内核会进行处理，已保存不会发生问题，但会降低性能
context-switches: 上下文切换次数
cpu-clock: cpu clock 的统计，每个 cpu 都有一个高精度定时器
cpu-migrations: 进程运行过程中从一个 cpu 迁移到另一 cpu 的次数
emulation-faults: GPT: 当硬件性能计数器无法直接计数某个事件时，可能会使用软件模拟的方式来模拟计数操作，而在这一过程中可能会发生缺页异常。它的意义在于帮助开发者或系统管理员了解由于模拟或仿真导致的内存访问问题。
major-faults: 页错误，内存页已经被 swap 到硬盘上，需要换回
minor-faults: 页错误，内存页在物理内存中，只是没有和逻辑页进行映射
page-faults: 页错误的统计
task-clock: cpu 处理 task 所消耗的时间，单位 ms, 该值越高代表程序是CPU bound而非IO bound 类型。
L1-dcache-load-misses: 
L1-dcache-loads: 
L1-dcache-store-misses: 
L1-dcache-stores: 
L1-icache-load-misses: 
L1-icache-loads: 
branch-load-misses: 
branch-loads: 
dTLB-load-misses: 
dTLB-loads: 
iTLB-load-misses: 
iTLB-loads: 
```

cycles, ref-cycles 区别
```
cycles: 时钟周期数；
ref-cycles: 时钟周期数，不受动态频率影响的那种。例如 reference cycles 很可能是与 TSC 保持一致的频率
例如程序 A 花了 100 个 core clocks, 程序 B 花了 200 个 core clocks, 由于动态频率的影响，没法直接说 A 和 B 耗费的时间的快慢。但是如果用 ref-cycles 来衡量，谁大谁就肯定花的时间多。
```

在 Intel 中对应三个别名：
- cpu-cycles OR cycles 对应 CPU_CLK_UNHALTED.THREAD
- ref-cycles 对应 CPU_CLK_UNHALTED.REF_TSC
- bus-cycles 对应 CPU_CLK_UNHALTED.REF_XCLK

关于 bus-cycles, 首先 
```
$ perf stat -e bus-cycles,cpu_clk_unhalted.ref_xclk -- sleep 10
Performance counter stats for 'sleep 10':

            11,570      bus-cycles:u                                                
            11,570      cpu_clk_unhalted.ref_xclk:u
```
测试确实是相等的，麻烦在于 bus-cycles 的定义，没有找到一个准确解释，从 GPT 和豆包得到的答案类似：
> 代表的是对总线周期的计数。总线周期是指 CPU 与内存、I/O 设备等通过系统总线进行一次数据传输所需要的时间周期（总线周期通常由若干个时钟周期组成）。每完成一次这样的数据传输，总线周期计数器就会增加，而 bus-cycles 正是用于标识这个计数器。