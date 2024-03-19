# 0x00. 导读

# 0x01. 简介

softirq 是一种推迟中断处理机制，将 IRQ 的大部分处理逻辑推迟到了这里执行。 tasklet 是基于 softirq ，实际驱动编程一般不直接使用软中断(内核固定了用途),用 tasklet 代替；

softirq不能动态分配，都是静态定义的。内核已经定义了若干种softirq number，例如网络数据的收发、block设备的数据访问（数据量大，通信带宽高），timer的deferable task（时间方面要求高）。

```c
// include/linux/interrupt.h

enum {
    HI_SOFTIRQ=0,          // tasklet
    TIMER_SOFTIRQ,         // timer
    NET_TX_SOFTIRQ,        // networking
    NET_RX_SOFTIRQ,        // networking
    BLOCK_SOFTIRQ,         // IO
    IRQ_POLL_SOFTIRQ,
    TASKLET_SOFTIRQ,       // tasklet
    SCHED_SOFTIRQ,         // schedule
    HRTIMER_SOFTIRQ,       // timer
    RCU_SOFTIRQ,           // lock
    NR_SOFTIRQS
};

// 也就是在 cat /proc/softirqs 看到的那些
```

# 0x02.

一个已注册的软中断必须被标记后才能执行，这被叫做触发软中断（raising the softirq）。通常，中断处理程序会在返回之前触发它对应的软中断（实际上就是将 pending bitmask 对应的bit位置1）。然后软中断会在一个合适的时机运行（do_softirq() 会检查 bitmask 然后执行）。


# 0x03. 内核线程 ksoftirqd

当系统被软中断淹没的时候，即出现了大量软中断，内核使用 per-process 的内核线程（每个处理器一个线程）来帮助处理软中断。

内核会在几个地方处理软中断，而在中断处理程序返回时是最常见的处理软中断的地方。软中断可能被高频触发（比如在网络负载非常高的时候），这种情况就可能导致用户进程无法获得足够的处理器时间（饥饿）。  

一种解决方案是优先保证软中断的执行，即每次__do_softirq()处理软中断时，在返回前都先检查是否有新的软中断被触发，如果有，立刻继续处理新触发的软中断，不断重复此过程，直到没有新的软中断被触发，这样的结果就是在高负载时发生用户进程饥饿。  

另一个解决方案是每次执行__do_softirq()时不管新触发的软中断，这样的后果是新触发的软中断只有等到下次内核处理软中断的时候才能被执行，而可能性最大的情况就是等到下次中断（硬件中断，因为中断处理程序返回时是最常见的处理软中断的地方），这意味可能要等一段时间。而在一个空闲的系统中，显然立刻处理新触发的软中断才是一个更好的做法。

上面的解决方案都有各自的缺点，Linux的采用的解决方案是这样的：

__do_softirq() 在返回前会查看是否有新的软中断被触发了，如果有，就重新遍历软中断数组，并执行新触发的软中断，__do_softirq() 维持了一个计数器，每次重新遍历软中断数组，计数器就会减1，如果计数器变为0，那么就唤醒内核线程ksoftirqd来负责处理软中断，__do_softirq()直接返回。（也就是说，__do_softirq() 会在返回前，尝试几次尽可能清理干净软中断，尝试几次还是清理不完，就可以认定此时软中断量比较大，那就让 ksoftirqd 来吧，我不管了）

ksoftirqd使用最低的优先级（nice值是19），这样就不会抢占其他重要的任务了。这种方案在有大量软中断时，既能保证用户程序不会因为大量的软中断而出现饥饿，也可以确保过量的软中断最终都能够被处理。在系统空闲时，软中断能够被迅速处理，因为系统空闲，所以内核线程softirqd会马上被调度。

每个处理器都有一个对应的ksoftirqd，命名方式是ksoftirqd/n，其中n是处理器的编号。比如在一个双处理器的机器上，就会有两个ksoftirqd，分别为ksoftirqd/0和ksoftirqd/1。ksoftirqd被初始化后就会执行类似下面的死循环：

```c
/*
 * ksoftirqd 如果发现有被触发的软中断，就会调用do_softirq()来处理软中断，
 * 每次执行完 do_softirq()，ksoftirqd 都会检查是否需要调度，如果需要，
 * 就调用 schedule()来进行调度，让其他更重要的任务先执行。
 * 如果发现有新的软中断被触发，就会再次执行 do_sofirq() 。
 * 重复上述过程直到系统中没有被触发的软中断后， ksoftirqd 将自己标记为 
 * TASK_INTERRUPTIBLE ，进入睡眠状态，等待下次 do_softirq() 发现有过量的
 * 软中断（通过计时器值判断）时将其唤醒。
*/

for (;;) {
    if (!softirq_pending(cpu))      // 看看有没有 bit mask 置 1 ，也就是看看有没有软中断来了
        schedule();     // 没有的话我就让出自己，把时间让给别人调度

    set_current_state(TASK_RUNNING);

    while (softirq_pending(cpu)) {
        do_softirq();
        if (need_resched())
            schedule();
    }

    set_current_state(TASK_INTERRUPTIBLE);
}
```