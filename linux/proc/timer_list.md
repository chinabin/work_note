# 0x00. 导读

# 0x01. 简介

打印 per_cpu 的 hrtimer_bases 信息以及基于此的 timer 列表，包括三种时钟 MONOTONIC/REALTIME/BOOTTIME ；以及 Broadcast Tick Device 和 Per CPU Tick Device 信息。

Per CPU TickDevice 是维护当前 CPU Tick 和 hrtimer 的基础，只有工作在 OneShot 模式下才能实现 hrtimer 。在 CPU 进入 cpuidle 后，Per CPU TickDevice 可能会被关闭，这时候可以打开 Broadcast Tick 作为唤醒源。超时后，就会将系统从cpuidle退出，进入正常工作模式打开 Per CPU TickDevice。

hrtimer有三种时钟基准，这三种时钟基准对应不同的时间获取函数，但主要是基于TimeKeeping的统计。

# 0x02. 

```
+       /proc/timer_list displays most everything about every kind
+       of timer, and some things about time too.
+
+
+          The contents of the data structures associated with each
+          clock on this CPU:
+           1 - clock ID: 0 == CLOCK_REALTIME, 1 == CLOCK_MONOTONIC
+           2 - base: kernel address of this clock's
+               hrtimer_clock_base structure.
+           3 - resolution: resolution of this clock.
+           4 - get_time: name of kernel function used to fetch
+               time from this clock.
+           5 - offset: difference, in nsecs, between this clock
+               and the reference clock for this clock.
+          Under each of these clocks is, in turn, a display of all
+          the active high resolution timers queued to that clock.
+          These are the lines beginning with '#' and are described
+          in detail later in this document.
+
+          The contents of per-CPU hrtimer data fields not
+          associated with a particular cpu clock (ie, shared by
+          both clocks or not associated with any clock).  These
+          are: expires_next, hres_active, nr_event, nohz_mode, all
+          things idle_*, tick_stopped, last_jiffies, next_jiffies.
+          The above are field names from 'struct tick_sched' and
+          'struct hrtimer_cpu_base', documentation for these may
+          be found in the kernel DocBook.
+
+          A display of low resolution (ie, jiffie) timer wheel
+          data.  These are prefixed by the lines:
+           1 - base: kernel virtual address of the timer wheel
+               data structure (struct tvec_base) for this cpu.
+           2 - running timer: kernel virtual address of the
+               expired timer being processed, NULL if none.
+           3 - timer_jiffies: what this wheel considers to
+               be the current time, will be == jiffies or
+           will lag it by a tick or two if it has not
+           caught up with the current time.
+          Also under this section is a display, one per line, of
+          each active jiffie timer queued to this CPU.  These are
+          the lines under an 'active jiffie timers' section that
+          begin with a number.
+
+       The third and final section describes each 'tick device'
+       known to the kernel.  A tick device is a piece of hardware
+       capable of generating periodic and/or one-shot interrupts
+       under software control, and thus is capable of generating
+       the interrupts needed to expire the various active timers
+       at their given expiration times.  Examples of tick devices:
+       hpet, pit, lapic.  All but the first two lines display
+       fields corresponding to structure elements from 'struct
+       clock_event_device', documentation for which can be found
+       in the kernel Docbook. The first two lines are:
+         1 - mode: 0 == periodic timer, 1 == one-shot timer
+         2 - is 'Per CPU device' or is 'Broadcast device'
+
+       Hires Timer Layout
+       ------------------
+       High-resolution timers are displayed on lines that begin
+       with a '#' and always appear under one of the many sections
+       labeled 'active timers'.  There is an 'active timers'
+       section for every CPU and every clock.
+
+       The fields of a hrtimer, spread out over two lines, are:
+
+       line 1 fields:
+         1 - unique hrtimer index (#0, #1, #2, etc)
+         2 - kernel address of the hrtimer data structure
+             in question
+         3 - function to be called when timer expires
+         4 - timer state (eg, S:01), avail states, OR-able:
+             0 - inactive
+             1 - enqueued
+             2 - callback
+             4 - pending
+             8 - migrate
+         5 - function which created the timer
+         6 - process name & pid which created the timer
+
+       line 2 fields:
+         1 - absolute expiration time, range format (start - end)
+         2 - relative expiration time, range format (start - end)
+
+       Lowres Timer Layout
+       -------------------
+       Low-resolution timers are displayed one-per-line under
+       sections labeled 'active jiffie timers'.  There is one such
+       section per CPU.  A lowres timer has the following fields:
+
+         1 - number of jiffies remaining until timer expires
+         2 - function to be called on expiration
+         3 - data value to be given to the above function on
+             expiration
+         4 - function which created this timer
+         5 - name & pid of the process that created this timer
```