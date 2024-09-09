# 0x00. 导读

需要额外安装，并不是原生自带. （GFDG0 上有~）

```
yum install sysstat
```

# 0x01. 简介

mpstat 是 Multiprocessor Statistics 的缩写，是实时系统监控工具。

其报告与CPU的一些统计信息，这些信息存放在 /proc/stat 文件中。在多 CPUs 系统里，其不但能查看所有 CPU 的平均状况信息，而且能够查看特定 CPU 的信息。

mpstat 最大的特点是：可以查看多核心 cpu 中每个 core 的统计数据；而类似工具 vmstat 只能查看系统整体 cpu 情况。

# 0x02. 命令

```
mpstat [ options ] [ <interval> [ <count> ] ]
Options are:
[ -A ] [ -u ] [ -V ] [ -I { SUM | CPU | SCPU | ALL } ]
[ -P { <cpu> [,...] | ON | ALL } ]

参数 解释
-P {|ALL} 表示监控哪个CPU， cpu在[0,cpu个数-1]中取值
-u CPU 利用率
-I 中断统计
-P 不管

internal 相邻的两次采样的间隔时间、
count 采样的次数，count只能和delay一起使用

当没有参数时， mpstat 则显示系统启动以后所有信息的平均值。
```

mpstat -P ALL 1

# 0x03. 解释

`%user`: 在 internal 时间段里，用户态的CPU时间  
`%sys`  : 在 internal 时间段里，内核时间  
`%iowait`: 在 internal 时间段里，硬盘IO等待时间  
`%irq`  : 在 internal 时间段里，硬中断时间  
`%soft`: 在 internal 时间段里，软中断时间  
`%idle`: 在 internal 时间段里，CPU除去等待磁盘IO操作外的因为任何原因而空闲的时间闲置时间   