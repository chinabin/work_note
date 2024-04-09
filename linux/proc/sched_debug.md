# 0x00. 导读

# 0x01. 简介

查看所有 CPU 的调度队列状态信息以及系统中所有进程的调度信息。

Contains the current values of all tunable variables that affect the task scheduler behavior, CFS statistics, and information about the run queues (CFS, RT and deadline) on all available processors.   
A summary of the task running on each processor is also shown, with the task name and PID, along with scheduler specific statistics.   
The first being tree-key column, it indicates the task's virtual runtime, and its name comes from the kernel sorting all runnable tasks by this key in a red-black tree. The switches column indicates the total number of switches (involuntary or not), and naturally the prio refers to the process priority. The wait-time value indicates the amount of time the task waited to be scheduled. Finally both sum-exec and sum-sleep account for the total amount of time (in nanoseconds) the task was running on the processor or asleep, respectively.

# 0x02. 