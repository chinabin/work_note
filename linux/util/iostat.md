# 0x00. 导读

yum install sysstat 

# 0x01. 简介

iostat 主要用于监控系统设备的 IO 负载情况.

iostat 统计的是通用块层经过合并(rrqm/s, wrqm/s)后，直接向设备提交的 IO 数据，可以反映系统整体的 IO 状况，
但是有以下 2 个缺点:

1. 距离业务层比较遥远，跟代码中的 write，read 不对应(由于系统预读 + PageCache + IO 调度算法等因素，也很难对应)
2. 是系统级，没办法精确到进程，比如只能告诉你现在磁盘很忙，但是没办法告诉你是谁在忙，在忙什么

# 0x02. 命令

iostat [ option ] [ interval [ count ] ]  
man iostat

-c: 输出cpu统计信息  
-d: 输出磁盘统计信息  
-k|-m: 以 kb/s|mb/s 代替原来的 扇区数/s, 例如下面的 Blk_read  
-x: 输出详细的拓展统计数据，比如各种等待时间，队列，利用率等信息。  
-z: 忽略在统计周期内无 IO 活动的设备，不对这些设备输出信息。

常用 `iostat -xz 1`

```bash
$ iostat
Linux 2.6.16.60-0.21-smp (linux)     06/12/12

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
           0.07    0.00    0.05    0.06    0.00   99.81

Device:            tps   Blk_read/s   Blk_wrtn/s   Blk_read   Blk_wrtn
sda               0.58         9.95        37.47    6737006   25377400
sdb               0.00         0.00         0.00        824          0

avg-cpu: 总体cpu使用情况统计信息，对于多核cpu，这里为所有cpu的平均值
    iowait: 指示 cpu 用于等待 io 请求完成的时间。
    user: CPU 在用户态执行进程的时间百分比。
    system: CPU 处在内核态执行进程的时间百分比

Device: 各磁盘设备的IO统计信息
    tps: 每秒进程的 IO 读、写请求数量
    Blk_read/s: 每秒读扇区数量(一扇区为512bytes)
    Blk_wrtn/s: 每秒写扇区数量
    Blk_read: 取样时间间隔内读扇区总数量
    Blk_wrtn: 取样时间间隔内写扇区总数量
```

```bash
$ iostat -x
Linux 3.10.0-1160.49.1.el7.x86_64 (localhost.localdomain)       09/06/2023      _x86_64_        (4 CPU)

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
          23.77    0.01    3.00    0.01    0.06   73.15

Device:         rrqm/s   wrqm/s     r/s     w/s    rkB/s    wkB/s avgrq-sz avgqu-sz   await r_await w_await  svctm  %util
vda               0.00     0.14    0.24    0.65    13.21    26.05    87.90     0.07   81.40   23.35  102.71   1.54   0.14
vdb               0.00     0.02    0.07    0.11     2.70     8.85   129.55     0.02   97.27    2.67  154.44   1.82   0.03
dm-0              0.00     0.00    0.00    0.10     0.02     0.41     8.01     0.07  614.59    1.40  640.30   0.10   0.00
dm-1              0.00     0.00    0.24    0.69    13.19    26.52    85.35     0.09   93.32   23.74  117.11   1.15   0.11

rrqm/s: 每秒对该设备的读请求被合并次数，文件系统会对读取同块(block)的请求进行合并
wrqm/s: 每秒对该设备的写请求被合并次数

注: 当系统调用需要读取数据的时候，VFS 将请求发到各个 FS，如果 FS 发现不同的读取请求读取的是相同 Block 的数据，FS 会将这个请求合并 merge

建议关注的部分：
r/s: 每秒完成的读次数
w/s: 每秒完成的写次数
rkB/s: 每秒读数据量(kB为单位)
wkB/s: 每秒写数据量(kB为单位)
avgqu-sz: 平均等待处理的 IO 请求队列长度
await: 平均每次 IO 请求等待时间(包括等待时间和处理时间，毫秒为单位)
r_await: 每个读请求耗费的平均时间
w_await: 每个写请求耗费的平均时间
util: 采用周期内用于 IO 操作的时间比率，即 IO 队列非空的时间比率。表示该设备的繁忙程度。例如，如果统计间隔1秒，该设备有0.5秒在处理IO，而0.5秒闲置，则该设备的%util = 0.5/1 = 50%。一般地，如果该参数是100%表示设备已经接近满负荷运行。

avgrq-sz: 平均每次 IO 操作的数据量(扇区数为单位)
svctm: 平均每次 IO 请求的处理时间(毫秒为单位)
```

- 如果 %iowait 的值过高，表示磁盘存在 I/O 瓶颈。
- 如果 %util 接近 100%，说明产生的 I/O 请求太多，I/O 系统已经满负荷，该磁盘可能存在瓶颈。
- 如果 svctm 比较接近 await，说明 I/O 几乎没有等待时间；
- 如果 await 远大于 svctm，说明 I/O 队列太长，I/O 响应太慢，则需要进行必要优化。
- 如果 avgqu-sz 比较大，也表示有大量 IO 在等待。
