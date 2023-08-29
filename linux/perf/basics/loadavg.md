# 0x00. 导读

# 0x01. 简介

我们平时所讲的 load ，其全称是 Linux system load averages ，即 linux 系统负载平均值。

注意两个关键词：一个是 **负载**，它衡量的是 task ( linux 内核中用于描述一个进程或者线程) 对系统的需求 (CPU、内存、IO 等等) ，第二个关键词是 **平均**，它计算的是一段时间内的平均值，分别为 1、5 和 15 分钟值。system load average 由内核负载计算并记录在 /proc/loadavg 文件中， 用户态的工具（比如 uptime, top 等等）读的都是这个文件。

我们一般认为：
1. 如果load接近0，意味着系统处于空闲状态；
2. 如果 1min 平均值高于 5min 或 15min 平均值，则负载正在增加；
3. 如果 1min 平均值低于 5min 或 15min 平均值，则负载正在减少；
4. 如果它们高于系统 CPU 的数量，那么系统很可能遇到了性能问题（视情况而定）。

# 0x02. Load Average( 平均负载 )

[man uptime](https://man7.org/linux/man-pages/man1/uptime.1.html)  
> System load averages is the average number of processes that are
either in a runnable or uninterruptable state.  A process in a
runnable state is either using the CPU or waiting to use the CPU.
A process in uninterruptable state is waiting for some I/O
access, eg waiting for disk.  The averages are taken over the
three time intervals.  Load averages are not normalized for the
number of CPUs in a system, so a load average of 1 means a single
CPU system is loaded all the time while on a 4 CPU system it
means it was idle 75% of the time.

系统平均负载是指 **处于可运行或不可中断状态的进程的平均数**。  
处于可运行状态的进程是使用CPU或等待使用 CPU。   
处于可中断状态的进程是正在等待某些 I/O 访问，例如，等待磁盘 IO。

**重点 1** 在于 不可中断状态的进程 也纳入了统计，使得数字所反应的含义不是那么简单。  
**重点 2** 在于 1分钟、5分钟、15分钟 并不是简单的平均数。

## 2.1 重点 1

在 文章 [Linux Load Averages: Solving the Mystery](https://www.brendangregg.com/blog/2017-08-08/linux-load-averages.html) ([中文](https://blog.csdn.net/sunny_98_98/article/details/120880676)) 中大致有介绍 Load Average 是如何一步步演变：

最开始的时候：
> The original load averages show only CPU demand: the number of processes running plus those waiting to run.

然后加入变化 -- 不可中断状态线程：
> When load averages first appeared in Linux, they reflected CPU demand, as with other operating systems(就和其它操作系统一样，反应的是 CPU 的情况).   
> But later on Linux changed them to include not only runnable tasks, but also tasks in the uninterruptible state

这使得：
> Adding the uninterruptible state means that Linux load averages can increase due to a disk (or NFS) I/O workload, not just CPU demand. For everyone familiar with other operating systems and their CPU load averages, including this state is at first deeply confusing.

> On Linux, load averages are (or try to be) "system load averages", for the system as a whole, measuring the number of threads that are working and waiting to work (CPU, disk, uninterruptible locks). Put differently, it measures the number of threads that aren't completely idle. Advantage: includes demand for different resources.  
在 Linux 上，负载平均值是（或尝试成为）“系统负载平均值”，对于整个系统而言，其测量的是正在工作和等待工作的线程数（CPU、磁盘、不间断锁）。换句话说，它测量未完全空闲的线程数。这样统计的优点是：包括对不同资源的需求。


总而言之就是说：  
**CPU负载平均值** 变成了所谓的 **系统负载平均值** 。

所以大神建议，使用更加专业和精确的命令去查找你要的答案：

1. per-CPU utilization: eg, using mpstat -P ALL 1  
每个 CPU 的使用率: `mpstat -P ALL 1`

2. per-process CPU utilization: eg, top, pidstat 1, etc.  
每个进程使用 CPU 的比率: `top or pidstat 1`

3. per-thread run queue (scheduler) latency: eg, in /proc/PID/schedstat, perf sched  
每个线程 run queue 延迟: `/proc/PID/schedstat`, `perf sched`

4. CPU run queue latency: eg, in /proc/schedstat, perf sched.  
CPU run queue 延迟: `/proc/schedstat`, `perf sched`

5. CPU run queue length: eg, using vmstat 1 and the 'r' column.  
CPU run queue 长度: `vmstat 1` 的 r 列

## 2.2 重点 2

核心算法其实就是 **指数加权移动平均法 (Exponential Weighted Moving Average，EMWA)**，简单表示就是:  
`a1 = a0 * factor + a * (1 - factor)`，其中 a0 是上一时刻的值，a1 是当前时刻的值，factor 是一个系数，取值范围是[0,1]，a 是当前时刻的某个指标采样值。

为什么要采用指数移动加权平均法？我个人理解：
1. 指数移动加权平均法，是指各数值的加权系数随时间呈指数式递减，**越靠近当前时刻的数值加权系数就越大，更能反映近期变化的趋势**；
2. 计算时不需要保存过去所有的数值，这对内核非常重要。

我们来看看，内核是怎么计算 load average 的，以下简称 load。

上面的指数移动平均公式，a1 = a0 * e + a * (1 - e)，具体到 linux load 的计算，a0 是上一时刻的 load，a1 是当前时刻的 load，e 是一个常量系数，a 是当前时刻的 active 的进程/线程数量。

linux 内核计算了三个 load 值，分别是 1分钟/5分钟/15分钟 load 。计算这三个load 值时，使用了三个不同的常量系数 e，定义如下：
```c
#define EXP_1 1884       /* 1/exp(5sec/1min) */
#define EXP_5 2014       /* 1/exp(5sec/5min) */
#define EXP_15 2037      /* 1/exp(5sec/15min) */
```

内核实际代码:
```c
/*
 * a1 = a0 * e + a * (1 - e)
 */     
static inline unsigned long
calc_load(unsigned long load, unsigned long exp, unsigned long active)
{       
        unsigned long newload;
        // FIXED_1 = 2048
        newload = load * exp + active * (FIXED_1 - exp);
        if (active >= load)
                newload += FIXED_1-1;

        return newload / FIXED_1;
}
// 第一个参数就是上一时刻的load， 第二个参数就是常量系数，
// 第三个参数是active的进程/线程数量（包括runnable 和 uninterruptible）
```