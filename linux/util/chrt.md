# 0x00. 导读

# 0x01. 简介

查看和设置进程的调度策略。

# 0x02. 参数

```bash
$ chrt --help
Show or change the real-time scheduling attributes of a process.

Set policy:
 chrt [options] <priority> <command> [<arg>...]
 chrt [options] --pid <priority> <pid>

Get policy:
 chrt [options] -p <pid>

Policy options:
 -b, --batch          set policy to SCHED_BATCH
 -d, --deadline       set policy to SCHED_DEADLINE
 -f, --fifo           set policy to SCHED_FIFO
 -i, --idle           set policy to SCHED_IDLE
 -o, --other          set policy to SCHED_OTHER
 -r, --rr             set policy to SCHED_RR (default)

Scheduling options:
 -R, --reset-on-fork       set SCHED_RESET_ON_FORK for FIFO or RR
 -T, --sched-runtime <ns>  runtime parameter for DEADLINE
 -P, --sched-period <ns>   period parameter for DEADLINE
 -D, --sched-deadline <ns> deadline parameter for DEADLINE

Other options:
 -a, --all-tasks      operate on all the tasks (threads) for a given pid
 -m, --max            查看不同的调度策略下，用户可选的优先级范围
 -p, --pid            operate on existing given pid
```

```bash
# 将 PID 为 137619 的进程设置为 SCHED_RR 实时调度，且优先级为 30
$ chrt -r 30 -p 137619
```