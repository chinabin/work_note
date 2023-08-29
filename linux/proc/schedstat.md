# 0x00. 导读

`/proc/pid/schedstat` 是 Linux 操作系统中一个虚拟文件，用于记录进程调度器的统计信息。

# 0x01. 简介

# 0x02. 命令

文件内容的格式为：
1. nr_switches  进程切换次数
2. nr_voluntary_switches  自愿切换次数
3. nr_involuntary_switches  非自愿切换次数
4. cpu_time  在CPU上运行的时间
5. wait_time  在队列中等待的时间

# 0x03. Run Queue

[RunQueue.md](../sched/RunQueue.md)