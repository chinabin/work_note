# 0x00. 导读


# 0x01. 简介

fdisk命令 用于观察硬盘实体使用情况，也可对硬盘分区。它采用传统的问答式界面，而非类似DOS fdisk的cfdisk互动式操作界面，因此在使用上较为不便，但功能却丝毫不打折扣。

# 0x02. 命令

```
fdisk [选项] <磁盘>           更改分区表
fdisk [选项] -l [<磁盘>...]   列出分区表

选项：
 -b, --sectors-size <大小>     显示扇区计数和大小
 -B, --protect-boot            创建新标签时不要擦除 bootbits
 -c, --compatibility[=<模式>]  模式，为“dos”或“nondos”(默认)
 -L, --color[=<时机>]          彩色输出（auto, always 或 never）默认启用颜色
 -l, --list                    显示分区并退出
 -x, --list-details            类似 --list 但提供更多细节
 -n, --noauto-pt               不要在空设备上创建默认分区表
 -o, --output <列表>           输出列
 -t, --type <类型>             只识别指定的分区表类型
 -u, --units[=<单位>]          显示单位，“cylinders”柱面或“sectors”扇区(默认)
 -s, --getsz                   以 512-字节扇区显示设备大小[已废弃]
      -b, --bytes                   以字节为单位而非易读的格式来打印 SIZE
      --lock[=<模式>]           使用独占设备锁（yes、no 或 nonblock）
 -w, --wipe <模式>             擦除签名（auto, always 或 never）
 -W, --wipe-partitions <模式>  擦除新分区的签名(auto, always 或 never)

 -C, --cylinders <数字>        指定柱面数
 -H, --heads <数字>            指定磁头数
 -S, --sectors <数字>          指定每条磁道的扇区数
```