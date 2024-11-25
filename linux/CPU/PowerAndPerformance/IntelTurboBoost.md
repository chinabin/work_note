# 0x00. 导读

# 0x01. 简介

Intel Turbo Boost 是 Intel 在系列处理器实现的技术，通过动态控制处理器的时钟率来激活处理器运行在超过基础操作主频。

支持不同Turbo Boost版本技术的处理器分为：

- Turbo Boost 1.0: Nehalem
- Turbo Boost 2.0: Sandby Bridge
- Turbo Boost Max 3.0: Ivy Bridge, Haswell, Broadwell, Skylake, Broadwell-E

Turo Boost 是在操作系统请求处理器的最高性能状态（highest performance state, pstate）时候激活。

urbo Boost允许一个或多个CPU核心运行在更高的 P-states, 这个最大 P-state 需要考虑以下因素：

- 激活的核心数量（ C0 或 C1 状态）
- 评估当前处理器消耗（Imax）
- 评估处理器电能消耗（TDP - Thermal Design Power）
- 处理器稳定

# 0x02. Intel Turbo Boost Max Technology 3.0

Intel Turbo Boost Max Technology 3.0 使用一个和 CPU 中存储信息相连的驱动。它标识并直接工作在最快的芯片内核上。驱动也允许通过白名单自定义配置以便让用户设置应用程序的优先级。这个驱动必须在系统中存在和正确配置，否则操作系统就不能有效路由工作负载到目标处理器核心。

要使用 Intel Turbo Boost Max Technology 3.0 需要同时满足条件：

- CPU 处理器支持 （需要检查 Intel CPU 功能）
- 操作系统支持
- 驱动和相应的应用软件
- X99 或 X299 主板并使用激活 BIOS/firmware 支持

和 Intel Turbo Boost Technology 2.0 不同的是，3.0 版本允许单核心更高的主频。 Intel Turbo Boost Max 技术的软件用户接口和驱动允许用户优先将负载直接调度到最快的 CPU 核心上。

# 0x03. P-state

从 Linux Kernel 3.9(2009年4月)，一个新的 intel_pstate 加入到内核中，从 SandbyBridge 处理器开始，后续多代 Intel 处理器都得到了支持。

intel_pstate 驱动支持现代 Intel 处理器的温控。

处理器 P-state 是支持处理器运行在不同电压 和/或 主频级别的能力。总的来说，P0 是最高性能级别，而 P1 和 P2 等依次节约电能但是带来潜在的性能损失。

**在 Linux 内核启动参数中设置 intel_pstate=disable 选项可以强制使用传统遗留的 CPU 驱动 acpi_cpufreq。**

