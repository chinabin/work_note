# 0x00. 导读

注意，intel_pstate 再怎么复杂和厉害，也是属于前面说的 core governor driver 中的 driver 管辖。

# 0x01. 简介

intel_pstate 是 Intel P-state driver.

# 0x02. Sysfs Interface

Sysfs Interface: /sys/devices/system/cpu/intel_pstate/*
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