# 0x00. 导读

基础介绍，PCIe 物理世界长什么样，软件世界长什么样。

```
理解 PCIe 的基本概念
    学习目标：
        了解 PCIe 的基本原理、架构和应用场景。
        熟悉 PCIe 的关键术语和概念。
    关键概念：
        PCIe 的层次结构（事务层、数据链路层、物理层）。
        PCIe 的拓扑结构（根复合体、端点、交换机）。
        PCIe 的传输机制（TLP、DLLP）。
        PCIe 的带宽和版本（如 PCIe 3.0、4.0、5.0）。
```

# 0x01. 简介

上世纪 90 年代，Intel 就推出了一种共享总线协议，用于当时的 CPU 的 I/O 扩展，最初版本是 PCI 1.0 （全称是 Peripheral Component Interconnect），这个总线的定位就是外设的互联。

PCI 1.0 总线频率仅为 33MHz，位宽有 32bit，可以算出带宽为 33MHZ * 32bit / 8 = 133M Byte/s
- PCI1.0, 133MB/sec，32bit, 32MHz
- PCI2.0, 533MB/sec，64bit, 66MHz

从 PCI1.0 到 PCI2.0，带宽就实现了翻倍，带宽翻倍这个词一直是 PCI 协议发展的灵魂，延续到现在。PCI 一路发展，带宽不断翻倍，一发不可收拾，直到 PCI-X 2.0 当时的带宽已经达到 2133MByte/s。
- PCI-X 1.0, 1066MB/sec，64bit, 133MHz
- PCI-X 2.0, 2133MB/sec，64bit, 266MHz

这时问题来了，继续翻倍，64bit 位宽的并行线频率还要继续提升的话，串扰（Crosstalk）会越来越大，64 根线之间的不对齐延时（Skew）也很难保证，总结一句，信号完整性（SI）问题没法解决啊。一种新的技术应运而生，SerDes 技术可以解决上述问题，带宽还能继续往上提！Intel 索性把 PCI 改个名字，用了这么厉害的 SerDes 技术，带宽一下提升到了 2.5Gbit/s，那得把快体现出来呀，于是开天辟地 PCI Express 诞生了。

这时的 PCIe 1.0，一改之前 PCI 共享总线的架构，改为 **点对点（Point to Point）**（显著提高了数据传输的效率，因为每个设备都可以直接与主机或目标设备通信，而无需等待总线空闲）的连接（Link），一个 Link 的两端分别是两个 PCIe 设备（PCIe Device），一个 Link 可以最大包含 16 条数据通道（Lane），每条 Lane 采用全双工（Full Duplex）传输，每条 Lane 包含方向相反的 2 对差分线，共计 4 根线（一个通道 4 根线，最多 16 个通道。x1 是一个通道，x4 是四个通道，x16 是十六个通道）。

PCIe1.0 的最大带宽为 16Lane x 2.5Gbit/s = 5GByte/s。或者鸡贼点可说 PCIe 1.0 的双向带宽最大为 16Lane x 2.5Gbit/s x2 = 10GByte/s.

再后面，随着外设互联需求带宽的不断增加，PCIe 跟随着 SerDes 技术的演进不断的升级。

- PCIe 1.0, Gen1, 2.5Gbps/Lane
- PCIe 2.0, Gen2, 5Gbps/Lane
- PCIe 3.0, Gen3, 8Gbps/Lane
- PCIe 4.0, Gen4, 16Gbps/Lane
- PCIe 5.0, Gen5, 32Gbps/Lane
- PCIe 6.0 In progress, Gen6, 64Gbps/Lane

# 0x02.

PCIe 的全称是 Peripheral Component Interconnect Express, 是一种用于连接外设的总线。它于 2003 年提出来，作为替代 PCI 和 PCI-X 的方案，现在已经成了现代 CPU 和其他几乎所有外设交互的标准或者基石。

## 2.0 知乎科普

