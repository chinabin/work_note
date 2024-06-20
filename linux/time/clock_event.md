# 0x00. 导读

# 0x01. 简介

来自系统定时的需求（即 timer）。即从当前时间点开始，到xxx纳秒之后通知我做某些事情。

## 1.1 世说新语

本节内容来自 [Linux时间子系统之（十三）：Tick Device layer综述](http://www.wowotech.net/timer_subsystem/tick-device-layer.html)

### 1.1.1 什么是 tick ？  

要理解什么是 tick，首先要理解 OS kernel 是如何运作的。系统中有很多日常性的事情需要处理，例如：

－－－更新系统时间

－－－处理低精度 timer

－－－处理正在运行进程的时间片信息

系统在处理这些事情的时候使用了轮询的方式，也就是说按照固定的频率去做这些操作。这时候就需要 HW(HardWare) 的协助，一般而言，硬件会有 HW timer（称之 system timer ）可以周期性的 trigger interrupt ，让系统去处理上述的日常性事务。每次 timer 中断到来的时候，内核的各个模块就知道，一个固定的时间片已经过去。对于日常生活， tick 这个概念是和钟表关联的：钟表会发出周期性的滴答的声音，这个声音被称为 **tick**. CPU 和 OS kernel 扩展了这个概念：周期性产生的 timer 中断事件被称为 **tick**, 而能够产生 tick 的设备就称为 **tick device**.

**tick event device = tick device + mode(Periodic or oneshot)**

不同类型的 tick device 有不同的 **event handler**:
- 对于 periodic 类型的 tick device, 其 clock event device 的 event handler 是
    - tick_handle_periodic （没有配置高精度 timer）
    - 或者 hrtimer_interrupt （配置了高精度 timer）。
- 对于 one-shot 类型的 tick 设备，其 clock event device 的 event handler 是
    - hrtimer_interrupt （配置了高精度 timer）
    - 或者 tick_nohz_handler （没有配置高精度 timer）。

Tick Device 模块负责管理系统中的所有的 tick 设备，在 SMP 环境下，每一个 CPU 都自己的 tick device ，这些 tick device 中有一个被选择做 global tick device ，该 device 负责维护整个系统的 jiffies 以及更新哪些基于 jiffies 进行的全系统统计信息。

# 0x02. clock_event_device

clock_event_device 结构，它代表着一个时钟硬件设备，该设备就好像是一个具有事件触发能力（通常就是指中断）的 clocksource ，它不停地计数，当计数值达到预先编程设定的数值那一刻，会引发一个时钟事件中断，继而触发该设备的事件处理回调函数，以完成对时钟事件的处理。

```c
struct clock_event_device {
    void            (*event_handler)(struct clock_event_device *);                  // 回调函数指针
    int         (*set_next_event)(unsigned long evt, struct clock_event_device *);  // 设置下一次时间触发时间的函数指针，参数类型为差值
    int         (*set_next_ktime)(ktime_t expires, struct clock_event_device *);    // 设置下一次时间触发时间的函数指针，参数类型为 ktime
    ktime_t         next_event;
    u64         max_delta_ns;                                                       // 可设置的最大时间差
    u64         min_delta_ns;                                                       // 可设置的最小时间差
    u32         mult;                                                               // 用于 cycle 和 ns 的转换
    u32         shift;
    enum clock_event_state  state_use_accessors;
    unsigned int        features;
    unsigned long       retries;

    int         (*set_state_periodic)(struct clock_event_device *);
    int         (*set_state_oneshot)(struct clock_event_device *);
    int         (*set_state_oneshot_stopped)(struct clock_event_device *);
    int         (*set_state_shutdown)(struct clock_event_device *);
    int         (*tick_resume)(struct clock_event_device *);

    void            (*broadcast)(const struct cpumask *mask);
    void            (*suspend)(struct clock_event_device *);
    void            (*resume)(struct clock_event_device *);
    unsigned long       min_delta_ticks;
    unsigned long       max_delta_ticks;

    const char      *name;
    int         rating;                                                             // 优先级
    int         irq;
    int         bound_on;
    const struct cpumask    *cpumask;
    struct list_head    list;                                                       // 用来加入到 clockevent_devices 链表
    struct module       *owner;
} ____cacheline_aligned;
```

## 2.1 clock event device

clocksource 不能被编程，没有产生事件的能力，它主要被用于 timekeeper 来实现对真实时间进行精确的统计，而 clock_event_device 则是可编程的，它可以工作在周期触发或单次触发模式，系统可以对它进行编程，以确定下一次事件触发的时间， clock_event_device 主要用于实现普通定时器和高精度定时器，同时也用于产生 tick 事件，供给进程调度子系统使用。

与 clocksource 一样，系统中可以存在多个 clock_event_device ，系统会根据它们的精度和能力，选择合适的 clock_event_device 对系统提供时钟事件服务。在 smp 系统中，为了减少处理器间的通信开销，基本上每个 cpu 都会具备一个属于自己的本地 clock_event_device ，独立地为该 cpu 提供时钟事件服务，smp 中的每个 cpu 基于本地的 clock_event_device ，建立自己的 tick_device ，普通定时器和高精度定时器。

tick_device 是基于 clock_event_device 的进一步封装，用于代替原有的时钟滴答中断，给内核提供 tick 事件，以完成进程的调度和进程信息统计，负载平衡和时间更新等操作。

## 2.2 tick device

处理器采用时钟定时器来周期性地提供系统脉搏。时钟中断是普通外设中断的一种。调度器利用时钟中断来定时检测当前正在运行的线程是否需要调度。提供时钟中断的设备就是 tick device 。

如今在多核架构下，每个 CPU 形成了自己的一个小系统，有自己的调度、自己的进程统计等，这个小系统拥有自己的 tick device ，而且每个 CPU 上 tick device 是唯一的， tick device 可以工作在 periodic mode 或者 one shot mode ，这是和系统配置有关。因此，整个系统中，在 tick device layer ，有多少个 cpu ，就会有多少个 tick device ，称为 local tick device 。当然，有些事情（例如整个系统的负荷计算）不适合在 local tick 驱动下进行，因此，**所有的 local tick device 中会有一个被选择做 global tick device ，该 device 负责维护整个系统的 jiffies ，更新 wall clock ，计算全局负荷什么的**。

```c
/*tick device可以工作在两种模式下，一种是周期性tick模式，另外一种是one shot模式。*/
enum tick_device_mode {
	TICKDEV_MODE_PERIODIC,
	TICKDEV_MODE_ONESHOT,/*one shot模式主要和tickless系统以及高精度timer有关*/
};
struct tick_device {
	struct clock_event_device *evtdev;
	enum tick_device_mode mode;
};

```