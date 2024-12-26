# 0x00. 导读

# 0x01. 简介

总线历史
```
1992 64 bit Data Bus Pentium
1995 Double Pumped Bus
2000 Quad Pumped Bus Pentium
2003 Dual Independent Bus (DIB)
2004 Shared Front side Bus(FSB)
2005 Dedicated High Speed Interconnects (DHSI)
2008 Quick Path Interconnect（QPI） 2018H2 诞生，NUMA环境中连接多个处理器的，高速点对点通信协议；
2017 Ultra Path Interconnect (UPI) 搭载 skylake server processor
```

在大多数普通用户眼里，CPU 也许就是一块金属盖子的电路板而已。可是如果去掉顶盖，深入正中那片小小的集成电路，我们会发现这里有着人类科技史上，最伟大的奇迹。几十亿个晶体管层层叠叠，密密麻麻挤在一起，占据着这个仅有一点几个平方厘米的狭小世界。

晶体管们在“上帝之手”的安排下，组成了各个功能模块。而这些功能模块之间，则是宽阔无比的超高速公路。这条超高速公路如此重要，以至于它的距离、速度和塞车与否，会大大影响这个小小世界的效率。这些模块就是 CPU 内部的功能模块，例如内 存控制器、L3/L2 Cache、PCU、PCIe root complex 等等，而这条高速公路，就是我们今天的主角，片内总线了。处理器内的不同模块就是通过 片内总线 进行通讯的。它的设计高效与否，会大大影响处理器的性能。如果把各个模块看做一个个节点，那么它们之间相互连接关系一般可以有以下几种：

![Alt text](../../../pic/linux/memory/DRAM/example19.png)

而处理器片内总线连接就经历了从 Star Bus --> Ring Bus --> Mesh Bus 的过程。

现在处理器都有 片上网络(Network-on-Chip, NoC), 在 die, cores, cache slices (large LLC is usually divided into slices), memory controllers 等部件的内部，都是通过一个 on-chip network(e.g., a ring in old-generation Xeon, or a mesh in Xeon Scalable) 连接起来的。

人们习惯于将连接多台独立计算机的网络称为 Network, 而将芯片内部或者多个芯片之间的互联网称为 Fabric. 片外访存网络（或者说片间互联）：Intel 的 QPI、IBM 的 SMI 和 OpenCAPI、ARM 的 CCIX、HP 的 GenZ。Intel 的 QPI 经过多个版本演进，如今升级为 UPI 了。

On-Chip Interconnect, Off-Chip Interconnect

## 1.1 uncore