以下内容来自 [请问 PCIe 在计算机内部究竟扮演怎样的角色？ 老狼的回答](https://www.zhihu.com/question/565711215/answer/2867667249)

PCIe 在现代 x86 体系中除了传统的连接外部设备（真的有硬连线）的作用，更多的是充当 IP核 (Semiconductor intellectual property core, 可以理解为部分可重复使用的“芯片设计模块”，其作用就是在芯片设计环节中降低冗余设计成本，降低错误发生的风险，提高芯片设计效率) 粘合剂的作用。

这些 IP 相对 CPU 内核，是独立的存在。这种分离设计在 CPU 模块化设计的当下已经成为主流，谁不想减少 IP 之间、IP 和内核之间的耦合性，最终像搭积木一样搭出一个 CPU 呢？但它们又不是完全独立，还需要和带内内核进行互操作，甚至 IP 之间也可能互操作，这就需要暴露一些操作接口给对方。怎么暴露呢？

熟悉 CPU 原理的应该知道答案了，就是一组寄存器，准确的说是一组 CSR(Control and Status Register)。接下来唯一的问题是这些 CSR 如何暴露给带内内核了。在 x86 体系中，南桥的设备映射到 PCIe 地址空间中，而北桥和 CPU 内部的设备映射到 MMIO 中。但后期随着北桥的功能被移入 CPU，和 CPU 中加入越来越多的 IP，使得 MMIO 地址冲突越来越严重。更重要的是，MMIO 的映射关系不是 PnP 的，也就是不可以枚举的，而是在 ACPI 中列举的，这在系统整合和 SKU 越来越多的时候，造成很大麻烦。现在越来越多的设备 IP 从 MMIO 空间移入了 PCIe 空间（配置寄存器空间或者 BAR 上），有了自己的 Device 和 Func 号。在 BIOS 启动阶段设备甚至比在操作系统阶段可以看到的设备多得多，很多不需要终端用户关心的设备在启动最后阶段被隐藏了。那么这种迁移是如何做到的呢？是不是通过在芯片内部连接了无数 PCIe 总线呢？

并不是。这要归功于 IOSF(Intel On-Chip System Fabric) 总线，它提供 Die 内 backbone Fabric, 将各个 IP 整合在一起。IOSF 总线包括两种接口（Interface）：
- Primary Interface, 提供 PCIe 抽象，方便各个 IP CSR 映射到 PCIe 空间中。
- Sideband Interface, 旁路接口是传统的 index/value 接口，提供轻量级的访问，方便一些简单的 IP 的 CSR 映射到带外 IO 空间中，缺点是速度慢。

> backbone Fabric 是一种网络架构设计概念，它的主要作用是提供高带宽、低延迟和可靠的连接，以支持大规模的数据传输和通信需求。Backbone = 骨干网，指网络中的核心部分，负责连接不同的子网或节点，提供高速的数据传输通道。Fabric = 网络架构，指一种网状或分布式的网络拓扑结构，通常具有高冗余和高可扩展性。结合起来，Backbone Fabric 是一种基于网状或分布式拓扑的核心网络架构，旨在提供高效、可靠的数据传输。

借助于 IOSF 总线，大量的集成 IP 通过 Primary Interface 将自己的 CSR 暴露给带内 CPU，从而在独立和互操作上取得平衡。它们在软件逻辑上，看起来和 PCIe 没有什么不同，其实原理大不相同。正因为这样，我们今天，才能在 x86 PC 和服务器中，看到如此多的 PCIe 设备，它们不仅仅是传统的外部 PCIe 设备，还有很多是抽象的内部 IP。

## 2.1 总线

在电脑里，不同的设备要想交互数据，就必须要经过一定的通道，就好像现实当中，两个城市要交换货物，就必须要修路，这里的路就和计算机里的总线概念一样，总线就是计算机里，用于走数据的路，CPU核心和cache缓存交互数据的时候，使用的就是内部总线，这个总线只在CPU内交互数据，但是CPU不可能就自己在那空算数吧，他总要和其他设备交互数据，就需要用到外部总线了，CPU会通过外部总线和其他的设备比如硬盘，网卡，声卡，USB设备沟通，PCI-E 就是外部总线的一种（外部是相对于 CPU 来说，实际还是计算机内部的一种高速总线）。

维基百科：
> 总线（Bus）是指计算机组件间规范化的交换数据（data）的方式，即以一种通用的方式为各组件提供数据传送和控制逻辑。  
> 从另一个角度来看，如果说主板（Mother Board）是一座城市，那么总线就像是城市里的公共汽车（bus），能按照固定行车路线，传输来回不停运作的比特（bit）。这些线路在同一时间内都仅能负责传输一个比特。因此，必须同时采用多条线路才能发送更多资料，而总线可同时传输的资料数就称为宽度（width），以比特为单位，总线宽度愈大，传输性能就愈佳。总线的带宽（即单位时间内可以传输的总资料数）为：总线带宽 = 频率×宽度（Bytes/sec）。

> PC上一般有五种总线：  
> - 数据总线（Data Bus）：在CPU与RAM之间来回传送需要处理或是需要储存的数据。  
> - 地址总线（Address Bus）：用来指定在RAM（Random Access Memory）之中储存的数据的地址。  
> - 控制总线（Control Bus）：将微处理器控制单元（Control Unit）的信号，传送到周边设备，一般常见的为USB Bus和1394 Bus。  
> - 扩展总线（Expansion Bus）：可连接扩展槽和电脑。  
> - 局部总线（Local Bus）：取代更高速数据传输的扩展总线。  

![Alt text](../../../pic/linux/device/pcie_example4.png)

## 2.2 基础概念

首先，PCIe 硬件外观上（[PCIe的接口形态](https://zhuanlan.zhihu.com/p/368372274)），长这样，统一了江湖，不像之前一样，每个器件每个厂商各有各的长相

![Alt text](../../../pic/linux/device/pcie_example1.png)

![Alt text](../../../pic/linux/device/pcie_example2.png)

对内，自然就要实现一个架构来和操作系统交互。

![Alt text](../../../pic/linux/device/pcie_frame.png)

## 2.3 速率

![Alt text](../../../pic/linux/device/pcie_speed.png)

GT/s 即 Giga Transmission per second （千兆传输 / 秒），每一秒内传输的次数。  
Gbps 即 Giga Bits Per Second （千兆位/秒）。1 千兆比特每秒等于 100 万比特每秒。换算成 GB/s 的话需要除以 8.  
GT/s 与 Gbps 之间不存在成比例的换算关系。

例如：PCI-e2.0 协议支持 5.0 GT/s，即每一条 Lane 上支持每秒钟内传输 5G 个 bit（因为每条 lane 每次只能传一个 bit ，串行传输嘛）；但这并不意味着 PCIe 2.0 协议的每一条 Lane 支持 5Gbps 的速率，因为 PCIe 2.0 的物理层协议中使用的是 8b/10b 的编码方案。 即每传输 8 个 bit，需要发送 10 个 bit；这多出的 2 个 bit 并不是对上层有意义的信息。

那么， PCIe 2.0 协议的每一条 Lane 支持 5 * 8 / 10 = 4 Gbps = 0.5 GB/s = 500 MB/s 的速率，PCIe 2.0 x8 的通道为例，x8 的可用或者说有效带宽为 4Gbps * 8 = 32 Gbps = 4 GB/s。

PCIe采用了端到端的全双工的传输设计，基于数据包的传输，设备之间通过link相连，link支持1到32个lane：

![Alt text](../../../pic/linux/device/pcie_link.png)

# 0x. 其它

