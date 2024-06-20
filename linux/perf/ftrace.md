# 0x00. 导读

[Ftrace的配置和使用](https://www.cnblogs.com/hpyu/p/14348151.html)  
[Ftrace使用及实现机制](https://www.cnblogs.com/wsg1100/p/17020703.html)

# 0x01. 简介

ftrace 是基于 Linux 中 tracefs 实现的一种可以用来追踪内核函数执行时间、调用关系、调用堆栈等信息的文件系统。

**不同的 tracer 会使得 options 目录的内容不同**

```
cmd： 执行程序名
pid：进程pid
CPU#： CPU id
irqs-off：当前CPU中断是否是关闭状态
need-resched：TIF_NEED_RESCHED和TIF_NEED_RESCHED的设置状态
hardirq/softirq： 中断处理上下文状态
preempt-depth：preempt_disabled的深度
```

```
# 体系结构相关的实现
CONFIG_HAVE_FUNCTION_TRACER=y
CONFIG_HAVE_FUNCTION_GRAPH_TRACER=y
CONFIG_HAVE_DYNAMIC_FTRACE=y
CONFIG_HAVE_FTRACE_MCOUNT_RECORD=y
CONFIG_HAVE_SYSCALL_TRACEPOINTS=y
CONFIG_HAVE_ARCH_TRACEHOOK=y

# Function Trace
CONFIG_FTRACE=y
CONFIG_FUNCTION_TRACER=y
CONFIG_FUNCTION_GRAPH_TRACER=y
CONFIG_DYNAMIC_FTRACE=y # 打开DYANMIC_FTRACE后性能开销可忽略

# 各种tracer的支持
CONFIG_GENERIC_TRACER=y
CONFIG_NOP_TRACER=y
CONFIG_STACK_TRACER=y
CONFIG_IRQSOFF_TRACER=y
CONFIG_PREEMPT_TRACER=y
CONFIG_SCHED_TRACER=y
CONFIG_CONTEXT_SWITCH_TRACER=y
CONFIG_TRACER_SNAPSHOT=y
CONFIG_TRACER_SNAPSHOT_PER_CPU_SWAP=y

# 静态Trace events
CONFIG_TRACEPOINTS=y
CONFIG_EVENT_TRACING=y
CONFIG_TRACING_SUPPORT=y
CONFIG_TRACING=y

# 动态Trace events
CONFIG_PROBE_EVENTS=y
CONFIG_KPROBE_EVENTS=y
CONFIG_UPROBE_EVENTS=y

# 其它相关配置
CONFIG_TRACE_CLOCK=y
CONFIG_STACKTRACE_SUPPORT=y
CONFIG_STACKTRACE=y
CONFIG_TRACE_IRQFLAGS_SUPPORT=y
CONFIG_TRACE_IRQFLAGS=y
CONFIG_SYSCTL_EXCEPTION_TRACE=y
CONFIG_TRACER_MAX_TRACE=y
CONFIG_FTRACE_SYSCALLS=y
CONFIG_BLK_DEV_IO_TRACE=y

```

trace_pipe 是消耗性的，cat 一次后 buffer 会清空，可用于持续导出 trace log  

per_cpu 是个目录，可以对每个 cpu 的 buffer size 进行设置，和查看 trace 内容

TIMESTAMP: 秒.微秒。时间戳是进入 所追踪的函数 时的时间。

```
entries-in-buffer/entries-written: 358096/10552611   #P:8

358096/10552611,由于缓冲区填满而丢失的条目数（10552611−358096=10,194,515事件丢失），接着是处理器数量：8
```

# 0x02.

[ftrace.txt](https://www.kernel.org/doc/Documentation/trace/ftrace.txt)

`/sys/kernel/debug/tracing` 目录：

- current_tracer

    用来决定要使用哪种 tracer 来跟踪内核。可用 tracer 在 avaliable_tracers 文件中。

    比如 function 可以用来追踪系统函数调用，function_graph 可以用来追踪函数调用同时还可以把调用关系以及函数执行时间给获取到等。

- set_ftrace_filter

    配置追踪函数，支持通配符，比如对其 echo *icmp* > set_ftrace_filter 的话，那就可以只追踪和 icmp 相关的函数。

- set_ftrace_pid

    用来指定要追踪哪个进程，只需要将想要追踪的进程的 pid 给写进去，那就只追踪该进程相关的内核调用函数。如果不指定该配置的话，那 tracer 会把所有进程的内核调用都给抓到，最后的输出结果就会贼大。

- tracing_on

    该选项用来决定是否开启 tracer 功能，配置为 0 则不开启，配置为 1 则为开启。

- tracer

    该文件不是配置项，但是通过 fstrace 追踪到的函数调用结果会输出到这个文件中，这个文件就相当于一个缓冲区，当超过缓冲区大小后，新的结果会覆盖旧的结果。

- available_filter_functions

    该文件中记录了操作系统可以追踪的所有函数。

- available_events

    该文件中记录可以追踪哪些事件，比如像 socket 的 send 或者 recv 之类的系统调用等算是事件，都可以被追踪到。

- trace_options

    显示出当前所选择的跟踪器的option配置

- tracing_cpumask

    需要监控的CPU

    ffffff

- set_graph_function

    设置要清晰显示调用关系的函数，以 C 语言缩进式地呈现

- options/function-trace

    如果启用此选项，则latency跟踪器将启用函数跟踪（默认）。 禁用时，latency跟踪器不跟踪函数。 这样可以在执行延迟测试时降低跟踪器的开销。

准备工作：  
1. 
2. sysctl kernel.ftrace_enabled=1

实操：
```
# echo function > current_tracer; echo smp_apic_timer_interrupt > set_ftrace_filter; echo 0 > tracing_on; echo > trace; echo 1 > tracing_on
# cp trace /home/alfred/trace
$ echo function > current_tracer
$ echo smp_apic_timer_interrupt > set_ftrace_filter
$ echo 1 > tracing_on
$ cat trace


# echo nop > current_tracer; echo 0 > tracing_on; echo  > set_ftrace_filter
$ echo nop > current_tracer
$ echo 0 > tracing_on
# 注意 echo 后面有一个空格，即 “echo+ 空格>" 
$ echo  > set_ftrace_filter
$ 
```
