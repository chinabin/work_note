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
insns per cycle: 即 IPC ，每个 cpu 周期执行的指令条数，1.11 = 6100570(instructions) / 5506056(cycles)。  
**IPC 比上面的 CPU 使用率更能说明 CPU 的使用情况**，关于 IPC 有一篇 [brendangregg的文章](https://www.brendangregg.com/blog/2017-05-09/cpu-utilization-is-wrong.html)，很好的说明 CPU 使用率不是一个很好的性能分析指标。

3. cpu-migrations: 程序在运行过程中发生的CPU迁移次数，即被调度器从一个 CPU 转移到另外一个 CPU 上运行。

# 0x02. 命令

perf stat [options] [command]
 
-e 或 --event: 指定要收集的事件类型，例如：cache-misses, instructions 等。  
-p 或 --pid: 指定要监视的进程ID。  
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
$ perf -p 123456
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