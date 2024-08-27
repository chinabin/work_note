# 0x00. 导读

# 0x01. 简介

yum install pciutils

lspci 是一个用来显示系统中所有PCI总线设备或连接到该总线上的所有设备的工具。

lspci 主要扫描 `/sys/bus/pci/devices` 中的文件，并根据 pci 设备的 vender 及 device 到 `/usr/share/hwdata/pci.ids` 去做设备名称匹配并输出。

# 0x02. 

```bash
$ lspci -t -v
-+-[0000:c0]-+-00.0  Advanced Micro Devices, Inc. [AMD] Starship/Matisse Root Complex
             # A NIC card with 2 ports:
 |           +-01.1-[c1]--+-00.0  Mellanox Technologies MT2892 Family [ConnectX-6 Dx]
 |           |            \-00.1  Mellanox Technologies MT2892 Family [ConnectX-6 Dx]
 +-[0000:80]-+-00.0  Advanced Micro Devices, Inc. [AMD] Starship/Matisse Root Complex
             # A graphic card with 4 endpoints:
 |           +-01.1-[81]--+-00.0  NVIDIA Corporation TU104 [GeForce RTX 2080]
 |           |            +-00.1  NVIDIA Corporation TU104 HD Audio Controller
 |           |            +-00.2  NVIDIA Corporation TU104 USB 3.1 Host Controller
 |           |            \-00.3  NVIDIA Corporation TU104 USB Type-C UCSI Controller

```

```
-v
-vv: 显示更详细信息

-n: 以数字形式显示 PCI 生产厂商和设备号，而不是在 PCI ID 数据库中查找它们。 
-x: 以十六进制显示 PCI 配置空间 (configuration space) 的前64个字节映象 (标准头部信息)。此参数对调试驱动和 lspci 本身很有用。 
-xxx: 以十六进制显示所有 PCI 配置空间的映象。此选项只有 root 可用，并且很多 PCI 设备在你试图读取配置空间的未定义部分时会崩溃 (此操作可能不违反PCI标准，但是它至少非常愚蠢)。 

-b: 以总线为中心进行查看。显示所有 IRQ 号和内存地址，就象 PCI 总线上的卡看到的一样，而不是内核看到的内容。 
-t: 以树形方式显示包含所有总线、桥、设备和它们的连接的图表。 

-s [[<bus>]:][<slot>][.[<func>]]
    仅显示指定总线、插槽上的设备或设备上的功能块信息。设备地址的任何部分都可以忽略，或以“*”代替 (意味着所有值)。所有数字都是十六进制。例如：“0：”指的是在0号总线上的所有设备；“0”指的是在任意总线上0号设备的所有功能块；“0.3”选择 了所有总线上0号设备的第三个功能块；“.4”则是只列出每一设备上的第四个功能块。 
-d [<vendor>]:[<device>]
    只显示指定生产厂商和设备 ID 的设备。 这两个 ID 都以十六进制表示，可以忽略或者以“*”代替 (意味着所有值)。 
```

还有一个编号，叫做 DomainNumber ，不过一般输出中没有，因为都是0，所以就忽略了，理论上，一个 Segment 可以有 256 个 Bus，每个 Bus 可以有 32 个 Device ，每个 Device 可以有 8 个 Function，在一些场景下，比如设备特别多，是会有多个 Domain 的，在硬件层面对应多个 PCI Segment，在这种情况下，或者使用 lspci -D 命令，输出就会带上DomainNumber

