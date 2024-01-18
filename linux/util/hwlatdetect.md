# 0x00. 导读

[github:rt-tests](https://github.com/jlelli/rt-tests/tree/master)

看个乐子，目前用的系统并没有打 PREEMPT_RT 补丁。

检查是否有打补丁：
```bash
$ uname -a
```
[what-does-smp-preempt-rt-mean-in-uname-a](https://unix.stackexchange.com/questions/591760/what-does-smp-preempt-rt-mean-in-uname-a)  
不能只有 PREEMPT ，还要有 RT ，看这 [PREEMPT](https://elixir.bootlin.com/linux/v5.7.1/source/kernel/Kconfig.preempt#L38)

补丁地址：[CentOS 7 - RealTime for x86_64: RealTime: kernel-rt-doc](https://linuxsoft.cern.ch/cern/centos/7/rt/x86_64/repoview/kernel-rt-doc.html)  

3.10.0-693.2.2.rt56.623.el7.x86_64：[下载](https://linuxsoft.cern.ch/cern/centos/7/rt/x86_64/Packages/kernel-rt-doc-3.10.0-693.2.2.rt56.623.el7.noarch.rpm)
3.10.0-1127.rt56.1093.el7.x86_64：[下载](https://linuxsoft.cern.ch/cern/centos/7/rt/x86_64/Packages/kernel-rt-doc-3.10.0-1127.rt56.1093.el7.noarch.rpm)

# 0x01. 简介

hwlatdetect 是 hardware latency detector 的缩写

rt-tests 中提供的 hwlatdetect 本质上只是一个 python 脚本，内部实际上是封装 Linux 内核的一个同名的叫做 hwlat_detector 的 tracer。所以如果要启用 rt-tests 中的 hwlatdtect，我们需要在构建 Linux 内核时使能 CONFIG_HWLAT_TRACER。有关 Linux 中这个 hwlat detector tracer 的说明，可以看一下 [内核文档 Hardware Latency Detector](https://elixir.bootlin.com/linux/v6.1.38/source/Documentation/trace/hwlat_detector.rst)。

需要注意的是，在早期，内核部分的支持是独立于 Linux，以一个 kernel module 的方式实现在 **PREEMPT_RT** 补丁中的，但后来这部分功能被 Linux 内核吸收了，实现为一个 tracer。最早大概是在 4.9 合入主线的（e7c15cd8a113335cf7154f027c9c8da1a92238ee）。

那么这个 hwlatdetect 究竟有什么用呢？我的理解是，我们可以用这个工具检查一下底层的硬件上是否存在一些特别的特性导致我们在这个硬件平台上运行 RT-Linux 时会遇到特别大的延迟问题。man 手册中提到了 x86 上的 SMI 中断，这也是 hwlatdetect 最早开发的原因，用于检测 x86/amd 上的 SMI 中断对系统延迟的影响。但目前的实现已经和具体 ARCH 无关了，所以我们也可以用这个工具来测试其他的 ARCH 平台。

SMI = System Management Interrupts, 系统管理中断，SMIs are a facility used by hardware vendors ensure the system is operating correctly. The SMI interrupt is usually not serviced by the running operating system, but by code in the BIOS. SMIs are typically used for thermal management, remote console management (IPMI), EDAC checks, and various other housekeeping tasks.

# 0x02. 

程序的原理是：  
创建一个在给定时间段内定期运行的线程。在一个周期开始时，线程禁用中断并开始采样。运行时，hwlatd 线程循环读取时间。由于中断被禁用，线程、IRQ 和 SoftIRQ 无法干扰 hwlatd 线程。因此，两次不同的时间读取之间的任何间隙的原因要么在于 NMI，要么在于硬件本身。  
在该周期结束时，hwlatd 启用中断并报告读取之间观察到的最大间隙。它还打印 NMI 发生计数器。如果输出不报告 NMI 执行情况，用户可以断定硬件是延迟的罪魁祸首。 hwlat通过观察NMI的进入和退出来检测NMI的执行情况。

```bash
$ hwlatdetect --duration=60s
hwlatdetect:  test duration 60 seconds
	detector: tracer
	parameters:
		Latency threshold:    10us
		Sample window:        1000000us
		Sample width:         500000us
		Non-sampling period:  500000us
		Output File:          None

Starting test
test finished
Max Latency: Below threshold
Samples recorded: 0
Samples exceeding threshold: 0
```