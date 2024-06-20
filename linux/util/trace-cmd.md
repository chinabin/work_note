# 0x00. 导读

# 0x01. 简介

我们可以手工操作 /sys/kernel/debug/tracing 路径下的大量的配置文件接口，来使用 ftrace 的强大功能。但是这些接口对普通用户来说太多太复杂了，我们可以使用对 ftrace 功能进行二次封装的一些命令来操作。

trace-cmd 就是 ftrace 封装命令其中的一种。该软件包由两部分组成：

1. trace-cmd。提供了数据抓取和数据分析的功能。
2. kernelshark。可以用图形化的方式来详细分析数据，也可以做数据抓取。

安装：
```bash
$ sudo yum install -y trace-cmd
```

# 0x02.

```
-p function_graph: 指定当前 tracer 为 function_graph, 只有 function_graph 才能测量函数执行时间

-l abcd*: 函数过滤，指定 function_graph 追踪哪些函数。function_graph有两种过滤条件可以配置：
    -l func。实际对应 set_ftrace_filter，这种方式插桩的开销较小，只会追踪顶层 func 的执行时间且支持*等通配符的设置。粗粒度分析
    -g func。实际对应 set_graph_function，这种方式插桩的开销较大，但能追踪 func 以及 func 所有子函数的的执行时间，不支持*等通配符的设置。细粒度分析

-P pid: 指定进程。
```

```
commands:
     record - record a trace into a trace.dat file
     start - start tracing without recording into a file
     extract - extract a trace from the kernel
     stop - stop the kernel from recording trace data
     restart - restart the kernel trace data recording
     show - show the contents of the kernel tracing buffer
     reset - disable all kernel tracing and clear the trace buffers
     clear - clear the trace buffers
     report - read out the trace stored in a trace.dat file
     stream - Start tracing and read the output directly
     profile - Start profiling and read the output directly
     hist - show a historgram of the trace.dat information
     stat - show the status of the running tracing (ftrace) system
     split - parse a trace.dat file into smaller file(s)
     options - list the plugin options available for trace-cmd report
     listen - listen on a network socket for trace clients
     list - list the available events, plugins or options
     restore - restore a crashed record
     snapshot - take snapshot of running trace
     stack - output, enable or disable kernel stack tracing
     check-events - parse trace event formats


子命令后面使用 --help 或者 -h 继续查看帮助
trace-cmd list --help
```


```bash
# 查看可用的 tracer
$ trace-cmd list -t
hwlat blk mmiotrace function_graph wakeup_dl wakeup_rt wakeup function nop
# 查看可过滤的函数
$ trace-cmd list -f


# 分析数据
$ trace-cmd report --profile
# 显示 trace 内容
$ trace-cmd show
```

# 0x03. 例子

```bash
$ trace-cmd record -p function_graph -g do_sys_open -P 123456
$ trace-cmd report
```