```bash
# 按树状格式显示 PCI 器件，并提供根端口总线、器件和功能 (BDF) 编号。
$ lspci -t -v
 # [Domain:Bus]
 \-[0000:00]-+-00.0  Advanced Micro Devices, Inc. [AMD] Starship/Matisse Root Complex
         # Device.Function
             +-14.0  Advanced Micro Devices, Inc. [AMD] FCH SMBus Controller

# 有时候遇到这样的，那么查看命令就是 lspci -s 4e:00.0 -v 或者 lspci -s 4e:00.1 -v
$ lspci -t -v
 +-[0000:4d]-+-00.0  Intel Corporation Device 09a2
 |           +-00.1  Intel Corporation Device 09a4
 |           +-00.2  Intel Corporation Device 09a3
 |           +-00.4  Intel Corporation Device 0b23
 |           \-01.0-[4e]--+-00.0  Solarflare Communications XtremeScale SFC9250 10/25/40/50/100G Ethernet Controller
 |                        \-00.1  Solarflare Communications XtremeScale SFC9250 10/25/40/50/100G Ethernet Controller

# 在我们知道了任何一个设备的BDF之后，我们就可以通过它查看到这个设备的详细信息了
$ lspci -s 00:14.0 -vv 
00:14.0 SMBus: Advanced Micro Devices, Inc. [AMD] FCH SMBus Controller (rev 61)
        Subsystem: Super Micro Computer Inc H12SSL-i
        Control: I/O+ Mem+ BusMaster- SpecCycle- MemWINV- VGASnoop- ParErr- Stepping- SERR- FastB2B- DisINTx+
        Status: Cap- 66MHz+ UDF- FastB2B- ParErr- DEVSEL=medium >TAbort- <TAbort- <MAbort- >SERR- <PERR- INTx-
        IOMMU group: 39
        Kernel driver in use: piix4_smbus
        Kernel modules: i2c_piix4, sp5100_tco


$ lspci -v | grep -ai xilinx
2a:00.0 Processing accelerators: Xilinx Corporation Device 903f
        Subsystem: Xilinx Corporation Device 0507

# 注意 NUMA node 和 Physical Slot
$ sudo lspci -s 2a:00.0 -vv
2a:00.0 Processing accelerators: Xilinx Corporation Device 903f
        Subsystem: Xilinx Corporation Device 0507
        Physical Slot: 2
        Control: I/O- Mem+ BusMaster- SpecCycle- MemWINV- VGASnoop- ParErr+ Stepping- SERR+ FastB2B- DisINTx-
        Status: Cap+ 66MHz- UDF- FastB2B- ParErr- DEVSEL=fast >TAbort- <TAbort- <MAbort- >SERR- <PERR- INTx-
        NUMA node: 0
        Region 0: Memory at a9e00000 (64-bit, non-prefetchable) [size=4K]
        Capabilities: [40] Power Management version 3
                Flags: PMEClk- DSI- D1- D2- AuxCurrent=0mA PME(D0-,D1-,D2-,D3hot-,D3cold-)
                Status: D0 NoSoftRst+ PME-Enable- DSel=0 DScale=0 PME-
        Capabilities: [70] Express (v2) Endpoint, MSI 00
                DevCap: MaxPayload 256 bytes, PhantFunc 0, Latency L0s <64ns, L1 <1us
                        ExtTag- AttnBtn- AttnInd- PwrInd- RBE+ FLReset- SlotPowerLimit 75.000W
                DevCtl: Report errors: Correctable- Non-Fatal- Fatal+ Unsupported-
                        RlxdOrd+ ExtTag- PhantFunc- AuxPwr- NoSnoop+
                        MaxPayload 256 bytes, MaxReadReq 512 bytes
                DevSta: CorrErr+ UncorrErr- FatalErr- UnsuppReq+ AuxPwr- TransPend-
                LnkCap: Port #0, Speed 8GT/s, Width x16, ASPM not supported, Exit Latency L0s unlimited, L1 unlimited
                        ClockPM- Surprise- LLActRep- BwNot- ASPMOptComp+
                LnkCtl: ASPM Disabled; RCB 64 bytes Disabled- CommClk+
                        ExtSynch- ClockPM- AutWidDis- BWInt- AutBWInt-
                LnkSta: Speed 8GT/s, Width x16, TrErr- Train- SlotClk+ DLActive- BWMgmt- ABWMgmt-
                DevCap2: Completion Timeout: Range BC, TimeoutDis+, LTR-, OBFF Not Supported
                DevCtl2: Completion Timeout: 260ms to 900ms, TimeoutDis-, LTR-, OBFF Disabled
                LnkCtl2: Target Link Speed: 8GT/s, EnterCompliance- SpeedDis-
                         Transmit Margin: Normal Operating Range, EnterModifiedCompliance- ComplianceSOS-
                         Compliance De-emphasis: -6dB
                LnkSta2: Current De-emphasis Level: -6dB, EqualizationComplete+, EqualizationPhase1+
                         EqualizationPhase2+, EqualizationPhase3+, LinkEqualizationRequest-
        Capabilities: [100 v1] Advanced Error Reporting
                UESta:  DLP- SDES- TLP- FCP- CmpltTO- CmpltAbrt- UnxCmplt- RxOF- MalfTLP- ECRC- UnsupReq- ACSViol-
                UEMsk:  DLP- SDES- TLP- FCP- CmpltTO- CmpltAbrt- UnxCmplt- RxOF- MalfTLP- ECRC- UnsupReq+ ACSViol-
                UESvrt: DLP+ SDES+ TLP+ FCP+ CmpltTO- CmpltAbrt- UnxCmplt- RxOF+ MalfTLP+ ECRC- UnsupReq- ACSViol-
                CESta:  RxErr- BadTLP- BadDLLP- Rollover- Timeout- NonFatalErr-
                CEMsk:  RxErr- BadTLP- BadDLLP- Rollover- Timeout- NonFatalErr-
                AERCap: First Error Pointer: 00, GenCap- CGenEn- ChkCap- ChkEn-
        Capabilities: [1c0 v1] #19
        Kernel driver in use: PCIE_XQUOTE_DRIVER

```