# 0x00. 导读

DMI (Desktop Management Interface, DMI) 就是帮助收集电脑系统信息的管理系统，DMI 信息的收集必须在严格遵照 SMBIOS 规范的前提下进行。 

SMBIOS (System Management BIOS) 是主板或系统制造者以标准格式显示产品管理信息所需遵循的统一规范。SMBIOS 和 DMI 是由行业指导机构 Desktop Management Task Force (DMTF) 起草的开放性的技术标准，其中 DMI 设计适用于任何的平台和操作系统。

DMI 充当了管理工具和系统层之间接口的角色。它建立了标准的可管理系统更加方便了电脑厂商和用户对系统的了解。DMI 的主要组成部分是 Management Information Format (MIF) 数据库。这个数据库包括了所有有关电脑系统和配件的信息。通过 DMI ，用户可以获取序列号、电脑厂商、串口信息以及其它系统配件信息。

dmidecode 的作用是将 DMI 数据库中的信息解码，以可读的文本方式显示。由于 DMI 信息可以人为修改，因此里面的信息不一定是系统准确的信息。

# 0x01. 简介

dmidecode的输出格式一般如下：
```
Handle 0×0002
DMI type 2, 8 bytes
Base Board Information
Manufacturer:Intel
Product Name: C440GX+
Version: 727281-0001
Serial Number: INCY92700942
```

其中的前三行都称为记录头(recoce Header), 其中包括了：

1. recode id(handle): DMI表中的记录标识符，这是唯一的,比如上例中的 `Handle 0×0002`。
2. dmi type id: 记录的类型，譬如说: BIOS，Memory，上例是 type 2，即 Base Board Information 
3. recode size: DMI 表中对应记录的大小，上例为 8 bytes.（不包括文本信息，所有实际输出的内容比这个size要更大。）  

记录头之后就是记录的值：  

4. decoded values: 记录值可以是多行的，比如上例显示了主板的制造商 (manufacturer)、model、version 以及 serial Number。

# 0x02. 命令

```bash
$ sudo dmidecode        # 打印所有硬件信息
$ sudo dmidecode -q     # 比较简洁，打印所有硬件信息

# 可以用 -t 代替 --type
$ dmidecode --type bios        # 查看BIOS相关的硬件信息
$ dmidecode --type system      # 查看系统相关的硬件信息
$ dmidecode --type baseboard   # 查看主板相关的硬件信息
$ dmidecode --type chassis     # 查看机箱相关的硬件信息
$ dmidecode --type processor   # 查看处理器相关的硬件信息
$ dmidecode --type memory      # 查看内存相关的硬件信息
```

或者

```
dmidecode -t typecode

typecode 对应的编码如下：

0 BIOS
1 System
2 Base Board
3 Chassis
4 Processor
5 Memory Controller
6 Memory Module
7 Cache
8 Port Connector
9 System Slots
10 On Board Devices
11 OEM Strings
12 System Configuration Options
13 BIOS Language
14 Group Associations
15 System Event Log
16 Physical Memory Array
17 Memory Device
18 32-bit Memory Error
19 Memory Array Mapped Address
20 Memory Device Mapped Address
21 Built-in Pointing Device
22 Portable Battery
23 System Reset
24 Hardware Security
25 System Power Controls
26 Voltage Probe
27 Cooling Device
28 Temperature Probe
29 Electrical Current Probe
30 Out-of-band Remote Access
31 Boot Integrity Services
32 System Boot
33 64-bit Memory Error
34 Management Device
35 Management Device Component
36 Management Device Threshold Data
37 Memory Channel
38 IPMI Device
39 Power Supply
40 Additional Information
41 Onboard Device
```

```bash
# 查看内存的插槽数，已经使用多少插槽。每条内存多大，已使用内存多大
$ dmidecode|grep -P -A5 "Memory\s+Device"|grep Size|grep -v Range 
```

```bash
# 查看内存支持的最大内存容量
$ dmidecode|grep -P 'Maximum\s+Capacity'
```

```bash
# 查看内存的频率
$ dmidecode|grep -A16 "Memory Device"
$ dmidecode|grep -A16 "Memory Device"|grep 'Speed'
```

```bash
# 查看服务器型号
$ dmidecode | grep 'Product Name'

# 查看主板序列号
$ dmidecode | grep 'Serial Number'

# 查看系统序列号
$ dmidecode -s system-serial-number

# 查看内存详细信息
$ dmidecode -t memory

# 查看OEM信息
$ dmidecode -t 11

# 查看服务器系统制造商
$ dmidecode -s system-manufacturer

# 查看服务器系统产品名称（服务器品牌型号）
$ dmidecode -s system-product-name

# 查看CPU详细信息
$ dmidecode -t processor
```