# 0x00. 导读

调度器的核心工作就是把可运行的任务扔到CPU上去运行。如何有效、合理地组织可运行的任务，是调度器需要考虑的重要问题。

Linux 内核使用运行队列 (runqueue) 来存放可运行的任务。

# 0x01. 简介

用来存放等待被执行进程的队列，这个队列中的进程可能是 runnable 或 uninterruptible。每个 core 都有各自的 run queue 来存放符合条件的进程。

- runnable 的进程可以认为是所有的资源全部准备就绪，就等 CPU 来执行了
- uninterruptible 的进程属于不可中断睡眠进程，可以任务它正在等待 IO 的返回，比如说在等待磁盘读操作返回的一个进程，就处于 uninterruptible 状态（D 状态）

`vmstat` 可以看到有多少在 run queue 中的进程是 runnable 或 blocked .

# 0x02. 

该运⾏队列中包含 dl_rq/rt_rq/cfs_rq 等多个调度类的运行队列。
