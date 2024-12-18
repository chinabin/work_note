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

人们习惯于将连接多台独立计算机的网络称为 Network, 而将芯片内部或者多个芯片之间的互联网称为 Fabric. 片外访存网络（或者说片间互联）：Intel 的 QPI、IBM 的 SMI 和 OpenCAPI、ARM 的 CCIX、HP 的 GenZ。Intel 的 QPI 经过多个版本演进，如今改名为 UPI 了。

TODO:  
1. 11
2. 每个架构对应的缓存一致性策略，例如 snooping。
3. 带宽计算
4. uncore

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

处理器列表：[List_of_Intel_processors](https://en.wikipedia.org/wiki/List_of_Intel_processors), 微架构列表：[List_of_Intel_CPU_microarchitectures](https://en.wikipedia.org/wiki/List_of_Intel_CPU_microarchitectures)

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

快速通道互联（Intel QuickPath Interconnect, QPI），是一种由 Intel 开发并使用的点对点处理器互联架构，**用来实现 CPU 之间的互联**。尽管多数时候被称作 总线，但是 QPI 是一种点对点互联结构。它被设计成与 AMD 自 2003 年使用的 超传输（HyperTransport）总线 竞争。

Intel 在 2008 年开始用 QPI 取代以往用于至强、安腾处理器的前端总线（FSB）。初期，Intel 给这种连接架构的名称是 公共系统界面（Common System Interface ，CSI），是一种包传输的串行式高速点对点连接技术，用来实现：
- 芯片之间的直接互连，而非通过 FSB 连接到北桥，并且，其数据传输速率相较于 FSB 有大幅提升
- QPI 还用于处理器与 IOH（IO Hub）的互连（内存控制器已集成于处理器中）
- QPI 也常用于最末级缓存（Last Level Cache, LLC）（通常是 L3）的**缓存代理（CA, Caching Agent）**和**宿主代理（HA, Home Agent）**之间的通信。如果目的 HA 不在本处理器中，则也需要通过处理器间的QPI交互。

Cache Agent主要实现CPU Cache的管理，并且发起对内存的访问功能，并且响应其他请求的侦听请求。Home Agent主要实现多个CA对同一地址的串行化访问和对不同地址的并行化访问。

Intel 在发布 Sandy Bridge-EP 核心（Romley 平台）后，也顺势公布首代 QPI 的改进版 QPI 1.1 版本。Intel 于 2017 年发布的 SkyLake-SP Xeon 中，用 UPI（UltraPath Interconnect）取代 QPI。

![Alt text](../../../pic/linux/memory/DRAM/example37.png)

## 带宽

一组 QPI 具有具有 20 条数据传输线，以及发送 (TX) 和接收方 (RX) 的时钟信号。一个 QPI 数据包包含 80 位，需要两个时钟周期或四次传输完成整个数据包的传送。在每次传输的 20bit 数据中，有 16bit 是真实有效的数据，其余四位用于循环冗余校验，以提高系统的可靠性。由于 QPI 是双向的，在发送的同时也可以接收另一端传输来的数据，这样，每个 QPI 总线总带宽=每秒传输次数(即 QPI 频率) × 每次传输的有效数据(即 16bit/8=2Byte) × 双向。所以 QPI 频率为 4.8GT/s 的 总带宽 = 4.8GT/s × 2Byte × 2 = 19.2GB/s， QPI 频率为 6.4GT/s 的总带宽 = 6.4GT/s × 2Byte × 2 = 25.6GB/s。

## QPI架构分层

QPI 协议层制订了一系列规则，用于保证分布式共享存储系统的缓存同一性。常见的 QPI 缓存同一性协议分为 Home Snoop（宿主侦听）和 Source Snoop（数据源侦听）两种。

![Alt text](../../../pic/linux/memory/DRAM/example38.png)

从下到上是：物理层，链路层，路由层，传输层，协议层；(基本上认为是四层协议)
- 物理层：负责收发0,1；传输的单位是20bit，也被称为是physical unit 或者phit
    - 4.8GT/s的话，带宽是：4.8GT/s*16bit/8*2 direction=19.2GB/s
    - 可以工作在full half quarter宽度，full宽度的时候2个phit组成为1个flit
- 链路层：负责可靠传输和流控，传输的单位是80bit，也被称为是flow control unit或者flit
    - 支持14个消息类型，但是又一部分没使用；实际上可能只使用6个；
    - 支持3个虚拟网络，最大就是14*3=42个虚拟通道；
    - 流速控制使用令牌；发送者的令牌减少，接收者的令牌增加，每转发完成返回令牌；
    - 每80bit需要8wbit进行crc校验；
- 路由层：指导包的传输
    - 基本是FW写路由表实现；
- 传输层：负责可靠端到端传输，为可靠性预留；
- 协议层：各种flit包的传输
    - 实现一致性
    - 基于MESI(messy)协议，增加一个forward状态成MESIF协议；
    - 核心上是CA(Cache Agent),内存侧是HA(Home Agent)
    - 请求行为分为两种倾向：source snoop和home snoop，简单来讲就是少核心的时候使用source snoop ,多核心的时候使用home snoop；
        - source snoop是two hop snoop，二跳探听，在第二次的时候收到数据；
        - home snoop是three hop snoop，三跳探听，在第三次的时候收到数据；

# 0x04. UPI

UPI，即 Intel 的 Ultra Path Interconnect，是英特尔开发的点对点处理器互连技术( **Package 之间互联，并非物理 CPU 核之间的互联** )，取代 QPI 的技术。拥有更高的通信速率、效率、更低的功耗。

UPI only supports directory-based coherency, unlike previous QPI processors which supported multiple snoop modes (no snoop, early snoop, home snoop, and directory).

![Alt text](../../../pic/linux/memory/DRAM/example33.png)

## UPI 架构优化

在分布式共享存储处理机（Distributed shared memory, aka. DSM）中需要维护缓存一致性（Cache coherence），为保持缓存一致性有两种策略，snooping 或者 directory-based（译作“侦听”和“基于目录”）。与以前支持多种窥探模式(no snoop, early snoop, home snoop, and directory) 的 QPI 处理器不同，UPI 仅支持基于 `directory-based` 的一致性. 一个结合了 Caching and Home Agent(CHA, Caching Agent and Home Agent are combined as CHA) 的组件负责解决多个处理器之间的一致性问题，以及来自处理器核心和本地及远程代理的 Snoop 请求. 为了根据核心数量、内存控制器或子 NUMA 聚类模式提高可扩展性，单独的物理 CHA 被放置在每个处理器核心和最后一级缓存(LLC) Bank 中. 地址空间在不同的 CHA 之间交错，这些 CHA 表现得像一个单一的逻辑代理.

AMD 用 MOESI, Intel 用 [MESIF](https://en.wikipedia.org/wiki/MESIF_protocol), 其中的 F 是 Forward，同样是把更新过的数据转发给别的 CPU Cache 但是，MOESI 中的 Owner 状态 和MESIF 中的 Forward 状态有一个非常大的不一样—— Owner状态下的数据是dirty的，还没有写回内存，Forward状态下的数据是clean的，可以丢弃而不用另行通知。所以，F 状态主要是针对 CPU L3 Cache 设计的（前面我们说过，L3是所有CPU核心共享的）。

- Directory 协议。这种方法的典型实现是要设计一个集中式控制器，它是主存储器控制器的一部分。其中有一个目录存储在主存储器中，其中包含有关各种本地缓存内容的全局状态信息。当单个CPU Cache 发出读写请求时，这个集中式控制器会检查并发出必要的命令，以在主存和CPU Cache之间或在CPU Cache自身之间进行数据同步和传输。
- Snoopy 协议。这种协议更像是一种数据通知的总线型的技术。CPU Cache通过这个协议可以识别其它Cache上的数据状态。如果有数据共享的话，可以通过广播机制将共享数据的状态通知给其它CPU Cache。这个协议要求每个CPU Cache 都可以“窥探”数据事件的通知并做出相应的反应。如下图所示，有一个Snoopy Bus的总线。

![Alt text](../../../pic/linux/memory/DRAM/example35.png)


[MESI交互网站](https://www.scss.tcd.ie/Jeremy.Jones/VivioJS/caches/MESIHelp.htm)

## CHA

Cache Agent主要实现CPU Cache的管理，并且发起对内存的访问功能，并且响应其他请求的侦听请求。Home Agent主要实现多个CA对同一地址的串行化访问和对不同地址的并行化访问。
  
Cache Agent（CA），连接到 L3 Cache 端，cache misses 的时候为 cache line 数据制作请求发送给 HA

![Alt text](../../../pic/linux/memory/DRAM/example36.png)

内存中的一份数据在处理器的多个 core 的 cache line 中存在的时候，这些 core 中的每个 core 称为一个 client。

为了实现缓存一致性，一个 client 对 cache 的操作必须按照顺序传播到所有其他的 client。

实现缓存一致性的两种方法：
- Snooping, 侦听方法；可以理解成广播方法，一个 client 的请求和响应必须广播到所有的 client 上，缺点是 client 多的时候，要求的总线带宽比较大，优点是更快，更加简单。目前广泛是用在核心少的处理器的缓存一致性机制的实现里面。
- Directories Based，基于目录的方法；可以理解成基于请求的方法，需要维护一致性的 cache 被放在一个 dir 中，client 发出请求，被允许之后，dir 更新，其他地方的都置无效。
