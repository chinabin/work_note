# 0x00. 导读

[NUMA系列文章](https://frankdenneman.nl/2016/07/11/numa-deep-dive-part-3-cache-coherency/)

socket：就是主板上插cpu的槽的数目，也即管理员说的”路“，一般做server chip说的dual-socket,就是双路直连的芯片，主要是因为单芯片性能不够，而限于工艺尺寸又没办法放更多的资源在一颗芯片，所以需要多路。`lscpu` 可以查看。

# 0x01. 简介

[Non-uniform memory access](https://en.wikipedia.org/wiki/Non-uniform_memory_access): Non-uniform memory access (NUMA) is a computer memory design used in multiprocessing, where the memory access time depends on the memory location relative to the processor.

# 0x02. 开启与关闭

在 Linux 系统中，​NUMA（非统一内存访问）的功能确实需要硬件和软件共同支持才能正常使用。
硬件支持：NUMA 的物理基础，软件支持：Linux 内核与工具。

硬件架构要求：

- NUMA 依赖于多处理器（或多核）系统的物理设计，通常出现在多路服务器（如双路、四路 CPU）或高端工作站中。
- 每个 CPU 插槽（Socket）或节点（Node）拥有独立的本地内存控制器和内存插槽，访问其他节点的内存需要通过跨节点互联（如 Intel 的 QPI 或 AMD 的 Infinity Fabric），导致更高的延迟。

内核必须启用 NUMA 功能：
- Linux 内核需在编译时启用 CONFIG_NUMA 选项，大多数主流发行版（如 RHEL、Ubuntu）默认开启。
- 检查内核是否支持 NUMA：
```bash
    grep CONFIG_NUMA /boot/config-$(uname -r)  # 输出应为 CONFIG_NUMA=y
```

总结一下：
- 硬件层：物理 CPU 和内存的 NUMA 拓扑被 BIOS/UEFI 上报给操作系统。
- ​内核层：Linux 内核解析硬件拓扑，创建 NUMA 节点数据结构，并管理内存分配策略。
- ​用户层：通过 numactl 或应用程序的 NUMA 感知编程，优化内存和 CPU 的绑定。

```
​硬件不支持 NUMA
    系统退化为 UMA 模式，所有内存访问被视为本地，软件层的 NUMA 配置无效。

​内核未启用 NUMA
    即使硬件支持 NUMA，内核会忽略 NUMA 拓扑，统一管理内存（等同于 numa=off）。

​未安装 NUMA 工具
    无法手动优化进程绑定或内存策略，但内核仍会按默认策略（如本地优先）管理 NUMA。
```

## 2.1 实操

如果使用 numactl 命令只有一个节点输出，我就当做 NUMA 没有开启。

1. 在 bios 层面 numa 关闭时，无论 os 层面的 numa 是否打开，都不会影响性能。TODO: 不知道 bios 层如何开启或者关闭，只知道检测办法。

    ```bash
    # 判断 bios 层是否开启 numa
    $ grep -i numa /var/log/dmesg
        No NUMA configuration found       # 这就是没有开启

    # 这就是开启了

    [    0.000000] mempolicy: Enabling automatic NUMA balancing. Configure with numa_balancing= or the kernel.numa_balancing sysctl
    [    1.639727] pci_bus 0000:00: on NUMA node 0
    [    1.642406] pci_bus 0000:40: on NUMA node 1
    [    1.645181] pci_bus 0000:3f: on NUMA node 0
    [    1.647692] pci_bus 0000:7f: on NUMA node 1
    ```

2. 在 os 层 numa 关闭时，打开 bios 层的 numa 会影响性能，QPS 会下降 15-30%;

    在 `/etc/grub.conf` 添加 `numa=off` 就行。

    NUMA cannot be disabled, but kernel can handle all memory as a single NUMA node. /usr/share/doc/kernel-doc-4.18.0/Documentation/x86/x86_64/boot-options.txt describes the option as follows.
    >  numa=off    #Only set up a single NUMA node spanning all memory.

# 0x03. ccNUMA

cc-NUMA 架构是 NUMA 架构的一种改进，它在 NUMA 的基础上引入了缓存一致性协议（如 MESI、MOESI 等），以确保不同节点之间的数据一致性。相较于传统的 NUMA 架构（NCC-NUMA），cc-NUMA 架构在保持扩展性和性能优势的同时，解决了数据一致性的问题。然而，cc-NUMA 架构的硬件复杂度和成本相对较高，因为它需要实现更为复杂的缓存一致性协议和通信机制。在实际应用中，cc-NUMA 架构可能更适用于对数据一致性要求较高的场景。

Q: 如何知道自己所用的 Linux 机器是否是 ccNUMA 呢？  
A: 日常应该都没有用上。

# 0x04. NUMA 对Linux 会产生什么影响？

系统启动的时候，硬件会把 numa 信息发送给 os，如果系统支持 numa，会发生以下几件事：

- 获取 numa 配置信息
- 将 processors（不是 cores） 分成很多 nodes，一般是一个 processor 一个 node。
- 将 processor 附近的 memory 分配给它。
- 计算 node 间通信的 cost（距离）。

系统启动之后：

- 每个进程、线程都会继承一个 numa policy，定义了可以使用那些CPU（甚至是那些 core），哪些内存可以使用，以及 policy 的强制程度，即是优先还是强制性只允许。
- 每个 thread 被分配到了一个默认的 node 上面运行，thread 可以在其他地方运行（如果 policy 允许的话），但是 os 会尝试让他在默认的 node 上面去运行。
- 内存分配：默认内存从同一个 node 里面进行分配。
- 在一个 node 上面分配地内存不会被移动到其他 node。

# 0x0. 常用命令

```bash
$ numactl --hardware
available: 1 nodes (0)  //1个节点有效
node 0 cpus: 0 1 2 3 4 5 6 7  //8核都安装在这个节点下
node 0 size: 16383 MB    //将近16G内存都由该节点控制
node 0 free: 3710 MB     //3.6g是空闲
node distances:
node 0 
0: 10
```

```bash
$ numactl --show
 policy: default
 preferred node: current
 physcpubind: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
 cpubind: 0 1
 nodebind: 0 1
 membind: 0 1
```

```bash
$ numactl -H
 available: 2 nodes (0-1)
 node 0 cpus: 0 1 2 3 4 5 6 7 16 17 18 19 20 21 22 23
 node 0 size: 16355 MB
 node 0 free: 12093 MB
 node 1 cpus: 8 9 10 11 12 13 14 15 24 25 26 27 28 29 30 31
 node 1 size: 16384 MB
 node 1 free: 7247 MB
 node distances:
 node 0 1
 0: 10 20
 1: 20 10
```

```bash
$ numastat
node0
numa_hit 5740940530
numa_miss 0
numa_foreign 0
interleave_hit 21170
local_node 5740940530
other_node 0
```