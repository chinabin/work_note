# 0x00. 导读

[Intel资源调配技术(Intel RDT)架构](https://cloud-atlas.readthedocs.io/zh-cn/latest/kernel/cpu/intel/intel_rdt/intel_rdt_arch.html)

# 0x01. 简介

对于 L3 Cache，多个 CPU 核共享，无法避免非实时任务争抢 L3 Cache 对实时任务的影响。

为此 intel 推出了资源调配技术( `Intel RDT(Resource Director Technology)` )，提供了两种能力：**监控和分配**。

Intel RDT 提供了一系列分配(资源控制)能力，包括缓存分配技术( `Cache Allocation Technology, CAT` )，代码和数据优先级( `Code and Data Prioritization, CDP` ) 以及内存带宽分配( `Memory Bandwidth Allocation, MBA` )。该技术旨在通过一系列的 CPU 指令从而允许用户直接对每个 CPU 核心（以及附加了 HT 技术后，为每个逻辑核心）的 L2 缓存、 L3 缓存 (`LLC--Last Level Cache`) 以及内存带宽进行监控和分配。

# 0x02. 

`Linux Kernel 4.10` 引入了 Intel RDT 实现架构，基于 resctrl 文件系统提供了 L3 CAT (Cache Allocation Technology)，L3 CDP(Code and Data Prioritization)，以及L2 CAT。并且 Linux Kernel 4.12 进一步实现支持了MBA(Memory Bandwidth Allocation)内存带宽分配技术。