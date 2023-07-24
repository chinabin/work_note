# 0x00. 导读

futex 全称为 Fast User-space Mutex ，是 Linux 2.5.7 内核引入的锁原语，不同于其他进程间通信 IPC 原语（如信号量 Semaphore 、信号 Signal 和各种锁 pthread_mutex_lock ）， futex 更轻量级、快速，一般应用开发人员可能很少用到，但可基于 futex 实现各类读写锁、屏障（ barriers ）和信号机制等。

# 0x01. 简介

futex的性能非常优异，它是怎样做到的呢？

这要从它的设计思想谈起。传统的 SystemV IPC(inter process communication) 进程间同步机制都是通过内核对象来实现的，以 semaphore 为例，当进程间要同步的时候，必须通过系统调用 semop(2) 进入内核进行 PV 操作（ PV 操作是一种实现进程互斥与同步的有效方法。 P 表示资源申请的意思， V 表示资源释放的意思。）。

系统调用的缺点是开销很大，需要从 user mode 切换到 kernel mode 、保存寄存器状态、从 user stack 切换到 kernel stack 、等等，通常要消耗上百条指令。事实上，有一部分系统调用是可以避免的，因为现实中很多同步操作进行的时候根本不存在竞争，即某个进程从持有 semaphore 直至释放 semaphore 的这段时间内，常常没有其它进程对同一 semaphore 有需求，在这种情况下，内核的参与本来是不必要的，可是在传统机制下，持有 semaphore 必须先调用 semop(2) 进入内核去看看有没有人和它竞争，释放 semaphore 也必须调用 semop(2) 进入内核去看看有没有人在等待同一 semaphore ，这些不必要的系统调用造成了大量的性能损耗。

# 0x02. Futex

futex 就为了解决这个问题而生的，它的办法是：在无竞争的情况下， futex 的操作完全在 user space 进行，不需要系统调用，仅在发生竞争的时候进入内核去完成相应的处理( wait 或者 wake up )。
所以说， futex 是一种 user mode 和 kernel mode 混合的同步机制，需要两种模式合作才能完成， futex 变量必须位于 user space ，而不是内核对象， futex 的代码也分为 user mode 和 kernel mode 两部分，无竞争的情况下在 user mode ，发生竞争时则通过 sys_futex 系统调用进入 kernel mode 进行处理，具体来说：

- futex 变量是位于 user space 的一个整数，支持原子操作。

## 2.1 同步简介

futex 同步操作都是从 user space 开始的：

- 当要求持有 futex 的时候，对 futex 变量执行 down 操作，即原子递减
    - 如果变量变为 0 ，则意味着没有竞争发生，进程成功持有 futex 并继续在 user mode 运行；
    - 如果变量变为负数，则意味着有竞争发生，需要通过 sys_futex 系统调用进入内核执行 futex_wait 操作，让进程进入休眠等待。
- 当释放 futex 的时候，对 futex 变量进行 up 操作，即原子递增
    - 如果变量变成 1 ，则意味着没有竞争发生，进程成功释放 futex 并继续在 user mode 执行
    - 否则意味着有竞争，需要通过 sys_futex 系统调用进入内核执行 futex_wake 操作，唤醒正在等待的进程。

如果需要在多个进程之间共享 futex ，那就必须把 futex 变量放在共享内存中，并确保这些进程都有访问共享内存的权限；如果仅需在线程之间使用 futex 的话，那么 futex 变量可以位于进程的私有内存中，比如普通的全局变量即可。