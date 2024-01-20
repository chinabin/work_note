# 0x00. 导读

# 0x01. 简介

turbostat 由 kernel-tools 软件包提供。用来查看目前 CPU 状态，在规定的间隔中给出计时器的结果以协助管理员识别服务器异常，例如过度耗电，无法进入深睡眠状态或是创建了不必要的系统管理中断（SMIs）。支持在 AMD 64 和 Intel® 64 处理器的系统中使用。需要 root 特权来运行，处理器支持时间戳计时器以及 APERF 和 MPERF 型号的特定寄存器。

> turbostat  reports processor topology, frequency, idle power-state statistics, temperature and power on X86 processors.  There are two ways to invoke turbostat.  The first method is to  supply a command (turbostat cat /dev/zero > /dev/null), which is forked and statistics are printed upon its completion.  The second method is to omit the command, and turbostat displays statistics every  5  seconds. The 5-second interval can be changed using the --interval option.

# 0x02. 常用选项

`man turbostat`

–Summary: 透过这个参数可以直接看所有处理器的总合资讯.
```
$ turbostat --Summary
```

–interval: 预设执行 turbostat 时间为 5 秒, 可以自行设定间隔时间.
```
$ turbostat --interval 1 --Summary
```

# 0x03. 结果注释

```bash
$ sudo turbostat
Disabling Low Power Idle CPU output
Core    CPU     Avg_MHz Busy%   Bzy_MHz TSC_MHz IRQ     SMI     CPU%c1  CPU%c6  CoreTmp PkgTmp  SYS%LPI CorWatt RAMWatt PKG_%   RAM_%
-       -       4608    100.00  4600    3005    49701   0       0.00    0.00    60      60      0.00    0.00    31.28   0.00    0.00
0       0       4610    100.00  4600    3006    22911   0       0.00    0.00    58      60      0.00    0.00    31.28   0.00    0.00
1       1       4609    100.00  4600    3006    220     0       0.00    0.00    46
2       2       4609    100.00  4600    3006    91      0       0.00    0.00    49
3       3       4609    100.00  4600    3006    229     0       0.00    0.00    51
4       4       4609    100.00  4600    3006    36      0       0.00    0.00    49
8       5       4609    100.00  4600    3006    322     0       0.00    0.00    50
9       6       4609    100.00  4600    3006    191     0       0.00    0.00    51
10      7       4609    100.00  4600    3006    100     0       0.00    0.00    47
11      8       4609    100.00  4600    3006    248     0       0.00    0.00    44
16      9       4609    100.00  4600    3006    128     0       0.00    0.00    48
17      10      4609    100.00  4600    3006    29      0       0.00    0.00    49
18      11      4609    100.00  4600    3006    5016    0       0.00    0.00    56
19      12      4608    100.00  4600    3005    5016    0       0.00    0.00    55
20      13      4608    100.00  4600    3005    35      0       0.00    0.00    48
24      14      4608    100.00  4600    3005    79      0       0.00    0.00    47
25      15      4602    100.00  4600    3001    5016    0       0.00    0.00    60
26      16      4601    100.00  4600    3001    5018    0       0.00    0.00    58
27      17      4600    100.00  4600    3000    5016    0       0.00    0.00    57
```

SMI 不需要一定为 0 ，但是不能增加

```
Core
处理器核心编号.

CPU
CPU 逻辑处理器号码, 0,1 代表 CPU 的逻辑处理器号码, –代表所有处理器的总合. 

Avg_MHz
CPU 平均工作频率.

Busy%
CPU 在 C0 (Operating State) 状态的平均时间百分比

Bzy_MHz
CPU 在 C0 (Operating State) 状态的平均工作频率 P stat.

TSC_MHz
处理器最高的运行速度 (不包含 Turbo Mode).

IRQ
在测量间隔期间由该 CPU 提供服务的中断 Interrupt Request (IRQ) 数量.

SMI
在测量间隔期间由 CPU 提供服务的系统管理中断 system management interrupt (SMI) 数量.

C1 , C3 , C6 , C7
在测量间隔期间请求 C1 (Halt), C3 (Sleep) , C6 (Deep Power Down) , C7 (C6 + LLC may be flushed ) 等状态的次数,.

C1% , C3% , C6%, C7%
在测量间隔期间请求 C1 (Halt), C3 (Sleep) , C6 (Deep Power Down) , C7 (C6 + LLC may be flushed ) 等状态的百分比.

CPU%c1, CPU%c3, CPU%c6, CPU%c7
在测量间隔期间请求 C1 (Halt), C3 (Sleep) , C6 (Deep Power Down) , C7 (C6 + LLC may be flushed ) 等状态的百分比.

CoreTmp
CPU 核心 Core 温度感测器回传的温度值.

PkgTtmp
CPU Package 温度感测器回传的温度值.

GFX%rc6
在测量间隔期间 GPU 处于 render C6 (rc6) 状态的时间百分比.

GFXMHz
测量间隔 GPU 工作频率.

Pkg%pc2, Pkg%pc3, Pkg%pc6, Pkg%pc7?

PkgWatt
CPU package 消耗的瓦特数.

CorWatt
CPU Core 消耗的瓦特数.

GFXWatt
GPU 消耗的瓦特数.

RAMWatt
DRAM DIMM 消耗的瓦特数.

PKG_%
CPU Package 处于 Running Average Power Limit (RAPL) 活动状态的时间百分比.

RAM_%
DRAM 处于 Running Average Power Limit (RAPL) 活动状态的时间百分比.
```

# 0x04. 经验

```bash
$ lscpu
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                18
On-line CPU(s) list:   0-17
Thread(s) per core:    1
Core(s) per socket:    18
Socket(s):             1
NUMA node(s):          1
Vendor ID:             GenuineIntel
CPU family:            6
Model:                 85
Model name:            Intel(R) Core(TM) i9-10980XE CPU @ 3.00GHz
Stepping:              7
CPU MHz:               4599.975
CPU max MHz:           4600.0000
CPU min MHz:           1200.0000
BogoMIPS:              6000.00
Virtualization:        VT-x
L1d cache:             32K
L1i cache:             32K
L2 cache:              1024K
L3 cache:              25344K
NUMA node0 CPU(s):     0-17
Flags:                 fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc aperfmperf eagerfpu pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid dca sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch epb cat_l3 cdp_l3 intel_pt ssbd mba ibrs ibpb stibp ibrs_enhanced tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm cqm mpx rdt_a avx512f avx512dq rdseed adx smap clflushopt clwb avx512cd avx512bw avx512vl xsaveopt xsavec xgetbv1 cqm_llc cqm_occup_llc cqm_mbm_total cqm_mbm_local dtherm ida arat pln pts hwp hwp_act_window hwp_epp hwp_pkg_req avx512_vnni spec_ctrl intel_stibp flush_l1d arch_capabilities
```

看 CPU max MHz 是 4600 ，结合前面的 Bzy_MHz 一栏也是稳定在 4600 ，表明稳定全核睿频