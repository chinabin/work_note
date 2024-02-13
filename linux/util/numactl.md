# 0x00. 导读

yum -y install numactl

[CPU 隔离之 numactl](https://github.com/moooofly/MarkSomethingDown/blob/master/Linux/CPU%20%E9%9A%94%E7%A6%BB%E4%B9%8B%20numactl.md)

# 0x01. 简介

在 bios 层面 numa 关闭时，无论 os 层面的 numa 是否打开，都不会影响性能。
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

在 os 层 numa 关闭时，打开 bios 层的 numa 会影响性能，QPS会下降15-30%;
```bash
# OS 层是否开启判断
$ cat /proc/cmdline
BOOT_IMAGE=/vmlinuz-4.9.14-1.el7.centos.x86_64 root=UUID=c42731a1-ffb6-4aed-a1ec-0758d79c5bee ro crashkernel=auto rhgb quiet LANG=en_US.UTF-8
```
如果出现 `numa=off` ，则表明在操作系统启动的时候就把 numa 给关掉了；否则就没有没有关闭；


# 0x02. 

常用选项
```
--interleave=nodes
    设置内存交织策略（memory interleave policy）；
    即内存分配采用在 nodes 上轮询的方式；当内存无法从当前交织目标（interleave target）上成功分配时，则转移到其他 nodes 上尝试获取；

--membind=nodes -m
    仅从指定 nodes 上分配内存；当在指定的这些 nodes 上没有足够的可用内存时，分配可能会失败；

--cpunodebind=nodes -N
    仅在指定 nodes 上的 CPUs 中执行 command ；需要注意的是，nodes 可能由多个 CPUs 构成；

--physcpubind=cpus -C
    仅在指定的 cpus 上执行；

--localalloc
    总是在当前 node 上分配内存；

--preferred=node
    优先在指定 node 上分配内存，但如果无法分配到所需内存，则会转移到其他 nodes 上进行分配；


--show, -s
    展示当前进程的 NUMA 策略设置；

--hardware, -H
    显示当前系统中可用 nodes 的详细清单；
```

## 2.1 NUMA 的内存分配策略

- 缺省(default)：总是在本地节点分配（分配在当前进程运行的节点上）；
- 绑定(bind)：强制分配到指定节点上；
- 交叉(interleave)：在所有节点或者指定的节点上交织分配；
- 优先(preferred)：在指定节点上分配，失败则在其他节点上分配。

因为NUMA默认的内存分配策略是优先在进程所在CPU的本地内存中分配，会导致CPU节点之间内存分配不均衡，当某个CPU节点的内存不足时，会导致swap产生，而不是从远程节点分配内存。这就是所谓的swap insanity 现象。

## 2.2 内核参数

内核参数 `overcommit_memory` 控制内存分配策略，可选值 0、1、2。

- 0: 表示内核将检查是否有足够的可用内存供应用进程使用；如果有足够的可用内存，内存申请允许；否则，内存申请失败，并把错误返回给应用进程。

- 1: 表示内核允许分配所有的物理内存，而不管当前的内存状态如何。

- 2: 表示内核允许分配超过所有物理内存和交换空间总和的内存

内核参数 `zone_reclaim_mode` 控制内存回收策略，通过设置 zone_reclaim_mode 能够实现在当某个 zone 内存用光时，采用相对多少有些激进（aggressive）的内存回收策略；如果设置为 0 ，那么将不会有 zone reclaim 发生；内存分配的需求将会从系统中其他 zones / nodes 上进行分配以满足需要；zone_reclaim_mode 默认是 disabled 的；

# 0x03. 例子

```bash
# 列举系统上的 NUMA 节点，如果 available 大于 0 表示开启了 numa
$ numactl --hardware  
available: 1 nodes (0)
node 0 cpus: 0 1 2 3
node 0 size: 15884 MB
node 0 free: 1847 MB
node distances:
node   0 
  0:  10 

# 查看绑定信息
$ numactl --show
policy: default
preferred node: current
physcpubind: 0 1 2 3 
cpubind: 0 
nodebind: 0 
membind: 0s
```