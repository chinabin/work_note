# 0x00. 导读

[【时间子系统】二、计时原理－timekeeper与clocksource](https://rootw.github.io/2018/01/%E8%AE%A1%E6%97%B6/)

# 0x01. 简介

时钟源 (clocksource) 只能用来查询时间，就好像一个手表一样，当你想查询时间时看一下，知道现在几点了。但如果你想设定一个闹钟，让它在特定的时间点提醒你，那么就需要时钟事件设备 (clock_event_device)。此类设备可以用来注册事件，让它们在未来的特定时间点被触发事件。


查看支持的时间源  
`/sys/devices/system/clocksource/clocksource0/available_clocksource`

查看当前时间源  
`/sys/devices/system/clocksource/clocksource0/current_clocksource`

# 0x02. 详解

clock source 用于为 linux 内核提供一个时间基线，如果你用 linux 的 date 命令获取当前时间，内核会读取当前的 clock source ，转换并返回合适的时间单位给用户空间，换句话说系统需要知道现在是xx年xx月xx日xx时xx分xx秒xx纳秒。

在硬件层，它通常实现为一个由固定时钟频率驱动的计数器，计数器只能单调地增加，直到溢出为止。时钟源是内核计时的基础，例如，系统启动时，内核通过硬件 RTC 获得当前时间，在这以后，在大多数情况下，内核通过选定的时钟源更新实时时间信息（墙上时间），而不再读取 RTC 的时间。

clocksource 提供了对不同软硬件时钟的抽象。可以理解为时间源，为 kernel 提供当前时间。

## 2.1 clocksource 结构体

```c
struct clocksource {
    cycle_t (*read)(struct clocksource *cs);            // 指向读取时钟的函数
    cycle_t mask;                                       // 能够表示的 cycle 上限，通常是 32/64 位的全 f，做与操作可以避免对 overflow 进行专门处理
    u32 mult;                                           // 将时间源的计数单位 (cycle_t) 转换为 ns
    u32 shift;                                          // 换算公式为 (cycles * mult) >> shift
    u64 max_idle_ns;                                    // 允许的最大空闲时间，单位 ns。当设置 CONFIG_NO_HZ 时，使用动态 tick，不限制 kernel 的睡眠时间，需要进行限制
    u32 maxadj;                                         // 允许的最大调整值，避免转换时 overflow
#ifdef CONFIG_ARCH_CLOCKSOURCE_DATA
    struct arch_clocksource_data archdata;              // 架构专有(目前只有 x86 和 ia64)。
#endif
    u64 max_cycles;                                     // 设置了 cycle 上限，避免换算时溢出
    const char *name;                                   // 时间源名称
    struct list_head list;                              // 注册了该时间源?
    int rating;                                         // 精度
    int (*enable)(struct clocksource *cs);              // 启用时间源函数
    void (*disable)(struct clocksource *cs);            // 停用时间源函数
    unsigned long flags;
    void (*suspend)(struct clocksource *cs);            // 暂停时间源函数
    void (*resume)(struct clocksource *cs);             // 恢复时间源函数

    /* private: */
#ifdef CONFIG_CLOCKSOURCE_WATCHDOG                      // 用于监控时间源，校验时间是否准确
    /* Watchdog related data, used by the framework */
    struct list_head wd_list;
    cycle_t cs_last;
    cycle_t wd_last;
#endif
    struct module *owner;                               // 指向拥有该时间源的内核模块
}; 
```

几个重要字段解释：

1. rating

    同一个设备下，可以有多个时钟源，每个时钟源的精度由驱动它的时钟频率决定，比如一个由 10MHz 时钟驱动的时钟源，他的精度就是 100ns。clocksource结构中有一个 rating 字段，代表着该时钟源的精度范围，它的取值范围如下：
    ```
    1--99： 不适合于用作实际的时钟源，只用于启动过程或用于测试；
    100--199：基本可用，可用作真实的时钟源，但不推荐；
    200--299：精度较好，可用作真实的时钟源；
    300--399：很好，精确的时钟源；
    400--499：理想的时钟源，如有可能就必须选择它作为时钟源；
    ```

2. read

    时钟源本身不会产生中断，要获得时钟源的当前计数，只能通过**主动调用**它的 read 回调函数来获得当前的计数值，注意这里只能获得计数值（意思就是一个数字，不同硬件可能带有不同的单位），要转换成我们熟悉的单位，例如纳秒，必须要借助 clocksource 的 `mult` 和 `shift` 字段辅助进行转换计算。

# 0x03. 常见 clocksource

clocksource 会向上层 timekeeping 提供服务，只会有一个精度最高的 clocksource 被选择，用来提供服务(cur_clocksource)。

系统启动初始化阶段，会先使用 jiffies 作为默认 clocksource ，启动过程中，如果有新的 clocksource 注册进来，则会调用 clocksource_select ，重新选择出高精度 clocksource ，然后调用 timekeeping_notify 通知 timekeeping 系统更新 clocksource。

timekeeping 系统使用的 clocksource 中，除了 cur_clocksource ，还会有一个 suspend_clocksource ，这个 clocksource 可以在系统下电后继续运行，专门用于记录睡眠时间。

## 3.1 jiffies

英语中 jiffy 表示 a moment，即一瞬间。在 Linux 中作为软件维护的时钟。表示一小段短暂而不确定的时间。

属于低精度时间源，因为没有 `CLOCK_SOURCE_VALID_FOR_HRES` 的 flag，所以不会出现在 available_clocksource 中。

```c
static struct clocksource clocksource_jiffies = {
    .name       = "jiffies",
    .rating     = 1, /* lowest valid rating*/                               // 优先级最低
    .read       = jiffies_read,                                             // 读时返回 jiffies
    .mask       = CLOCKSOURCE_MASK(32),
    .mult       = NSEC_PER_JIFFY << JIFFIES_SHIFT, /* details above */
    .shift      = JIFFIES_SHIFT,                                            // NSEC_PER_JIFFY 和 JIFFIES_SHIFT 由 CONFIG_HZ 决定
    .max_cycles = 10,
};
```

## 3.2 RTC(Real Time Clock)

主板上的晶振及相关电路组成的时钟，往往被集成到南桥芯片内，依靠主板上的电池供电。在 IRQ8 上周期性的产生中断，频率范围是 2Hz ~ 8192Hz。

通过 CMOS 进行读写，开机时读取，需要时写入。

## 3.3 PIT(Programmalbe Interval Timer)

通过 8253/8254 时钟芯片实现，经过适当编程后，能够周期性地发出时钟中断。频率为 1193182 Hz (PIT_TICK_RATE)。

定义为：
```c
static struct clocksource i8253_cs = {
    .name       = "pit",
    .rating     = 110,
    .read       = i8253_read,
    .mask       = CLOCKSOURCE_MASK(32),
};
```
由于精度问题，在有 HPET 的情况下不会使用 PIT。

## 3.4 acpi_pm(ACPI power management timer)

acpi_pm 时钟源的优点是其频率不会根据 power management code 而改变，但由于它以 3.58MHz 运行（每 279 ns 一次滴答）。

几乎所有 ACPI-based 的主板上都会有该设备。频率为 3.579545 MHz
```c
static struct clocksource clocksource_acpi_pm = {
    .name       = "acpi_pm",
    .rating     = 200,
    .read       = acpi_pm_read,
    .mask       = (cycle_t)ACPI_PM_MASK,
    .flags      = CLOCK_SOURCE_IS_CONTINUOUS,
};
```

## 3.5 HPET(High Precision Event Timer)

提供了更高的时钟频率 (10MHz+) 以及更宽的计数范围(64bit)。通过 APIC 发现，利用 MMIO 进行编程。集成到南桥中。

包含一个 64bit 的主计数器 (up-counter) 计数器，频率至少为 10MHz，一堆 (最多 256 个) 32 /64 bit 比较器(comparators)。当一个计数器(相关的位) 等于比较器 (最低有效位) 时，产生中断。

定义为：
```c
static struct clocksource clocksource_hpet = {
    .name       ="hpet",
    .rating     = 250,
    .read       = read_hpet,
    .mask       = CLOCKSOURCE_MASK(64),
    .flags      = CLOCK_SOURCE_IS_CONTINUOUS,
};
```

## 3.6 TSC(Time Stamp Counter)

Pentium 开始提供的 64 位寄存器。每次外部振荡器产生信号时 (每个 CPU 时钟周期) 加 1，因此频率依赖于 CPU 频率(Intel 手册上说相等)，如果 CPU 频率为 400MHz 则每 2.5 ns 加 1。

为了使用 TSC，Linux 在系统初始化的时候必须通过调用 calibrate_tsc(native_calibrate_tsc) 来确定时钟的频率 (编译时无法确定，因为 kernel 可能运行在不同于编译机的其他 CPU 上)。一般用法是在一定时间内(需要通过其他时间源，如 hpet) 执行两次，记录 start 和 end 的时间戳，同时通过 rdtsc 读取 start 和 end 时 TSC counter，通过 (end - start time) / (end - start counter) 算出期间 CPU 实际频率。

但在多核时代下，由于不能保证同一块主板上每个核的同步，CPU 变频和指令乱序执行导致 TSC 几乎不可能取到准确的时间，但新式 CPU 中支持频率不变的 constant TSC。

定义为：
```c
static struct clocksource clocksource_tsc = {
    .name                   = "tsc",
    .rating                 = 300,
    .read                   = read_tsc,
    .mask                   = CLOCKSOURCE_MASK(64),
    .flags                  = CLOCK_SOURCE_IS_CONTINUOUS |
                  CLOCK_SOURCE_MUST_VERIFY,
    .archdata               = { .vclock_mode = VCLOCK_TSC },
};
```