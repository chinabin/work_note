# 0x00. 导读

[sysdevicessystemcpucpun-目录](https://arthurchiao.art/blog/linux-cpu-4-zh/#11-sysdevicessystemcpucpun-%E7%9B%AE%E5%BD%95)

/sys/devices/system/cpu/cpufreq 里面都是 policyx 目录

# 0x01. 简介

# 0x02. /sys/devices/system/cpu

```bash
# monitor 机器
$ ls -hl /sys/devices/system/cpu
drwxr-xr-x  9 root root    0 Jan 18 22:06 cpu0
drwxr-xr-x  9 root root    0 Jan 18 22:06 cpu1
drwxr-xr-x  9 root root    0 Jan 18 22:06 cpu2
drwxr-xr-x  9 root root    0 Jan 18 22:06 cpu3
drwxr-xr-x  9 root root    0 Jan 18 22:06 cpu4
drwxr-xr-x 42 root root    0 Jan 18 22:06 cpufreq
drwxr-xr-x  2 root root    0 Jan 18 22:06 cpuidle
drwxr-xr-x  2 root root    0 Jan 18 22:06 hotplug
drwxr-xr-x  2 root root    0 Jan 18 22:06 intel_pstate
-r--r--r--  1 root root 4.0K Jan 18 22:06 isolated
-r--r--r--  1 root root 4.0K Jan 18 22:06 kernel_max
drwxr-xr-x  2 root root    0 Jan 18 22:06 microcode
-r--r--r--  1 root root 4.0K Jan 18 22:06 modalias
-r--r--r--  1 root root 4.0K Jan 18 22:06 offline
-r--r--r--  1 root root 4.0K Jan 18 22:06 online
-r--r--r--  1 root root 4.0K Jan 18 22:06 possible
drwxr-xr-x  2 root root    0 Jan 18 22:06 power
-r--r--r--  1 root root 4.0K Jan 18 22:06 present
drwxr-xr-x  2 root root    0 Jan 18 22:06 smt
-rw-r--r--  1 root root 4.0K Jan 18 22:06 uevent
drwxr-xr-x  2 root root    0 Jan 18 22:06 vulnerabilities
```

- possible  
    代表主板支持的 cpu, 例如 0-7, 表示主板有 8 个 cpu 插槽, 支持 8 个 cpu, 编号 0-7

- present  
    表示主板上已经安装的 cpu, 例如 0-3, 表示当前主板上安装了 4 个 cpu, 编号 0-3

- online  
    代表目前正在工作的 cpu, 例如 0-3, 表示系统中有 4 个 cpu 在工作, 编号 0~3

- offline  
    代表目前被关掉的 cpu 

- kernel_max  
    代表内核支持的最大 cpu 数目

- isolated  
    显示当前系统是否有 cpu 被设置为单独隔离

# 0x03. 子目录 cpufreq

子目录 cpufreq 就是调频子系统的接口目录, 对应每个 cpux 单独有一个 policyx 的子目录, 其对应了编号为 x 的 cpu 的变频设置

```bash
# monitor 机器
$ cd /sys/devices/system/cpu/
$ ls cpufreq/
policy0  policy1  policy2  policy3
$ ls -hl cpufreq/policy0/
total 0
-r--r--r-- 1 root root 4.0K Jan 18 22:16 affected_cpus
-r--r--r-- 1 root root 4.0K Jan 18 22:16 cpuinfo_max_freq
-r--r--r-- 1 root root 4.0K Jan 18 22:16 cpuinfo_min_freq
-r--r--r-- 1 root root 4.0K Jan 18 22:16 cpuinfo_transition_latency
-r--r--r-- 1 root root 4.0K Jan 18 22:16 related_cpus
-r--r--r-- 1 root root 4.0K Jan 18 22:16 scaling_available_governors
-r--r--r-- 1 root root 4.0K Jan 18 22:16 scaling_cur_freq
-r--r--r-- 1 root root 4.0K Jan 18 22:16 scaling_driver
-rw-r--r-- 1 root root 4.0K Dec  9 22:15 scaling_governor
-rw-r--r-- 1 root root 4.0K Jan 18 22:16 scaling_max_freq
-rw-r--r-- 1 root root 4.0K Jan 18 22:16 scaling_min_freq
-rw-r--r-- 1 root root 4.0K Jan 18 22:16 scaling_setspeed
```

前缀 cpuinfo_ 代表的是 cpu 硬件上支持的频率, 而 scaling_ 前缀代表的是可以通过 CPUFreq 系统用软件进行调节时所支持的频率.  

- affected_cpus

    属于该策略的 online CPU 列表。

- bios_limit

    如果平台固件 (BIOS) 告诉操作系统对 CPU 频率限制一个上限，则将通过此属性（如果存在）报告该限制。  
    如果使用的缩放驱动程序不支持该属性，则该属性不存在。

- cpuinfo_cur_freq

    从硬件获取的属于此策略的 CPU 的当前频率（以 KHz 为单位）。
    这预计是硬件实际运行的频率。如果无法确定该频率，则不应存在该属性。

- cpuinfo_max_freq

    属于此策略的 CPU 可以运行的最大可能运行频率（以 kHz 为单位）。

- cpuinfo_min_freq

    属于此策略的 CPU 可以运行的最低可能运行频率（以 kHz 为单位）。

- cpuinfo_transition_latency

    将属于该策略的 CPU 从一种 P 状态切换到另一种 P 状态所需的时间（以纳秒为单位）。

- related_cpus

    属于该策略的所有（在线和离线）CPU 的列表。

- scaling_available_governors

    内核中可用于此策略的 CPUFreq scaling governors 列表  
    或者（如果正在使用 intel_pstate 缩放驱动程序）是驱动程序提供的可应用于此策略的 scaling algorithms 列表。

- scaling_cur_freq

    属于此策略的所有 CPU 的当前频率（以 kHz 为单位）。

    在大多数情况下，这是 scaling driver 使用 scaling interface 请求的 P 状态的频率，这可能反映也可能不反映 CPU 实际运行的频率（由于硬件设计或者其他限制）。

    某些架构（例如 x86 ）可能会尝试通过此属性提供更准确地反映当前 CPU 频率的信息，但这仍然可能不是硬件看到的准确 CPU 频率。

- scaling_driver

    当前使用的缩放驱动程序。

- scaling_governor

    当前 attached to 此策略的 scaling governor 
    或（如果正在使用 intel_pstate 扩展驱动程序）是当前应用于此策略的驱动程序提供的扩展算法。

- scaling_max_freq

    允许属于此策略的 CPU 运行的最大频率（以 kHz 为单位）。  
    该属性是读写的，向其写入表示整数的字符串将导致设置新的限制（它不得低于 scaling_min_freq 属性的值）。

- scaling_min_freq

    允许属于此策略的 CPU 运行的最低频率（以 kHz 为单位）。
    该属性是读写的，向其写入表示非负整数的字符串将导致设置新的限制（它不得高于 scaling_max_freq 属性的值）。

- scaling_setspeed

    仅当用户空间扩展调控器附加到给定策略时，此属性才起作用。
    它返回 governor 请求的最后一个频率（以 kHz 为单位），或者可以写入该频率以便为策略设置新频率。


# 0x04. 子目录 cpuX

每个安装的 cpu 都有一个单独的目录 cpux, cpux 中存储了编号为 x 的 cpu 的所有信息. 其中其子目录 cpufreq 存储了其 CPU-Freq 变频的信息, 它是一个链接文件, 指向了我们之前提到的 ../cpufreq/policyx

内核中的 cpufreq 子系统通过 sysfs 文件系统提供了用户接口, 对于系统中的每一个 CPU 而言, 其 cpufreq 的 sysfs 用户接口位于 `/sys/devices/system/cpu/cpuX/cpufreq/` 目录下，其中 X 代表 processor id, 与 /proc/cpuinfo 中的信息(只提供了基本的信息)相对应。

```bash
# monitor 机器
$ cd /sys/devices/system/cpu/
$ ls -hl cpu0/
total 0
drwxr-xr-x 7 root root    0 Jan 18 23:06 cache
lrwxrwxrwx 1 root root    0 Jan 18 23:06 cpufreq -> ../cpufreq/policy0
drwxr-xr-x 7 root root    0 Jan 18 23:06 cpuidle
-r-------- 1 root root 4.0K Jan 18 23:06 crash_notes
-r-------- 1 root root 4.0K Jan 18 23:06 crash_notes_size
lrwxrwxrwx 1 root root    0 Jan 18 23:06 driver -> ../../../../bus/cpu/drivers/processor
lrwxrwxrwx 1 root root    0 Jan 18 23:06 firmware_node -> ../../../LNXSYSTM:00/LNXSYBUS:00/ACPI0004:00/LNXCPU:00
drwxr-xr-x 2 root root    0 Jan 18 23:06 hotplug
drwxr-xr-x 2 root root    0 Jan 18 23:06 microcode
lrwxrwxrwx 1 root root    0 Jan 18 23:06 node0 -> ../../node/node0
drwxr-xr-x 2 root root    0 Jan 18 23:06 power
lrwxrwxrwx 1 root root    0 Jan 18 23:06 subsystem -> ../../../../bus/cpu
drwxr-xr-x 2 root root    0 Jan 18 23:06 thermal_throttle
drwxr-xr-x 2 root root    0 Jan 18 23:06 topology
-rw-r--r-- 1 root root 4.0K Jan 18 23:06 uevent
```

# 0x05. 子目录 intel_pstate

```bash
$ ls -hl intel_pstate/
total 0
-rw-r--r-- 1 root root 4.0K Feb 13 16:58 max_perf_pct
-rw-r--r-- 1 root root 4.0K Feb 13 16:58 min_perf_pct
-rw-r--r-- 1 root root 4.0K Feb 13 16:58 no_turbo
-r--r--r-- 1 root root 4.0K Feb 13 16:58 num_pstates
-rw-r--r-- 1 root root 4.0K Feb 13 16:58 status
-r--r--r-- 1 root root 4.0K Feb 13 16:58 turbo_pct

$ cat intel_pstate/*
100
35
0
23
active
44
```

- max_perf_pct

    Maximum P-state the driver is allowed to set in percent of the maximum supported performance level (the highest supported turbo P-state).  
    该值实际是设置最大的性能百分比，max_perf_pct = 50 可以限制其主频最多跑到 max_freq 的 50%。

    如果内核命令行中存在 intel_pstate=per_cpu_perf_limits 参数，则不会公开此属性。

- min_perf_pct

    Minimum P-state the driver is allowed to set in percent of the maximum supported performance level (the highest supported turbo P-state).

    如果内核命令行中存在 intel_pstate=per_cpu_perf_limits 参数，则不会公开此属性。

- num_pstates

    Number of P-states supported by the processor (between 0 and 255 inclusive) including both turbo and non-turbo P-states (see Turbo P-states Support).  
    处理器支持的 P-states 数量（介于 0 和 255 之间），包括 Turbo 和非 Turbo P-states.

- turbo_pct

    Ratio of the turbo range size to the size of the entire range of supported P-states, in percent.  
    Turbo range 大小与受支持的 P-states 的整个范围大小的比率（以百分比为单位）。

- no_turbo

    If set (equal to 1), the driver is not allowed to set any turbo P-states (see Turbo P-states Support). If unset (equalt to 0, which is the default), turbo P-states can be set by the driver. [Note that intel_pstate does not support the general boost attribute (supported by some other scaling drivers) which is replaced by this one.]  
    如果设置（等于 1），则不允许驱动程序设置任何 Turbo P-states 。如果未设置（等于 0，这是默认值），则驱动程序可以设置 Turbo P-states。

- hwp_dynamic_boost

    This attribute is only present if intel_pstate works in the active mode with the HWP feature enabled in the processor. If set (equal to 1), it causes the minimum P-state limit to be increased dynamically for a short time whenever a task previously waiting on I/O is selected to run on a given logical CPU (the purpose of this mechanism is to improve performance).  
    仅当 intel_pstate 工作在 active 模式且处理器中启用了 HWP 功能时，此属性才存在。如果设置（等于 1），则每当选择先前等待 I/O 的任务在给定逻辑 CPU 上运行时，都会导致最小 P-states 限制在短时间内动态增加（此机制的目的是提高性能）。

- status

    Operation mode of the driver: “active”, “passive” or “off”.

    - active  
        The driver is functional and in the active mode.  
        驱动程序正常工作并处于活动模式。
    - passive  
        The driver is functional and in the passive mode.  
        驱动程序正常工作且处于被动模式。
    - off  
        The driver is not functional (it is not registered as a scaling driver with the CPUFreq core).  
        该驱动程序不起作用（它未注册为 CPUFreq 核心的缩放驱动程序）。