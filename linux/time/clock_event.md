# 0x00. 导读

# 0x01. 简介

来自系统定时的需求（即 timer）。即从当前时间点开始，到xxx纳秒之后通知我做某些事情。

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

## 2.1 clock_event_device

clocksource 不能被编程，没有产生事件的能力，它主要被用于 timekeeper 来实现对真实时间进行精确的统计，而 clock_event_device 则是可编程的，它可以工作在周期触发或单次触发模式，系统可以对它进行编程，以确定下一次事件触发的时间， clock_event_device 主要用于实现普通定时器和高精度定时器，同时也用于产生 tick 事件，供给进程调度子系统使用。

与 clocksource 一样，系统中可以存在多个 clock_event_device ，系统会根据它们的精度和能力，选择合适的 clock_event_device 对系统提供时钟事件服务。在 smp 系统中，为了减少处理器间的通信开销，基本上每个 cpu 都会具备一个属于自己的本地 clock_event_device ，独立地为该 cpu 提供时钟事件服务，smp 中的每个 cpu 基于本地的 clock_event_device ，建立自己的 tick_device ，普通定时器和高精度定时器。

tick_device 是基于 clock_event_device 的进一步封装，用于代替原有的时钟滴答中断，给内核提供 tick 事件，以完成进程的调度和进程信息统计，负载平衡和时间更新等操作。

## 2.2 tick device

处理器采用时钟定时器来周期性地提供系统脉搏。时钟中断是普通外设中断的一种。调度器利用时钟中断来定时检测当前正在运行的线程是否需要调度。提供时钟中断的设备就是 tick device 。

如今在多核架构下，每个 CPU 形成了自己的一个小系统，有自己的调度、自己的进程统计等，这个小系统拥有自己的 tick device ，而且每个 CPU 上 tick device 是唯一的， tick device 可以工作在 periodic mode 或者 one shot mode ，这是和系统配置有关。因此，整个系统中，在 tick device layer ，有多少个 cpu ，就会有多少个 tick device ，称为 local tick device 。当然，有些事情（例如整个系统的负荷计算）不适合在 local tick 驱动下进行，因此，所有的 local tick device 中会有一个被选择做 global tick device ，该 device 负责维护整个系统的 jiffies ，更新 wall clock ，计算全局负荷什么的。

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