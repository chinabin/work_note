# 0x00. 导读

[1111](https://www.cnblogs.com/LoyenWang/p/12770878.html)

# 0x01. 简介

RCU(Read-Copy Update) 是数据同步的一种方式，历史中分为 经典RCU 和 tree RCU 。

RCU 的原理可以简述如下： RCU 记录了所有对共享数据的使用者。当内核线程需要 write 某个数据时，先创建一个副本，在副本中修改。当所有读线程都离开临界区后，新的数据才被更新。

场景：  
RCU 适用于需要频繁的读取数据，而相应修改数据并不多的情景，例如在文件系统中，经常需要查找定位目录，而对目录的修改相对来说并不多，这就是RCU发挥作用的最佳场景。


# 0x02. 详解

目前内核支持3种RCU。

- 不可抢占RCU（RCU-sched）。不允许进程在读端临界区被其他进程抢占。

- 加速版不可抢占RCU（RCU-bh，bh是“bottom half”的缩写，下半部），是针对不可抢占RCU的改进，在软中断很多的情况下可以缩短宽限期。

- 可抢占RCU（RCU-preempt），也称为实时 RCU，在内核版本2.6.26中引入。可抢占 RCU 允许进程在读端临界区被其他进程抢占。

## 2.1 概念

宽限期 (Grace Period, GP) ，宽限期是一个 period，可长可短。它的意义是， 在一个删除动作发生后，它必须等待所有在宽限期开始前已经开始的读线程结束，才可以进行真正的销毁操作。 

宽限期从删除动作发生后开始，此时 time 为 t1 ；  
当所有的读线程都结束后宽限期结束，此时 time 为 t2 。  
GP=t2-t1 。因此宽限期可长可短，取决于读线程读多久。  

![Alt text](../../pic/linux/system/rcu1.png)

Quiescent Status，用于描述处理器的执行状态。当某个CPU正在访问RCU保护的临界区时，认为是活动的状态，而当它离开了临界区后，则认为它是静止的状态。方便触发回收工作。

## 2.2 接口

在 Linux kernel 中还专门提供了一个头文件 `include/linux/rculist.h` ，提供了利用 RCU 机制对链表进行增删查改操作的接口。

内核中关于RCU的常见接口如下：

- 使用函数 rcu_read_lock() 标记进入读端临界区，使用函数 rcu_read_unlock() 标记退出读端临界区。读端临界区可以嵌套。
- 在读端临界区里面应该使用宏 rcu_dereference(p) 访问指针。
- 写的时候有 4 个函数可以使用：
    - 使用函数 synchronize_rcu() 等待宽限期结束，即所有读者退出读端临界区，然后写者执行下一步操作。这个函数可能睡眠。

    - 使用函数 synchronize_rcu_expedited() 等待宽限期结束。和函数 synchronize_rcu() 的区别是：该函数会向其他处理器发送处理器间中断 (Inter-Processor Interrupt，IPI) 请求，强制宽限期快速结束。我们把强制快速结束的宽限期称为加速宽限期 (expedited grace period) ，把没有强制快速结束的宽限期称为正常宽限期 (normal grace period) 。

    - 使用函数 call_rcu() 注册延后执行的回调函数，把回调函数添加到 RCU 回调函数链表中，立即返回，不会阻塞。

    - 使用函数 rcu_barrier() 等待使用 call_rcu 注册的所有回调函数执行完。这个函数可能睡眠。


不管是同步还是异步接口，最终都是调到 `__call_rcu` 接口，通过 __call_rcu 注册的这些回调函数在哪里调用呢？答案是在 `RCU_SOFTIRQ` 软中断中。

线程: rcu_sched rcu_preempt rcu_bh, [The new visibility of RCU processing](https://lwn.net/Articles/518953/)