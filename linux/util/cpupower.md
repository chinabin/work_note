# 0x00. 导读

The cpuspeed service from Red Hat Enterprise Linux 5 and Red Hat Enterprise Linux 6 has been replaced in Red Hat Enterprise Linux 7 by the cpupower service. The cpupower service also conflicts with the tuned service in Red Hat Enterprise Linux 7 (because both adjust power management settings). tuned must be disabled in order to use cpuspeed, and vice versa. tuned profiles provide the building blocks necessary to approximate cpuspeed/cpupower functionality.

使用 cpupower 而不是 cpufrequtils.   
[讨论一](https://bbs.archlinux.org/viewtopic.php?id=135820)  
讨论二：
> 1.8. cpupowerutils
> cpupowerutils is a new project derived from cpufrequtils and extended with other features, like a powerful HW monitoring tool. Why a new project? The announcement explains it:
> 
> "CPU power consumption vs performance tuning is not about CPU frequency switching anymore for quite some time. Deep sleep states, traditional dynamic frequency scaling and hidden turbo/boost frequencies are tight close together and depend on each other. The first two exist on different architectures like PPC, Itanium and ARM the latter only on x86. On x86 the APU (CPU+GPU) will only run most efficiently if CPU and GPU has proper power management in place. Users and Developers want to have *one* tool to get an overview what their system supports and to monitor and debug CPU power management in detail". cpupowerutils is that tool.

# 0x01. 简介

Linux 内部共有五种对频率的管理策略 `userspace` ， `conservative` ， `ondemand` ， `powersave`  和  `performance` 。

- performance: 省电模式，通常以最低频率运行

- Userspace: 用户自定义模式，系统将变频策略的决策权交给了用户态应用程序，并提供了相应的接口供用户态应用程序调节CPU 运行频率使用

- powersave: 性能模式！只有最高频率，从来不考虑消耗的电量

- ondemand: 系统默认的超频模式，按需调节，内核提供的功能，不是很强大，但有效实现了动态频率调节，平时以低速方式运行，当系统负载提高时候自动提高频率。以这种模式运行不会因为降频造成性能降低，同时也能节约电能和降低温度

- conservative: 保守模式，类似于 ondemand，但调整相对较缓

## 1.1 cpupower

cpupower 是一组为辅助 CPU 调频而设计的用户空间工具。linux 内核支持调节 CPU 主频，降低功耗，已到达节能的效果。

通过选择系统空闲状态不同的电源管理策略，可以实现不同程度降低服务器功耗。但是，更低的功耗策略意味着 CPU 唤醒更慢对性能影响更大。对于对时延和性能要求高的应用。

对于服务器，一般都是接着电源的，而且要求性能比较高，因此，建议关闭 CPU 的动态调节功能，禁止 CPU 休眠，并把 CPU 频率固定到最高。

一般在服务器 BIOS 中修改电源管理为 Performance，如果发现 CPU 模式为 conservative 或者 powersave，可以使用 cpupower 设置 CPU Performance 模式，效果也是相当显著的。

查看当前策略
```bash
$ cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
```

查看可用
```bash
$ cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_available_governors
```

查看 CPU 频率
```bash
cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq
```
CPU 实时频率查看
```bash
$ watch -n 1 cpupower monitor
```

# 1.2 

cpufreq 是一个动态调整 cpu 频率的模块，系统启动时生成一个文件夹 `/sys/devices/system/cpu/cpu0/cpufreq/` ，里面有几个文件，其中 `scaling_min_freq` 代表最低频率， `scaling_max_freq` 代表最高频率， `scaling_governor` 代表 cpu 频率调整模式，用它来控制 CPU 频率。

默认的 CPU governor 配置是 performance ，这个设置可以从 `/etc/sysconfig/cpupower` 环境配置文件得到：
```
# See 'cpupower help' and cpupower(1) for more info
CPUPOWER_START_OPTS="frequency-set -g performance"
CPUPOWER_STOP_OPTS="frequency-set -g ondemand"
```
意思是 cpupower.service 启动将设置 governor 为 performance ，停止时则设置 ondemand 。

# 0x02. 

```
$ cpupower help command
```

```
Usage:  cpupower [-d|--debug] [-c|--cpu cpulist ] <command> [<args>]
Supported commands are:
        frequency-info
        frequency-set
        idle-info
        idle-set
        set
        info
        monitor

Use 'cpupower help <command>' for getting help for above commands.
```

查看所有 CPU 的信息：  
```bash
$ cpupower frequency-info
analyzing CPU 0:
  driver: intel_pstate                                                # 驱动，源码在内核树
  CPUs which run at the same hardware frequency: 0
  CPUs which need to have their frequency coordinated by software: 0
  maximum transition latency:  Cannot determine or is not supported.
  hardware limits: 800 MHz - 3.40 GHz                                 # 硬件支持的频率范围
  available cpufreq governors: performance powersave
  current policy: frequency should be within 800 MHz and 3.40 GHz.
                  The governor "performance" may decide which speed to use within this range.
  current CPU frequency: Unable to call hardware
  current CPU frequency: 2.60 GHz (asserted by call to kernel)
  boost state support:
    Supported: yes
    Active: yes


- driver：intel_pstate，这个 driver 比较特殊，它绕过了 governor layer，直接在驱动里实现了频率调整算法。
- CPU 频率范围硬限制：800MHz - 3.4GHz
- 可用 cpufreq governors：performance powersave
- 正在使用的 cpufreq governor: performance
当前策略：
    - 频率范围运行在 800MHz - 3.4GHz 之间；
    - 具体频率由 performance governor 决定。
当前 CPU 的频率：
    - 从硬件未获取到；
    - 从内核获取到的是 2.6GHz
是否支持 boost，即 turbo frequency
    - 支持
    - 当前已经开启
```

`[root@lmlphp.com ~]# cpupower -c all frequency-info`

```bash
# cpupower monitor -l 可以列出所有可以监控的模块，也就是 cpupower monitor 输出的所有列可以按照模块来过滤选择
$ sudo cpupower monitor -l
Monitor "Nehalem" (4 states) - Might overflow after 922000000 s
C3      [C] -> Processor Core C3
C6      [C] -> Processor Core C6
PC3     [P] -> Processor Package C3
PC6     [P] -> Processor Package C6
Monitor "Mperf" (3 states) - Might overflow after 922000000 s
C0      [T] -> Processor Core not idle
Cx      [T] -> Processor Core in an idle state
Freq    [T] -> Average Frequency (including boost) in MHz

# 例如，我要监控 Idle_Stats 
# cpupower monitor -m Idle_Stats

# 通过 cpupower monitor 指令来获取 CPU 主频，该指令是直接读取 MSR 198H(Target performance State Value) 来直接获取 CPU 主频信息
# 所以即使禁用了 intel_pstate 驱动也可以获得准确的数据。
$ cpupower monitor
              | Nehalem                   || Mperf              || Idle_Stats
 PKG|CORE| CPU| C3   | C6   | PC3  | PC6   || C0   | Cx   | Freq  || POLL | C1   | C1E  | C6
   0|   0|   0|  0.00|  0.00|  0.00|  0.00||  3.10| 96.90|  2692||  0.00| 96.96|  0.00|  0.00
   0|   0|  20|  0.00|  0.00|  0.00|  0.00||  2.05| 97.95|  2692||  0.00| 98.04|  0.00|  0.00
   0|   1|   4|  0.00|  0.00|  0.00|  0.00||  0.80| 99.20|  2692||  0.00| 99.23|  0.00|  0.00

Idle_Stats: 空闲统计
    显示 cpuidle 内核子系统的统计信息。
    从 /sys/devices/system/cpu/cpu*/cpuidle/state*/ 检索值。每次进入或离开空闲状态时，内核都会更新这些值。因此，当测量开始或结束时核心处于空闲状态一段时间时，可能会出现一些不准确的情况。在最坏的情况下，可能会发生一个内核在整个测量时间内保持空闲状态并且内核导出的空闲状态使用时间没有更新的情况。

Mperf: 平均性能
    该名称来自所使用的 aperf/mperf（平均和最大）MSR 寄存器，这些寄存器在最新的 X86 处理器上可用。它显示平均频率（包括增强(boost)频率）。事实上，在所有最新的硬件上，mperf 计时器在任何空闲状态下都会停止计时，它也用于显示 C0（处理器处于活动状态）和 Cx（处理器处于任何睡眠状态）时间。

Nehalem:
    Intel Core 和 Package 睡眠状态计数器。
```

设置所有 CPU 为性能模式：  
`[root@lmlphp.com ~]# cpupower -c all frequency-set -g performance`

设置所有 CPU 为节能模式：  
`[root@lmlphp.com ~]# cpupower -c all frequency-set -g powersave`

cpupower 设置 performance：  
`[root@lmlphp.com ~]# cpupower frequency-set -g performance`

查看频率
`grep -E '^model name|^cpu MHz' /proc/cpuinfo`

`cpupower idle-info` 命令列出支持的 C-State.
注意：系统必须加载了 intel_idle 驱动之后才能使用 -m Idle_Stats 模块，才能列出支持的 C-state，否则输出如下：
```
$sudo cpupower idle-info
CPUidle driver: none
CPUidle governor: menu

Analyzing CPU 0:
CPU 0: No idle states
```

# 0x0. 意外

[Linux性能调优之用电调优(Power Usage Tuning)](https://zhuanlan.zhihu.com/p/556462556)

如果 `/sys/devices/system/cpu/cpu*/` 下面没有 `cpufreq` 目录，则通过下面的命令查一下可以用的 CPUfreq 模块，然后手动加载

```
$ ls /usr/lib/modules/$(uname -r)/kernel/drivers/cpufreq/
acpi-cpufreq.ko  amd_freq_sensitivity.ko  cpufreq_stats.ko  p4-clockmod.ko  pcc-cpufreq.ko  powernow-k8.ko  speedstep-lib.ko

$ modprobe acpi-cpufreq
```