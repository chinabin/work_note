# 0x00. 导读

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

每个安装的 cpu 都有一个单独的目录 cpux, cpux中存储了编号为 x 的 cpu 的所有信息. 其中其子目录 cpufreq 存储了其 CPU-Freq 变频的信息, 它是一个链接文件, 指向了我们之前提到的 ../cpufreq/policyx

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






```bash
$ cd /sys/devices/system/cpu/cpufreq/
$ ls
policy0  policy10  policy12  policy14  policy16  policy18  policy2   policy21  policy23  policy25  policy27  policy29  policy30  policy32  policy34  policy36  policy38  policy4  policy6  policy8
policy1  policy11  policy13  policy15  policy17  policy19  policy20  policy22  policy24  policy26  policy28  policy3   policy31  policy33  policy35  policy37  policy39  policy5  policy7  policy9
```

/sys/devices/system/cpu/cpuY/ 下的 cpufreq 符号链接指向 /sys/devices/system/cpu/cpufreq/policyX

```bash
alfred@bbro-gui:/sys/devices/system/cpu/cpu0$ ls -hlrt
total 0
lrwxrwxrwx 1 root root    0 Jan  3 15:03 cpufreq -> ../cpufreq/policy0
drwxr-xr-x 7 root root    0 Jan  3 15:03 cache
drwxr-xr-x 2 root root    0 Jan  4 16:48 topology
drwxr-xr-x 7 root root    0 Jan 18 22:15 cpuidle
-rw-r--r-- 1 root root 4.0K Jan 18 23:06 uevent
drwxr-xr-x 2 root root    0 Jan 18 23:06 thermal_throttle
lrwxrwxrwx 1 root root    0 Jan 18 23:06 subsystem -> ../../../../bus/cpu
drwxr-xr-x 2 root root    0 Jan 18 23:06 power
lrwxrwxrwx 1 root root    0 Jan 18 23:06 node0 -> ../../node/node0
drwxr-xr-x 2 root root    0 Jan 18 23:06 microcode
drwxr-xr-x 2 root root    0 Jan 18 23:06 hotplug
lrwxrwxrwx 1 root root    0 Jan 18 23:06 firmware_node -> ../../../LNXSYSTM:00/LNXSYBUS:00/ACPI0004:00/LNXCPU:00
lrwxrwxrwx 1 root root    0 Jan 18 23:06 driver -> ../../../../bus/cpu/drivers/processor
-r-------- 1 root root 4.0K Jan 18 23:06 crash_notes_size
-r-------- 1 root root 4.0K Jan 18 23:06 crash_notes

alfred@bbro-gui:/sys/devices/system/cpu/cpu0$ ls -hlrt ../cpufreq/policy0/
total 0
-rw-r--r-- 1 root root 4.0K Dec  9 22:15 scaling_governor
-rw-r--r-- 1 root root 4.0K Jan 18 22:16 scaling_min_freq
-r--r--r-- 1 root root 4.0K Jan 18 22:16 scaling_available_governors
-r--r--r-- 1 root root 4.0K Jan 18 22:16 cpuinfo_max_freq
-r--r--r-- 1 root root 4.0K Jan 18 22:16 related_cpus
-r--r--r-- 1 root root 4.0K Jan 18 22:16 scaling_cur_freq
-rw-r--r-- 1 root root 4.0K Jan 18 22:16 scaling_setspeed
-r--r--r-- 1 root root 4.0K Jan 18 22:16 affected_cpus
-rw-r--r-- 1 root root 4.0K Jan 18 22:16 scaling_max_freq
-r--r--r-- 1 root root 4.0K Jan 18 22:16 cpuinfo_transition_latency
-r--r--r-- 1 root root 4.0K Jan 18 22:16 scaling_driver
-r--r--r-- 1 root root 4.0K Jan 18 22:16 cpuinfo_min_freq
```

policyX 目录每个都包含特定于策略的属性（文件），以控制相应策略对象（即与它们关联的所有 CPU）的 CPUFreq 行为。

其中一些属性是通用的。它们由 CPUFreq core 创建，它们的行为通常不依赖于正在使用的 scaling driver 以及附加到给定策略的 scaling governor。一些 scaling driver 还将驱动程序特定的属性添加到 sysfs 中的策略目录，以控制驱动程序的策略特定行为。