# 0x00. 导读

需要自己安装。

```
sudo yum install tuned
sudo systemctl enable --now tuned

systemctl status tuned
```

# 0x01. 简介

RHEL/CentOS 在 6.3 版本以后引入了一套新的系统调优工具 tuned/tuned-adm，其中 **tuned** 是服务端程序，用来监控和收集系统各个组件的数据，并依据数据提供的信息动态调整系统设置，达到动态优化系统的目的; **tuned-adm** 是客户端程序，用来和 tuned 打交道，用命令行的方式管理和配置 tuned，tuned-adm 提供了一些预先配置的优化方案可供直接使用，比如：笔记本、虚拟机、存储服务器等。当然不同的系统和应用场景有不同的优化方案，tuned-adm 预先配置的优化策略不是总能满足要求，这时候就需要定制，tuned-adm 允许用户自己创建和定制新的调优方案。

```bash
$ tuned-adm list
Available profiles:
- balanced                    - General non-specialized tuned profile
- desktop                     - Optimize for the desktop use-case
- hpc-compute                 - Optimize for HPC compute workloads
- latency-performance         - Optimize for deterministic performance at the cost of increased power consumption
- network-latency             - Optimize for deterministic performance at the cost of increased power consumption, focused on low latency network performance
- network-throughput          - Optimize for streaming network throughput, generally only necessary on older CPUs or 40G+ networks
- powersave                   - Optimize for low power consumption
- throughput-performance      - Broadly applicable tuning that provides excellent performance across a variety of common server workloads
- virtual-guest               - Optimize for running inside a virtual guest
- virtual-host                - Optimize for running KVM guests
Current active profile: virtual-guest
```

- throughput-performance  
    针对高吞吐量优化的服务器配置文件。它禁用节能机制并启用 sysctl 设置，以提高磁盘和网络 IO 的吞吐量性能。CPU 调控器设置为 performance。

    它将 energy_performance_preference 和 scaling_governor 属性设置为 performance 配置集。

- accelerator-performance  
    accelerator-performance 配置集包含与 throughput-performance 配置集相同的调整。另外，它会将 CPU 锁定为低 C 状态，以便使延迟小于 100us。这提高了某些加速器的性能，如 GPU。

- latency-performance  
    为低延迟优化的服务器配置文件。它禁用节能机制并启用 sysctl 设置来缩短延迟。CPU 调控器被设置为 performance，CPU 被锁定到低 C 状态（按 PM QoS）。

    它将 energy_performance_preference 和 scaling_governor 属性设置为 performance 配置集。

- network-latency  
    低延迟网络调整的配置集。它基于 latency-performance 配置集。它还禁用透明大内存页和 NUMA 平衡，并调整其他一些与网络相关的 sysctl 参数。

    它继承 latency-performance 配置集，该配置集将 power_performance_preference 和 scaling_governor 属性更改为 performance 配置集。

- hpc-compute  
    针对高性能计算而优化的配置集。它基于 latency-performance 配置集。

- network-throughput  
    用于吞吐量网络调优的配置集。它基于 throughput-performance 配置集。此外，它还增加了内核网络缓冲区。

    它继承 latency-performance 或 throughput-performance 配置集，并将 energy_performance_preference 和 scaling_governor 属性改为 performance 配置集。

- virtual-guest  
    为 Red Hat Enterprise Linux 9 虚拟机和 VMWare 虚拟机设计的配置集基于 throughput-performance 配置集（除其他任务）减少了虚拟内存的交换性并增加磁盘预读值。它不会禁用磁盘障碍。

    它继承 throughput-performance 配置集，该配置集将 energy_performance_preference 和 scaling_governor 属性更改为 performance 配置集。

- virtual-host  
    基于 throughput-performance 配置集（除其他任务）为虚拟主机设计的配置集降低了虚拟内存交换，增加磁盘预读值，并启用更主动的脏页面回写值。

    它继承 throughput-performance 配置集，该配置集将 energy_performance_preference 和 scaling_governor 属性更改为 performance 配置集。

## 1.1 新建策略

了解以下。

在 `/usr/lib/tuned` 目录下面新建一个目录，拷贝一个已有策略的 `tuned.conf` 文件，例如 `virtual-host` ，然后修改。

`/etc/tuned/`

# 0x02. 常用选项

```bash
# 查看当前策略
$ tuned-adm active

# 应用 virtual-host 策略
$ tuned-adm profile virtual-host

# 推荐策略
$ tuned-adm recommend
virtual-guest

# 查看策略
$ tuned-adm profile_info virtual-guest
```

