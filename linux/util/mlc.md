# 0x00. 导读

# 0x01. 简介

[下载地址](https://www.intel.com/content/www/us/en/download/736633/736634/intel-memory-latency-checker-intel-mlc.html)

[文档](https://www.intel.com/content/www/us/en/developer/articles/tool/intelr-memory-latency-checker.html)

Intel Memory Latency Checker(Intel MLC) 是一个测试内存延迟和带宽的工具，并且可以测试延迟和带宽随着系统负载增加的变化。这个工具也提供了一些选项用于更好细粒度调查从特定处理器核心到缓存或内存的一系列选项的测试。

在 Linux 环境中，使用 MLC 需要 root 权限，因为这个工具修改 H/W prefetch control MSR （硬件预取控制寄存器）来 激活/禁用 预取器 用于延迟和带宽测试。因为在新型的 Intel 处理器上精确测试内存延迟是非常困难的，因为它有复杂的硬件预取器。Intel MLC 在测试延迟时会自动禁用这些预取器，并且在测试完成后自动恢复预期器原状态。预期器控制是通过修改 MSR 实现。

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

# 2. 不同读写比下的内存带宽
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

# 5. 处理器缓存间的延迟
# mlc --c2c_latency
Measuring cache-to-cache transfer latency (in ns)...      
Local Socket L2->L2 HIT  latency        58.3             
Local Socket L2->L2 HITM latency        52.6             
Remote Socket L2->L2 HITM latency (data address homed in writer socket)
                        Reader Numa Node          
Writer Numa Node     0       1  
            0        -   116.0  
            1    116.1       -  
Remote Socket L2->L2 HITM latency (data address homed in reader socket)
                        Reader Numa Node                  
Writer Numa Node     0       1                           
            0        -   116.0                           
            1    116.1       -                                                                                       
Remote Socket L2->L2 HITM latency (data address homed in reader socket)
                        Reader Numa Node
Writer Numa Node     0       1  
            0        -   177.8  
            1    175.5       -  
```

## 2.2

```
-a 测试所有可用CPU的idle延迟
-b 设置每个CPU的分配缓存大小（KB），默认是200MB延迟测试，100MB带宽测试
-c 将延迟测试的线程pin到指定CPU。所有内存访问都将从这个特定的CPU发出
-d 设置特定的负载注入延迟
```