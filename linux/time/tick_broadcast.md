# 0x00. 导读

# 0x01. 简介

[Linux时间子系统之（十四）：tick broadcast framework](http://www.wowotech.net/timer_subsystem/tick-broadcast-framework.html)

对于 Linux 内核中的每个子系统来说，电源管理工作变得越来越重要。内核中最为成熟的电源管理机制之一是 cpuidle 框架，它会将空闲的处理器置于休眠状态，直到它们有新的工作要做。这些睡眠状态被称为 “C 状态”（C-states）或处理器操作状态。C-state 睡眠的程度越深，节能的效果越好。

但是，当处理器进入某些深度 C-state 时，会出现一个有趣的问题。一般情况下为执行新的任务，睡眠态的处理器通常由各自的本地定时器（local timer，本文这里特指 Advanced Programmable Interrupt Controller （简称 APIC）的 local timer ）负责唤醒，可是一旦处理器进入深度 C-state 后这些定时器也会停止工作，那么谁来及时唤醒处理器呢？这就是 “时钟广播框架” （“tick broadcast framework”，）所要解决的问题。它通过指定一个不受 C-state 影响的时钟设备来负责唤醒所有进入深度 C-state 的处理器。

# 0x02. 

在一个系统处在空闲或半空闲的情况下，可能会有不止一个处理器进入深度睡眠状态以致其本地定时器也不再工作。这些处理器的唤醒时间可能并不相同。考虑到硬件定时器仅仅是一个时钟设备，除了在设定的时间触发中断外并不能跟踪更多的信息，那么如何利用它来唤醒这些处理器呢？内核中的 tick broadcast framework 提供了必要的措施，以便在正确的时间唤醒这些处理器。

## 2.1 前置知识

内核基于超时（timeouts）的概念来跟踪需要推迟执行的任务的时间。在硬件上，超时是利用一种特殊的时钟设备（也叫定时器（timers））实现的，定时器能够在指定的时间触发中断。在内核中，这样的设备被称为“时钟事件”（”clock event”）设备。每个处理器上有一个本地的时钟事件设备，如果我们有一个任务需要在以后的某个时刻运行，我们可以通过编程把这个期望时间设置给该设备，它就会在期望的时间点触发中断从而通知内核在处理器上调度执行该任务。这些本地时钟事件设备也可以被设定为定期触发中断以便运行周期性的事务，譬如更新 jiffies 值，检查是否可以执行任务调度等。因此这些定时器被称为 “tick devices”，在内核中对应的结构体类型是 struct tick_device。

对应每个处理器（per-CPU）的本地定时器（local timer）对象，内核定义了一个 tick_device 类型的变量 tick_cpu_device。内核为每个处理器的本地定时器维护下一次触发中断的时间。具体地，这个时间值保存在 tick_cpu_device->evtdev->next_event，其中 evtdev 是 “tick devices” 所对应的时钟事件设备的一个实例。

当处理器进入某些深度睡眠状态后，会需要一个外部时钟事件设备（相对于处理器内部的本地定时器）来代替本地定时器运行，它也是一种 “tick devices”，但通常不利用它跟踪特定处理器的事件。内核中将该设备定义为 tick_broadcast_device。

## 2.2

在内核初始化期间，系统中的每个定时器设备都将自己注册为一个 tick_device 类型的实例。而且每个定时器都具有一些属性，通过相应的标志位来定义。我们特别感兴趣的一个属性对应 `CLOCK_EVT_FEAT_C3STOP` 标志位。拥有此属性的定时器在处理器进入 C3 级别的睡眠态后会停止工作。要注意的是 C3 态是一个特定于 x86 架构的概念，内核只是借用这个术语表示定时器在某种深度睡眠状态下会停止工作。

任何没有设置 CLOCK_EVT_FEAT_C3STOP 属性标志的定时器都是 tick_broadcast_device 的潜在候选者。如果在某种体系架构上，一旦处理器进入深度睡眠后其本地定时器会停止工作，则该定时器需要在属性上设置该标志位，从而避免被内核选为 tick_broadcast_device。对于 x86 架构，在处理器外部存在一种设备，叫做“高精度事件定时器”（High Precision Event Timer，简称 `HPET`），它就很适合作为 tick_broadcast_device。原因是 HPET 放置在处理器的外部，管理处理器睡眠的电源模块影响不了它。自然，它的属性中不会设置 CLOCK_EVT_FEAT_C3STOP 标志位，可以被选择作为 tick_broadcast_device。

现在我们回到 tick broadcast framework，来看看对于进入睡眠态的处理器，当它的本地定时器不再工作后，我们该如何跟踪并唤醒它们。处理器在进入深度睡眠之前，它会调用框架提供的接口，将该处理器添加到需要唤醒的处理器列表中; 具体来说，该列表的实现方式是一个 “广播掩码”（“broadcast mask”），每一个处理器占据该掩码的一个位（所谓的添加到列表中就是对掩码中对应的位置 1）。

然后内核会进行检查，看看这个处理器需要被唤醒的时间是否在 tick_broadcast_device 当前设置的下一次事件时间之前。如果是这样，tick_broadcast_device 会更新自身的下一次触发中断的时间以反映该新值，并对硬件上对应的外部定时器进行设置。最后在处理器进入深度睡眠之前将其自身的 tick_cpu_device 设置为 CLOCK_EVT_MODE_SHUTDOWN 模式，表示该处理器的本地定时器已停止工作。

每当有一个处理器要进入深度睡眠时，都会重复上述步骤，并确保 tick_broadcast_device 所维护的下一次中断触发时间一定是所有需要被唤醒的处理器中最早的那一个。

每个 tick device 都需要实现一个 broadcast 的回调函数接口。该接口的实现是体系架构相关的，它封装了向一组处理器发送“核间中断”（inter-processor interrupts，简称 IPI）的逻辑。换句话说，每个本地定时器都具备该回调函数。内核会选择一个本地定时器，并调用其 broadcast() 方法向所有睡眠中的处理器发送 IPI。正是基于这种将唤醒中断发送到一组处理器的行为，这个框架被称为 “广播” 框架。

响应 IPI 的中断处理程序和响应本地定时器中断的处理程序是同一个函数，这么做的好处在于，从处于深度睡眠的处理器的角度来看，唤醒它的中断就好像依然是来自其自身的本地定时器，换句话说，它根本就不会察觉到本地定时器曾经停止过；由于它们睡眠之前和唤醒之后看到的状态是相同的，就好像什么事情也没有发生过一样。

在处理 IPI 的过程中，处理器将调用 tick broad framework 提供的函数，在函数处理中会将它们从 “广播掩码” 中移除，因为此时已经可以确认它们已收到 IPI 并已被唤醒。然后，它们各自对应的 tick 设备也会退出 CLOCK_EVT_MODE_SHUTDOWN 模式，从此恢复正常运行。