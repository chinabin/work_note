# 0x00. 导读

基础介绍，PCIe 物理世界长什么样，软件世界长什么样。

# 0x01. 简介

# 0x02.

PCIe 的全称是 Peripheral Component Interconnect Express, 是一种用于连接外设的总线。它于 2003 年提出来，作为替代 PCI 和 PCI-X 的方案，现在已经成了现代CPU和其他几乎所有外设交互的标准或者基石。

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

## 2.2 基础概念

首先，PCIe 硬件外观上，长这样，统一了江湖，不像之前一样，每个器件每个厂商各有各的长相

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

