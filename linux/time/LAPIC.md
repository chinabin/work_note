# 0x00. 导读

# 0x01. 简介

# 0x02. 流程

## 简洁流程

1. [Re: [nohz_full/apic] multiple timer interrupts a second](https://lkml.indiana.edu/hypermail/linux/kernel/1711.0/03017.html)  
2. [[tip:timers/urgent] x86/timers/apic: Fix imprecise timer interrupts by eliminating TSC clockevents frequency roundoff error](https://www.uwsg.indiana.edu/hypermail/linux/kernel/1608.1/02336.html)  
3. [fix](https://lore.kernel.org/lkml/20160812194300.GA30748@gmail.com/)

```
lapic timer 的处理函数入口为 apic_timer_interrupt -> smp_apic_timer_interrupt -> local_apic_timer_interrupt -> per cpu 变量 lapic_events 成员 event_handler, i.e. hrtimer_interrupt(高精度模式是这个) -> ... -> tick_sched_timer -> tick_sched_handle -> update_process_times
```

## 详细流程

[源码](https://elixir.bootlin.com/linux/v3.10/C/ident/hrtimer_interrupt)

[【时间子系统】三、时钟中断－定时基础](https://rootw.github.io/2018/02/%E6%97%B6%E9%92%9F%E4%B8%AD%E6%96%AD/)  
[【时间子系统】五、高精度定时器](https://rootw.github.io/2018/02/%E9%AB%98%E7%B2%BE%E5%BA%A6%E5%AE%9A%E6%97%B6%E5%99%A8/)