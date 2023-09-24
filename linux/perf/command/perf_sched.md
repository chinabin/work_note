# 0x00. 导读

# 0x01. 简介

perf sched 使用了转储后再分析 (dump-and-post-process) 的方式来分析内核调度器的各种事件。而这往往带来一些问题，因为这些调度事件通常非常地频繁，可以达到每秒钟百万级， 进而导致 CPU，内存和磁盘的在 **调度器事件记录** 上的严重开销。

# 0x02. 命令

我们以记录一秒钟内的所有调度事件作为开始：
```bash
# 开始必须运行 perf sched record 不然后面的命令没法执行，例如 perf sched latency
$ perf sched record -- sleep 1
[ perf record: Woken up 1 times to write data ]
[ perf record: Captured and wrote 1.886 MB perf.data (13502 samples) ]
```

```bash
$ perf sched -h
 Usage: perf sched [] {record|latency|map|replay|script|timehist}

    -D, --dump-raw-trace  dump raw trace in ASCII
    -f, --force           don't complain, do it
    -i, --input     input file name
    -v, --verbose         be more verbose (show symbol address, etc)
```

# 0x03. 解析

perf sched latency 可以给出每个任务 (task) 的调度延迟，包括平均和最大延迟:

perf sched script 展现其原始调度事件 (raw event)

perf sched map 显示所有的 CPU 的上下文切换的事件，其中的列输出了每个 CPU 正在做什么，以及具体时间。

perf sched timehist 是 Linux 4.10 里加入的，可以按照调度事件表示调度延迟，其中包括了任务等待被唤醒的时间 (wait time) 和任务从被唤醒之后到运行态的调度延迟。