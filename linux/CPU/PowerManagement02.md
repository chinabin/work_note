# 0x00. 导读

# 0x01. 简介

[Docs/The Linux kernel user’s and administrator’s guide/Power Management/Working-State Power Management](https://www.kernel.org/doc/html/v4.19/admin-guide/pm/working-state.html)

- CPU Performance Scaling
- **intel_pstate CPU Performance Scaling Driver**

# 0x02. General Information

intel_pstate 是 Linux 内核 ( CPUFreq ) 中 CPU performance scaling subsystem 的一部分。

intel_pstate 可以在三种不同的模式下运行，使用哪个取决于使用的内核命令行选项以及处理器的功能：
- active mode with hardware-managed P-states support
- active mode without hardware-managed P-states support
- passive mode(被动模式)

## 2.1 Active Mode

在此模式下，驱动程序绕过 CPUFreq 的 scaling governors 为 P-states 选择提供自己的 scaling algorithms.

这是 intel_pstate 的默认操作模式。如果在此模式下工作，则所有 CPUFreq 策略的 scaling_driver 策略属性包含字符串 intel_pstate.

```bash
$ cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_driver 
intel_pstate
```

They are not generic scaling governors, but their names are the same as the names of some of those governors. Moreover, confusingly enough, they generally do not work in the same way as the generic governors they share the names with(它们的工作方式通常与同名的通用调控器不同). For example, the powersave P-state selection algorithm provided by intel_pstate is not a counterpart of the generic powersave governor( intel_pstate 提供的 powersave P-state 选择算法不是通用 powersave 调控器的对应算法).

intel_pstate 在主动模式下提供了两种 P-states 选择算法： `powersave` 和 `performance` 。它们的操作方式取决于处理器中是否启用了 hardware-managed P-states (HWP) 功能，并且可能还取决于处理器型号。

默认使用哪种 P-state 选择算法取决于 `CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE` 内核配置选项。即，如果设置了该选项，则默认使用 performance 算法，如果未设置，则默认使用另一种算法。

### 2.1.1 Active Mode With HWP

如果处理器支持 HWP 功能，intel_pstate 将在处理器初始化期间启用，之后无法禁用。可以通过在命令行中将 `intel_pstate=no_hwp` 参数传递给内核来避免启用它。

如果启用了 HWP 功能， intel_pstate 依赖处理器自行选择 P-states，但仍然给予处理器内部的 P-states 选择逻辑一些 hints.

- HWP + performance

    In this configuration intel_pstate will write 0 to the processor’s Energy-Performance Preference (EPP) knob (if supported) or its Energy-Performance Bias (EPB) knob (otherwise), which means that the processor’s internal P-state selection logic is expected to focus entirely on performance.  
    覆盖来自 sysfs 界面的 EPP/EPB 设置。  

    在此配置中，处理器内部 P-states 选择逻辑可用的 P-states 范围始终限于上限（即允许驱动程序使用的最大 P-states ）。

- HWP + powersave

    In this configuration intel_pstate will set the processor’s Energy-Performance Preference (EPP) knob (if supported) or its Energy-Performance Bias (EPB) knob (otherwise) to whatever value it was previously set to via sysfs (or whatever default value it was set to by the platform firmware). This usually causes the processor’s internal P-state selection logic to be less performance-focused.

### 2.1.2 Active Mode Without HWP

这是不支持 HWP 功能的处理器的默认操作模式。默认情况下，它还与内核命令行中的 intel_pstate=no_hwp 一起使用。但是，在此模式下，如果 intel_pstate 无法识别给定处理器，则可能会拒绝 work.

这个模式下，intel_pstate 会通过 scaling_governor policy setting in sysfs 来决定P-state selection algorithm, either powersave or performance. 

- performance

    选择允许使用的最大 P-states ，但需遵守通过 sysfs 设置的限制。  
    如果设置了 CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE 内核配置选项，则这是默认的 P-state 选择算法。

- powersave

    通常选择与当前 CPU 利用率成比例的 P-states 。  
    如果未设置 CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE 内核配置选项，则这是默认的 P-state 选择算法。


## 2.2 Passive Mode

如果 intel_pstate=passive 参数在命令行中传递给内核（它也暗示 intel_pstate=no_hwp 设置），则使用此模式。与 Active Mode Without HWP 一样，在此模式下，如果给定处理器无法识别， intel_pstate 可能会拒绝 work.

如果驱动程序在此模式下工作，则所有 CPUFreq 策略的 sysfs 中的 scaling_driver 策略属性包含字符串 intel_cpufreq. 当需要与硬件通信以更改 CPU 的 P-state 时，它由 generic scaling governors 调用。


# 0x03. Turbo P-states Support

在大多数情况下， intel_pstate 可用的整个 P-states 范围可以分为两个子范围，分别对应于不同类型的处理器行为，位于边界之上和之下，该边界将被称为 turbo threshold.

高于 turbo threshold 的 P-states 被称为 turbo P-states ，并且它们所属的 P-states 的整个子范围被称为 turbo range.

turbo P-states 的一项重要特性是它们不可持续。更准确地说，不能保证任何 CPU 能够无限期地保持在任何这些状态。低于 turbo threshold 的 P-states 通常是可持续的。

有些处理器允许多个核心同时处于 turbo P-states，但是，可以为其设置的最大 P-states 通常取决于同时运行的核心数量：可以同时为 3 个核心设置的最大 Turbo P-states 通常低于 2 个核心的最大 P-states ，而 2 个核心通常又低于可以为 1 个核心设置的最大 Turbo P-states。The one-core maximum turbo P-state is thus the maximum supported one overall(总的，全面的；所有的).

# 0x04. Processor Support

要处理给定的处理器 intel_pstate 需要了解有关该处理器的许多不同信息，包括：

- The minimum supported P-state.
- The maximum supported non-turbo P-state.
- Whether or not turbo P-states are supported at all.  
    是否完全支持 Turbo P-states 。
- The maximum supported one-core turbo P-state
- The scaling formula(公式) to translate the driver’s internal representation of P-states into frequencies and the other way around.  
    将驱动程序的 P-states 表示转换为频率的缩放公式，反之亦然。

intel_pstate 中有一个支持的处理器列表，如果检测到的处理器不在该列表中，则驱动程序初始化将失败，除非它支持 HWP 功能。 

# 0x05. User Space Interface in sysfs



# 0x0 . Kernel Command Line Options for intel_pstate

可以使用多个内核命令行选项将参数传递给 intel_pstate ，以强制执行其特定行为。所有这些都必须以 intel_pstate= 前缀开头。

- disable

    Do not register intel_pstate as the scaling driver even if the processor is supported by it.

- passive

    Register intel_pstate in the passive mode to start with.  
    This option implies the no_hwp one described below.

- force

    Register intel_pstate as the scaling driver instead of acpi-cpufreq even if the latter is preferred on the given system.  
    将 intel_pstate 注册为缩放驱动程序，而不是 acpi-cpufreq ，即使后者在给定系统上是首选。

- no_hwp

    Do not enable the hardware-managed P-states (HWP) feature even if it is supported by the processor.

- hwp_only

    Register intel_pstate as the scaling driver only if the hardware-managed P-states (HWP) feature is supported by the processor.
    仅当处理器支持硬件管理的 P 状态 (HWP) 功能时，才将 intel_pstate 注册为缩放驱动程序。

- support_acpi_ppc

    Take ACPI _PPC performance limits into account.  
    考虑 ACPI _PPC 性能限制。

- per_cpu_perf_limits

    Use per-logical-CPU P-State limits