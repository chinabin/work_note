# 0x00. 导读
[Documentation/cpu-freq/intel-pstate.txt](https://www.kernel.org/doc/Documentation/cpu-freq/intel-pstate.txt)

# 0x01. 简介

intel_pstate 其实是 governor 和 driver 的合体。或者可以理解为绕过了 governor 的 driver.   
注意，intel_pstate 再怎么复杂和厉害，也是属于前面说的 core governor driver 中的 driver 管辖。

## 1.1 特点

- 特点二

    > The Intel P-State driver implements the setpolicy() callback. 
    > This driver decides what P-State to use based on the requested policy(performance or powersave) from the cpufreq core. 
    > If the processor is capable of selecting its next P-State internally, 
    > then the driver will offload this responsibility to the processor (aka HWP: Hardware P-States). 
    > If not, the driver implements algorithms to select the next P-State.

    如果处理器支持 HWP ，那么就没软件的事了。

- 特点三

    Sysfs Interface: /sys/devices/system/cpu/intel_pstate/*  
    见 [sys_devices_system_cpu.md](./sys_devices_system_cpu.md)

- 特点四

    对于现代 Intel CPU( SandyBridge 和更新的型号 )，将使用 intel_pstate 功率驱动程序，它的优先级高于其他驱动程序，并编入内核（而非编译为模块）。intel_pstate 可能会忽略 BIOS P-State 设置，或者通过 intel_cpufreq 运行于 "被动模式"。如果使用时遇到问题，可以在内核行加入 intel_pstate=disable，这样系统会使用 acpi_cpufreq 驱动。

    acpi_cpufreq 也是驱动程序，可充分利用 ACPI Processor Performance States。

# 0x02. 模式

intel_pstate 可以在三种不同的模式下运行，使用哪个取决于使用的内核命令行选项以及处理器的功能：
- active mode with hardware-managed P-states support  
    那就是 HWP 控制频率了，没有 intel_pstate 啥事
- active mode without hardware-managed P-states support  
    intel_pstate 的主场
- passive mode(被动模式)

## 2.1 Active Mode

这是 intel_pstate 的默认操作模式。如果在此模式下工作，则所有 CPUFreq 策略的 scaling_driver 策略属性包含字符串 intel_pstate.

```bash
$ cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_driver 
intel_pstate
```

intel_pstate 在主动模式下提供了两种 P-states 选择算法： `powersave` 和 `performance`  ( 注意，和前面 CPUFreq 中说的 performance、powersave、userspace、ondemand、conservative 只是同名，并无关联)。

- performance: always picks the highest p-state  
- powersave: attempts to balance performance with energy savings

使用哪种取决于 `CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE` 内核配置选项。即，如果设置了该选项，则默认使用 performance 算法，如果未设置，则默认使用另一种算法。

### 2.1.1 Active Mode With HWP

如果处理器支持 HWP 功能，intel_pstate 将在处理器初始化期间启用，之后无法禁用。可以通过在命令行中将 `intel_pstate=no_hwp` 参数传递给内核来避免启用 HWP.

如果启用了 HWP 功能， intel_pstate 依赖处理器自行选择 P-states，但仍然给予处理器内部的 P-states 选择逻辑一些 hints.

## 2.2 Passive Mode

如果 intel_pstate=passive 参数在命令行中传递给内核（它也暗示 intel_pstate=no_hwp 设置），则使用此模式。与 Active Mode Without HWP 一样，在此模式下，如果给定处理器无法识别， intel_pstate 可能会拒绝 work.

如果驱动程序在此模式下工作，则所有 CPUFreq 策略的 sysfs 中的 scaling_driver 策略属性包含字符串 intel_cpufreq. 当需要与硬件通信以更改 CPU 的 P-state 时，它由 generic scaling governors 调用。

# 0x03. 睿频

从 [术语](./术语.md) 中关于睿频定义和例子（睿频 睿频加速技术）来看，睿频像是一个硬件上的惊喜，我们控制有限。

用于禁止处理器进入 turbo P-States：
```bash
$ cat /sys/devices/system/cpu/intel_pstate/no_turbo
# 0表示开启睿频；1表示关闭睿频
```

对于 intel_pstate 以外的缩放驱动程序，如果驱动程序支持超频，系统中应该可以找到 sysfs 属性 /sys/devices/system/cpu/cpufreq/boost ，并可用于禁用/启用超频：
```bash
$ echo 0 > /sys/devices/system/cpu/cpufreq/boost
```

如果需要实时监测 CPU 的频率，运行以下命令：
```bash
$ watch cat /sys/devices/system/cpu/cpu[0-9]*/cpufreq/scaling_cur_freq
```