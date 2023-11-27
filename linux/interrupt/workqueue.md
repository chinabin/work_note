# 0x00. 导读

内容来自 [analysis-of-linux-workqueue](http://www.binss.me/blog/analysis-of-linux-workqueue/)

# 0x01. 简介

workqueue 是自 kernel 2.6 引入的一种任务执行机制，和 softirq 、 tasklet 并称下半部 (bottom half) 三剑客。比起这两者，workqueue 在进程上下文异步执行任务，能够进行睡眠，很快就受到内核开发者们的追捧。

workqueue 最核心的思想是分离了任务 (work) 的发布者和执行者。当需要执行任务的时候，构造一个 work，塞进相应的 workqueue，由 workqueue 所绑定的 worker (thread) 去执行。如果任务不重要，我们可以让多种任务共享一个 worker，而无需每种任务都开一个 thread 去处理(n -> 1)；相反如果任务很多很重要，那么我们可以开多个 worker 加速处理(1 -> n)，类似于生产者消费者模型。

# 0x01. CMWQ

在传统的实现中，workqueue 和 worker 的对应关系是二元化的：要么使用 Multi Threaded (MT) workqueue 每个 CPU 一个 worker，要么使用 Single Threaded (ST) workqueue 整个系统只有一个 worker。但即使是通过 MT 每个 CPU 都开一个 worker ，它们相互之间是独立的，在哪个 CPU 上挂入的 work 就只能被那个 CPU 上的 worker 处理，这样当某个 worker 因为处理 work 而阻塞时，位于其他 CPU 上的 worker 只能干着急，这并不是我们所期待的并行。更麻烦的是，它很容易导致死锁，比如有 A 和 B 两个 work，B 依赖于 A 的执行结果，而此时如果 A 和 B 被安排由同一个 worker 来做，而 B 恰好在 A 前面，于是形成死锁。

为了解决这个问题，Tejun Heo 在 2009 年提出了 `CMWQ(Concurrency Managed Workqueue)` ，于 2.6.36 进入 kernel 。

相比传统实现，CMWQ 做了一个很重要的改进就是 workqueue 和 worker 进行解耦，提出了 worker pool 的概念。worker 的创建不再与 workqueue 相关，而是由 worker pool 统一管理。不同 workqueue 共享全局的 worker pool，但 workqueue 可以根据需要 (flags) 选择使用特定的 worker pool 。

整理一下 workqueue 中出现的角色：

work ：工作。被加到 workqueue 中  
workqueue ：工作队列。逻辑上是 work 排成的队列  
worker：工人。本质上是内核线程(kthread)，负责真正执行 work  
worker pool：worker 的集合。负责管理 worker。  

# 0x02. worker pool

按照运行特性，主要 `CPU bound` 和 `CPU unbound` 分为两类。

[kworker vs events](https://unix.stackexchange.com/questions/436671/difference-between-kworker-n-and-events-n)

## 2.1 CPU bound worker pool

绑定特定 CPU，其管理的 worker 都运行在该 CPU 上。

根据优先级分为 `normal pool` 和 `high priority pool`，后者管理高优先级的 worker。

Linux 会为每个 online CPU 都创建 1 个 normal pool 和 1 个 high priority pool，并在命名上进行区分。

比如 [kworker/1:1] 表示 CPU 1 上 normal pool 的第 1 个 worker ，而 [kworker/2:0H] 表示 CPU 2 上 high priority pool 的第 0 个 worker

## 2.2 CPU unbound worker pool

其管理的 worker 可以运行在任意的 CPU 上。

比如 [kworker/u32:2] 表示 unbound pool 32 的第 2 个 worker 进程