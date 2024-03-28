# 0x00. 导读

# 0x01. 简介

```c
https://github.com/torvalds/linux/blob/v3.10/init/main.c#L471

asmlinkage __visible void __init start_kernel(void)
{
    ...
    tick_init();        // 初始化 tick 系统
    init_timers();      // 初始化低精度定期器系统
    hrtimers_init();    // 初始化高精度定期器系统
    softirq_init();     // 初始化软中断
    timekeeping_init(); // 填充timekeeper数据结构
    time_init();        // 设置 late_time_init 为 x86_late_time_init
    ...
}
```

## 1.1 tick_init

tick 系统初始化。

```c
/**
 * tick_init - initialize the tick control
 */
void __init tick_init(void)
{
	tick_broadcast_init();
	tick_nohz_init();
}
```

## 1.2 init_timers 

低精度定时器（timer）系统初始化。

```c
void __init init_timers(void)
{
	init_timer_cpus();              // 初始化每个 CPU 的所有的 timer_base
	posix_cputimers_init_work();
	open_softirq(TIMER_SOFTIRQ, run_timer_softirq);     // 为 TIMER_SOFTIRQ 中断注册了处理函数 run_timer_softirq 
}
```

## 1.3 hrtimers_init

## 1.4 timekeeping_init

初始化时钟守护者，其实就是时钟。

对于用户来说，我们感知的是真实世界的真实时间，也就是墙上时间（wall time），clocksource 只能提供一个按给定频率不停递增的周期计数，需要把它和真实的墙上时间相关联。

`struct timekeeper` 维护了 monotonic time, raw monotonic time, xtime ：
```c
struct timekeeper {
    struct tk_read_base tkr_mono;               // 维护 CLOCK_MONOTONIC
    struct tk_read_base tkr_raw;                // 维护 CLOCK_MONOTONIC_RAW
    u64         xtime_sec;                      // 当前 CLOCK_REALTIME，单位秒
    unsigned long       ktime_sec;              // 当前 CLOCK_MONOTONIC，单位秒
    struct timespec64   wall_to_monotonic;      // CLOCK_REALTIME 和 CLOCK_MONOTONIC 的差值
    ktime_t         offs_real;                  // CLOCK_MONOTONIC 和 CLOCK_REALTIME 的差值
    ktime_t         offs_boot;                  // CLOCK_MONOTONIC 和 boot time 的差值
    ktime_t         offs_tai;                   // CLOCK_MONOTONIC 和 CLOCK_TAI 的差值
    s32         tai_offset;                     // UTC 和 TAI 的差值，单位秒
    unsigned int        clock_was_set_seq;
    u8          cs_was_changed_seq;
    ktime_t         next_leap_ktime;
    struct timespec64   raw_time;               // raw monotonic time

    /* The following members are for timekeeping internal use */
    cycle_t         cycle_interval;             // 一个 NTP interval 的 cycle 数
    u64         xtime_interval;                 // 一个 NTP interval 的 ns 数
    s64         xtime_remainder;
    u64         raw_interval;
    /* The ntp_tick_length() value currently being used.
     * This cached copy ensures we consistently apply the tick
     * length for an entire tick, as ntp_tick_length may change
     * mid-tick, and we don't want to apply that new value to
     * the tick in progress.
     */
    u64         ntp_tick;
    /* Difference between accumulated time and NTP time in ntp
     * shifted nano seconds. */
    s64         ntp_error;
    u32         ntp_error_shift;
    u32         ntp_err_mult;
#ifdef CONFIG_DEBUG_TIMEKEEPING
    long            last_warning;
    /*
     * These simple flag variables are managed
     * without locks, which is racy, but they are
     * ok since we don't really care about being
     * super precise about how many events were
     * seen, just that a problem was observed.
     */
    int         underflow_seen;
    int         overflow_seen;
#endif
};
```

```c
void __init timekeeping_init(void)
{
    struct timekeeper *tk = &tk_core.timekeeper;
    struct clocksource *clock;
    unsigned long flags;
    struct timespec64 now, boot, tmp;

    // 获取持久化时间，在 x86 下是调用 x86_platform.get_wallclock (mach_get_cmos_time)，即读取 RTC
    read_persistent_clock64(&now);
    if (!timespec64_valid_strict(&now)) {
        pr_warn("WARNING: Persistent clock returned invalid value!\n"
            "         Check your CMOS/BIOS settings.\n");
        now.tv_sec = 0;
        now.tv_nsec = 0;
    } else if (now.tv_sec || now.tv_nsec)
        persistent_clock_exists = true;

    // x86 下为 0
    read_boot_clock64(&boot);
    if (!timespec64_valid_strict(&boot)) {
        pr_warn("WARNING: Boot clock returned invalid value!\n"
            "         Check your CMOS/BIOS settings.\n");
        boot.tv_sec = 0;
        boot.tv_nsec = 0;
    }

    raw_spin_lock_irqsave(&timekeeper_lock, flags);
    write_seqcount_begin(&tk_core.seq);
    // 初始化 NTP，重置相关变量
    ntp_init();

    // 获取默认的时钟源，即 clocksource_jiffies
    clock = clocksource_default_clock();
    if (clock->enable)
        clock->enable(clock);
    // 将 timekeeper 和 clocksource_jiffies 关联起来，即使用 clocksource_jiffies 来作为时钟源
    tk_setup_internals(tk, clock);

    // 利用 RTC 读到的时间来设置 xtime / raw time
    tk_set_xtime(tk, &now);
    tk->raw_time.tv_sec = 0;
    tk->raw_time.tv_nsec = 0;
    if (boot.tv_sec == 0 && boot.tv_nsec == 0)
        boot = tk_xtime(tk);

    // 将自启动以来的时间作为 monotonic time 和 xtime 的差值(wall_to_monotonic)，这里为 0？
    set_normalized_timespec64(&tmp, -boot.tv_sec, -boot.tv_nsec);
    tk_set_wall_to_mono(tk, tmp);

    timekeeping_update(tk, TK_MIRROR | TK_CLOCK_WAS_SET);

    write_seqcount_end(&tk_core.seq);
    raw_spin_unlock_irqrestore(&timekeeper_lock, flags);
}
```