# 0x00. 导读

# 0x01. 简介

# 0x02. 基础概念

x86多处理器发展历史上，早期的多核和多处理器系统都是 UMA 架构的。这种架构下，多个 CPU 通过同一个北桥(North Bridge)芯片与内存链接。北桥芯片里集成了内存控制器(Memory Controller)。下图是一个典型的早期 x86 UMA 系统，四路处理器通过 FSB (前端系统总线, Front Side Bus) 和主板上的内存控制器芯片 (MCH, Memory Controller Hub) 相连，DRAM 是以 UMA 方式组织的，延迟并无访问差异。

![Alt text](../../../pic/numa/06.png)

PCH(Platform Controller Hub)，Intel 于 2008 年起推出的 Chipset，用于取代以往的 I/O Controller Hub (ICH).

在 UMA 架构下，CPU 和内存控制器之间的前端总线 (FSB) 在系统 CPU 数量不断增加的前提下， 成为了系统性能的瓶颈。因此，AMD 在引入 64 位 x86 架构时，实现了 NUMA 架构。之后， Intel 也推出了 x64 的 Nehalem 架构，x86 终于全面进入到 NUMA 时代。x86 NUMA 目前的实现属于 ccNUMA。

从 Nehalem 架构开始，x86 开始转向 NUMA 架构，内存控制器芯片被集成到处理器内部，多个处理器通过 QPI 链路相连，从此 DRAM 有了远近之分。 而 Sandy bridge 架构则更近一步，将片外的 IOH 芯片也集成到了处理器内部，至此，内存控制器和 PCIe Root Complex 全部在处理器内部了。 下图就是一个典型的 x86 的 NUMA 架构：

![Alt text](../../../pic/numa/07.png)

Uncore 里集成了过去 x86 UMA 架构时代北桥芯片的基本功能。在 Nehalem 时代，内存控制器被集成到 CPU 里，叫做 iMC(Integrated Memory Controller)。 而 PCIe Root Complex 还做为独立部件在 IO Hub 芯片里。到了 SandyBridge 时代，PCIe Root Complex 也被集成到了 CPU 里。 现今的Uncore部分，除了 iMC，PCIe Root Complex，还有 QPI(QuickPath Interconnect) 控制器， L3缓存，CBox(负责缓存一致性)，及其它外设控制器。

在Intel x86上，NUMA Node之间的互联是通过 QPI(QuickPath Interconnect) Link的。 CPU的Uncore部分有QPI的控制器来控制CPU到QPI的数据访问。