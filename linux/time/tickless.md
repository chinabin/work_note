# 0x00. 导读

# 0x01. 简介

peroid tick 会按照固定周期触发中断，唤醒 cpu 来处理 tick （处理 timer 、处理调度等等）。如果没有到期 timer 需要处理，或者没有任务需要切换，那么这个 tick 中断实际是可以避免的，从而节省功耗，并提高性能。

# 0x02. 模式

tick 有三种工作模式：`periodic`，`nohz idle`，`nohz full`

1. periodic

    周期性 tick, `CONFIG_NO_HZ=n, CONFIG_HZ_PERIODIC=y`

2. nohz idle

    CPU idle 时不产生 tick(默认模式), `CONFIG_NO_HZ=y, CONFIG_NO_HZ_IDLE=y`

    dyntick-idle, in dyntick-idle mode, in nohz mode, or running tickless 都指这种模式

    在 CONFIG_NO_HZ_IDLE 开启的情况下，还可以通过 nohz=on/off 在 bootargs 中动态开关 dyntick 。

3. nohz full 

    CPU idle 或者 CPU 只有一个任务运行时，不产生 tick, `CONFIG_NO_HZ=y, CONFIG_NO_HZ_FULL=y`

    被设置为 nohz full 的 CPU 叫做 `adaptive-ticks CPUs`.

    可以通过 bootargs nohz_full=1,6-8 来设置启用 nohz full 的 CPU, 不能将 cpu0 设置为 nohz full

# 0x03. nohz full 

[LWN 549580: 3.10 版本开始支持（接近）完全无周期时钟（full tickless）](https://tinylab.org/lwn-549580/)

从 3.10 版本开始，开始支持新的 full NOHZ 模式。

在 3.10 版本的内核中，CONFIG_NO_HZ 选项被分解为三个选项：

- CONFIG_HZ_PERIODIC: 传统模式，时钟中断始终运行。
- CONFIG_NO_HZ_IDLE: （默认开启）处理器空闲时会尝试禁止时钟中断，对应早期版本中的 CONFIG_NO_HZ 选项。
- CONFIG_NO_HZ_FULL: 将启用 “完全” 的无时钟中断（”full” tickless）模式。

内核启动时可以在命令行中通过 `nohz_full=` 参数指定需要对哪些处理器尝试在运行时关闭时钟定时器中断。但不能对用于引导内核的处理器使用此模式，因为至少需要保留一个处理器继续接收中断并执行必要的日常管理事务。`CONFIG_NO_HZ_FULL_ALL` 选项默认情况下会针对所有的 CPU（引导内核的 CPU 除外）启用 Full tickless 模式；但如果在命令行中指定了 nohz_full= 参数则该选项会被覆盖（overridden）。目前不支持内核启动后重新指定启用 Full tickless 模式的处理器；一来实现这个功能所涉及的改动量太大，二来似乎也没有迫切的需求需要这种能力。

**实际上，即使只有一个可运行的任务，也不能完全停止时钟中断，为了保证调度器可以正常工作，时钟中断至少需要每秒发生一次。但是从每秒 1000 次的频率下降到每秒 1 次俨然是很大的进步了。**