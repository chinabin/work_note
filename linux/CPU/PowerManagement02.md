# 0x00. 导读

# 0x01. 简介

[Docs/The Linux kernel user’s and administrator’s guide/Power Management/Working-State Power Management](https://www.kernel.org/doc/html/v4.19/admin-guide/pm/working-state.html)

- CPU Performance Scaling
- **intel_pstate CPU Performance Scaling Driver**

# 0x02. General Information

intel_pstate 是 Linux 内核 ( CPUFreq ) 中 CPU performance scaling subsystem 的一部分。



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