[Uncore](https://en.wikipedia.org/wiki/Uncore) 是 Intel 使用的一个术语，用来描述微处理器的功能，这些功能不在核心中，但必须与核心紧密相连才能实现高性能。自 **Sandy Bridge** 微体系结构发布以来，它一直被称为 系统代理(system agent)。

典型的处理器内核包含执行指令所涉及的处理器组件，包括 ALU 、 FPU 、 L1 和 L2 高速缓存。相比之下，Uncore 包括 QPI 控制器、 L3 缓存、监听代理管道、片上内存控制器、片上 PCI Express Root Complex 和T hunderbolt 控制器。 其他总线控制器（例如SPI和LPC）是芯片组的一部分。

Intel 的 uncore 设计源于北桥。Intel 的 uncore 重新组织了对核心至关重要的功能，使它们在物理上更接近片上核心，从而减少了访问延迟。

Specifically, the microarchitecture of the Intel uncore is broken down into a number of modular units. The main uncore interface to the core is the so-called cache box (**CBox**, 大话计算机2 P473 中说 CBox 是 L3 缓存控制器), which interfaces with the last level cache (LLC) and is responsible for managing cache coherency. Multiple internal and external QPI links are managed by physical-layer units, referred to as PBox. Connections between the PBox, CBox, and one or more iMCs (MBox) are managed by the system configuration controller (UBox) and a router (RBox).

## 1.2 命名规则

[Intel 处理器架构演进](https://jcf94.com/2018/02/13/2018-02-13-intel/)

![Alt text](../../../pic/linux/memory/DRAM/example12.png)

英特尔旗下处理器有许多子品牌（Brand Name），包括我们熟悉的凌动（ATOM）、赛扬（CELERON）、奔腾（PENTIUM）、酷睿（CORE）、至强（XEON）等。

- 凌动（ATOM）是移动端处理器，用于平板、手机。
- 赛扬（CELERON）、奔腾（PENTIUM）、酷睿（CORE）属于桌面级(包括笔记本电脑)，多用于台式机和笔记本电脑。
- 至强（XEON）则属于企业级，多用于服务器和工作站。


品牌修饰符（Brand Modifier），例如 i3，i5，i7，i9，修饰符表示处理器在该系列中的等级。一般来说，数字越大，处理器性能越高，功能也越丰富。
- i3：低端，适合日常办公和轻度多任务处理。
- i5：中端，适合较高的多任务处理和轻度游戏。
- i7：高端，适合高性能任务，如视频编辑和游戏。
- i9：旗舰级，适合极高性能需求，如 4K 视频编辑、3D 建模和高端游戏。

**CPU代号** 代表的 CPU 的生产年份不同、生产工艺不同、架构设计的不同。各个代号除了这个数字的编号外，还会有一个英文代号。比如第 7 代的代号编号就是 Kaby Lake。注意，这和 微架构 不同。

![Alt text](../../../pic/linux/memory/DRAM/example13.png)

[编程考古-CPU(上)](https://zhuanlan.zhihu.com/p/11340114858)  
[编程考古-CPU(中)](https://zhuanlan.zhihu.com/p/12540407426)  
[编程考古-CPU(下)](https://zhuanlan.zhihu.com/p/12705805467)  
[至强Xeon工作站CPU介绍，洋垃圾为何大行其道](https://zhuanlan.zhihu.com/p/12340105587)
[晶体管的奋斗史（一文搞懂芯片晶体管发展历程）](https://zhuanlan.zhihu.com/p/1688492575)

处理器列表：[List_of_Intel_processors](https://en.wikipedia.org/wiki/List_of_Intel_processors), 微架构列表：[List_of_Intel_CPU_microarchitectures](https://en.wikipedia.org/wiki/List_of_Intel_CPU_microarchitectures), 为了方便 [wiki/Xeon](https://en.wikipedia.org/wiki/Xeon).

```
64位架构：core -> Nehalem -> Sandy Bridge -> Haswell -> Skylake -> Palm Cove -> Sunny Cove -> Cypress Cove -> Willow Cove -> Golden Cove -> Raptor Cove

衍生：
core, 2006: 
    Penryn
Nehalem, 2008.11: 
    Westmere
Sandy Bridge, 2011.01: 
    Ivy Bridge
Haswell, 2013.06: 
    Broadwell
Skylake, 2015.08: 
    Kaby Lake:
        Amber Lake
        Whiskey Lake
    Skylake-X
    Coffee Lake
    Cascade Lake
    Comet Lake
    Cooper Lake
Palm Cove:
    Cannon Lake
Sunny Cove, 2019.09:
    Ice Lake
    Lakefield
    Ice Lake-SP
Cypress Cove:
    Rocket Lake
Willow Cove:
    Tiger Lake
Golden Cove:
    Alder Lake
    Sapphire Rapids
Raptor Cove, 2021.11:
    Raptor Lake
    Emerald Rapids
```

![Alt text](../../../pic/linux/memory/DRAM/example15.png)

## 例子

工作和 Xeon 打交道较多，我多介绍一下，更多[看我](https://zh.wikipedia.org/wiki/%E8%87%B3%E5%BC%BA#Xeon%E7%B3%BB%E5%88%97)：
> Xeon（至强）主要供服务器及工作站使用。以前的 Xeon 分为 E3、E5、E7 三个系列，现在的 Xeon 分为 Xeon E、Xeon W、Xeon D、Xeon Scalable（分为青铜级 Bronze、白银级 Silver、黄金级 Gold 和铂金级 Platinum） 几个系列。另有加速卡 Xeon Phi。只有部分 Xeon E3、Xeon E 和 Xeon W 处理器内置 GPU。

通过 CPU 型号找微架构，例如 Intel Xeon W7-2495X, 首先看得出来这是 Xeon W 系列：
1. 打开 [wiki/Xeon](https://en.wikipedia.org/wiki/Xeon), 搜索 2400 ，得到答案 Sapphire Rapids-based Xeon
2. **推荐**：直接搜 Intel Xeon W7-2495X 的 [Intel 官方](https://www.intel.com/content/www/us/en/products/sku/233416/intel-xeon-w72495x-processor-45m-cache-2-50-ghz/specifications.html), Code Name Products formerly Sapphire Rapids, 得到答案
3. 打开 [wiki/List_of_Intel_processors](https://en.wikipedia.org/wiki/List_of_Intel_processors)， 搜索 Xeon W 得到答案。

例如 Intel Platinum 8580, 直接谷歌进入 [Intel 官网](https://www.intel.com/content/www/us/en/products/sku/237250/intel-xeon-platinum-8580-processor-300m-cache-2-00-ghz/specifications.html)，得到 Code Name Products formerly Emerald Rapids, 答案 Emerald Rapids.

注意，Intel 官网使用 "Products formerly xxx" 来描述 Code Name, 原因是 [Why use the ‘formerly’ word for something that is still around?](https://www.quora.com/Why-does-Intel-mark-their-CPUs-as-Products-formerly-Kaby-Lake-etc-even-for-the-series-that-are-still-in-production-Why-use-the-formerly-word-for-something-that-is-still-around)
> Kaby Lake、Skylake、Haswell 等是英特尔设计芯片时使用的开发名称。官方说法，Intel 将 Kaby Lake 一代称为“第七代核心架构”，而不是 Kaby Lake。英特尔所说的“以前的 kabylake”指的是这些芯片的开发代号是kabylake。
> 以前叫五道口技术学院，现在叫清华。
> 神经病


## core 架构

[core 架构](https://en.wikipedia.org/wiki/Intel_Core_(microarchitecture))

![Alt text](../../../pic/linux/memory/DRAM/example14.png)

相对上一代的主要改进：

- 引入了片内 4-12 MB 的 L3 cache！！！
- 重新加入超线程（奔腾 4 时代有，后来砍掉了，这一代开始重新引入）
- Intel Turbo Boost 1.0！！！
- 分支预测器分级！！！
- 二级的 TLB
- 每个核上有 3 个整数 ALU, 2 个向量 ALU and 2 个 AGU
- 采用 Intel QPI 来代替原来的前端总线！！！
- PCIE 和 DMI 控制器直接做到片内了，不再需要北桥
- IMC（集成内存控制器），内存控制也从北桥移到了片内
- 第二代的 Intel 虚拟化技术
- 流水线加到 20 到 24 级
- 其他指令扩展升级等等

## Sandy Bridge 架构

[Nehalem 架构](https://en.wikipedia.org/wiki/Sandy_Bridge)

- Intel Turbo Boost 2.0
- 增大了 L1 和 L2 cache
- 共享的 L3 cache 也同时支持片上的核芯显卡
- IMC 强化成了 GMCH（integrated graphics and memory controller），片上显卡共用主存作为它的显存
- 每个核上的运算部件增强
- 分支预测增强
- 微操作译码部分新增了一个 cache（uop cache）
- 14 到 19 级指令流水线！！！（长度区别基于上面那个 uop cache 是否命中）
- 多个核间、核芯显卡、cache 间用了**环状总线（ring bus）**
- Intel Quick Sync Video，支持视频的硬解码
- 其他指令扩展升级等等

![Alt text](../../../pic/linux/memory/DRAM/example16.png)

## Skylake 架构

从 Skylake-X 开始，之后的系列开始全面**改用 Mesh 结构**。

![Alt text](../../../pic/linux/memory/DRAM/example18.png)

## 总线变化

服务器上高核数 CPU 的设计，简要的说，为了在一块 CPU 里塞进越来越多的核心数，Intel 的多核 CPU 引入了 **ring bus** 的设计，核心、LLC、QPI 控制器分布于环上，内存控制器和 QPI 总线分布在环的两端，然而这样的结构随着核心的增加，延迟也会增加，于是从 Haswell 开始，高核心数的服务器 CPU 开始使用 **双环+双内存控制器** 的架构，在 LCC 中是单环， MCC 中是一个完整环和一个半环，HCC 中则是两个完整的环，半环和完整的环都带有一个独立的内存控制器。（LCC=low core count, MCC=medium, HCC=high, XCC=extreme. 参考：LCC: < 10, MCC: 10 - 15, HCC: 15 - 20, XCC: 20 - 28）

ring bus 架构的缺点很明显，即便是分成了两个环，依旧会有核心越多延迟越大的问题，且跨环访问时会增加一个额外的 CPU cycle 的延迟，所以 Broadwell 算是把 ring bus 续命做到了极致最多也只塞到了 22 个核心，为了塞进更多的核心数、更大的内存带宽，Intel 把 Xeon Phi 上最早出现过的 **mesh** 架构搬到了 Skylake 上， mesh 架构最多六行六列，其中一行被 UPI 和 PCI 控制器占据，另有两个位置是内存控制器，故最多塞进（6*6-6-2=28）个核心，同时还把 Cache 控制器、I/O 控制器等分布到了各个核心之间，新的控制器称为 CHA(cache home agent)，这个 CHA 同时还提供了不同核心之间寻路的路由功能。很直观的可以发现，由于 mesh 结构避免了顺着环进行顺序访问的问题，核心-缓存延迟和核心-内存延迟会降低；除此之外，I/O 操作的初始化延迟也会改善。

# 0x02. FSB

众所周之，前端总线（Front Side Bus，简称 FSB）是将 CPU 连接到北桥芯片的系统总线，它是 CPU 和外界交换数据的主要通道。前端总线的数据传输能力对计算机整体性能影响很大，如果没有足够带宽的前端总线，即使配备再强劲的 CPU，用户也不会感觉到计算机整体速度的明显提升。　

目前 intel 处理器主流的前端总线频率有 800MHz、1066MHz、1333MHz 几种，而就在 2007 年 11 月，intel再度将处理器的前端总线频率提升至 1600MHz（默认外频 400MHz），这比 2003 年最高的 800MHz FSB 总线频率整整提升了一倍。这样高的前端总线频率，其带宽多大呢？前端总线为 1333MHz 时，处理器与北桥之间的带宽为 10.67GB/s，而提升到 1600MHz 能达到 12.8GB/s，增加了 20%。

虽然 intel 处理器的前端总线频率看起来已经很高，但与同时不断提升的内存频率、高性能显卡（特别是双或多显卡系统）相比，CPU 与芯片组（南北桥）存在的前端总线瓶颈仍未根本改变。例如 1333MHz 的 FSB 所提供的内存带宽是 1333MHz × 64bit / 8 = 10667 MB/s = 10.67GB/s，与双通道的 DDR2-667 内存刚好匹配，但如果使用双通道的 DDR2-800、DDR2-1066 的内存，这时 FSB 的带宽就小于内存的带宽。更不用说和未来的三通道和更高频率的 DDR3 内存搭配了（Nehalem平台三通道DDR3-1333内存的带宽可达32GB/s）。

Intel 推出新的总线技术势在必行。　

# 0x03. QPI

