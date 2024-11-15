# 0x00. 导读

# 0x01. 简介

```
--syscalls                  Trace syscalls
--kernel-syscall-graph      Show the kernel callchains on the syscall exit path
--sched           show blocking scheduler events
```

超过一定延迟的系统调用信息, show only events with duration > N.M ms
`perf trace --duration 200`

统计某个进程一段时间内系统调用的开销
`perf trace -p $PID -s`, -S 可以显示详细