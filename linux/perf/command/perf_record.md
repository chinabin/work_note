# 0x00. 导读

[stack trace and call graph](../basics/symbol_record.md)

# 0x01. 简介

perf reord 可以运行一个命令，但更多的是对已运行的进程进行性能分析，并将分析结果输出到 perf.data (默认该文件名)中。  
不会像 perf top 一样实时输出分析结果。输出 perf.data 可以通过 perf report 进行分析，或者通过 perf script 输出结果给第三方输出可视化的视图。

# 0x02. 命令

**默认以 4000HZ 的频率收集，大约每秒生成 4000 个采样点**

perf record [options] [command]

以下是 perf record 命令的一些常用参数：    
-F: 事件采样的频率, 单位HZ, 更高的频率会获得更细的统计，但会带来更多的开销  
-e 或 --event：指定要记录的事件类型，例如 cache-misses, instructions 等。  
-p 或 --pid：指定要监控的进程ID。  
-t 或 --tid：指定要监控的线程ID。  
-a 或 --all-cpus：监控所有CPU，而不仅仅是当前CPU。  
-C 或 --cpu：指定要监控的CPU列表。  
-o 或 --output：指定要将数据写入的文件。  
-g: 让 perf record 在记录各个样本的时候，同时记录取样点的 Stack Trace, 其次，使用者可以透过 --call-graph 选项指定走访 Stack Trace 的方法  
--call-graph [fp,dwarf,lbr]：指定调用图记录方法，例如 dwarf 或 fp （帧指针），默认 fp。  
--switch-events：记录上下文切换事件。  

## 2.1 --switch-events

```
# 默认情况下，perf stat 是一秒要采集 4000 次（软件事件频率）。
perf record -F 99 -g -p 11488 --switch-events
```

## 2.2 -g 或 --call-graph
```
# 默认是 frame pointer
perf record --call-graph dwarf ./a.out
```