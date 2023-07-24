# 0x00. 导读

# 0x01. 简介

perf list命令可以列出当前perf可用的事件：

```
$ sudo perf list
```

# 0x02. 事件

这些事件可以分为三类:
- Hardware Event
- Software event
- Tracepoint event.

每个具体事件的含义在perf_event_open的man page中有说明：

- cpu-cycles: 统计cpu周期数，cpu周期：指一条指令的操作时间。
- instructions:  机器指令数目
- cache-references:  cache命中次数
- cache-misses:  cache失效次数
- branch-instructions:  分支预测成功次数
- branch-misses:  分支预测失败次数
- alignment-faults:  统计内存对齐错误发生的次数， 当访问的非对齐的内存地址时，内核会进行处理，已保存不会发生问题，但会降低性能
- context-switches:  上下文切换次数，
- cpu-clock:  cpu clock的统计，每个cpu都有一个高精度定时器
- task-clock : cpu clock中有task运行的统计
- cpu-migrations: 进程运行过程中从一个cpu迁移到另一cpu的次数
- page-faults:  页错误的统计
- major-faults: 页错误，内存页已经被swap到硬盘上，需要I/O换回
- minor-faults : 页错误，内存页在物理内存中，只是没有和逻辑页进行映射

```
perf stat -e alignment-faults,context-switches,cpu-migrations,page-faults,major-faults,minor-faults
```