# 简介

sar是 System Activity Reporter （系统活动情况报告）的缩写。 sar 工具将对系统当前的状态进行取样，然后通过计算数据和比例来表达系统的当前运行状态。 

它有两种用法:   
- 追溯过去的统计数据（默认）   
- 周期性的查看当前数据

> 怀疑 CPU 存在瓶颈，可用 -u 和 -q 等来查看   
> 怀疑内存存在瓶颈，可用 -B、 -r 和 -W 等来查看   
> 怀疑 I/O 存在瓶颈，可用 -b、 -u 和 -d 等来查看


# 用法

```bash
主选项和报告：
	-b	I/O 和传输速率信息状况
	-B	分页状况
	-d	块设备状况
	-I { <中断> | SUM | ALL | XALL }
		中断信息状况
	-m	电源管理信息状况
	-n { <关键词> [,...] | ALL }
		网络统计信息
		关键词可以是：
		DEV	 	网卡
		EDEV	网卡 (错误)
		SOCK	Sockets (套接字)	(v4)
		IP		IP  流	      (v4)
		EIP	 	IP 流	   (v4) (错误)
		TCP	 	TCP 流  (v4)
		ETCP	TCP 流  (v4) (错误)
		UDP	 	UDP 流  (v4)
	-q	队列长度和平均负载
	-r	内存利用率
	-R	内存状况
	-S	交换空间利用率
	-u [ ALL ] CPU 利用率
	-v	Kernel table 状况
	-w	任务创建与系统转换统计信息
	-W	交换信息
	-y	TTY 设备状况
	-o {<文件路径>}
	   将命令结果以二进制格式存放在指定文件中
```

## 网络

```bash
# sar -n <关键词> [ <时间间隔> [ <次数> ] ]
$ sar -n DEV 1 5
01:32:59 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
01:33:00 PM      eth0    158.00    107.00     15.31      8.93      0.00      0.00      0.00
01:33:00 PM        lo     50.00     50.00      3.56      3.56      0.00      0.00      0.00
01:33:00 PM virbr0-nic     0.00      0.00      0.00      0.00      0.00      0.00      0.00
01:33:00 PM    virbr0      0.00      0.00      0.00      0.00      0.00      0.00      0.00

# IFACE 本地网卡接口的名称
# rxpck/s 每秒钟接受的数据包
# txpck/s 每秒钟发送的数据包
# rxKB/S 每秒钟接受的数据包大小，单位为KB
# txKB/S 每秒钟发送的数据包大小，单位为KB
# rxcmp/s 每秒钟接受的压缩数据包
# txcmp/s 每秒钟发送的压缩包
# rxmcst/s 每秒钟接收的多播数据包
```

## CPU

```bash
# sar -u [ <时间间隔> [ <次数> ] ]
$ sar -u 1 5
01:35:33 PM     CPU     %user     %nice   %system   %iowait    %steal     %idle
01:35:34 PM     all      5.33      0.00     11.68      0.00      0.00     82.99
Average:        all      5.33      0.00     11.68      0.00      0.00     82.99

$ sar -P ALL 1 1

# %user：用户模式下消耗的CPU时间的比例
# %system：系统模式下消耗的CPU时间的比例
# %iowait：CPU等待磁盘I/O导致空闲状态消耗的时间比例
# %idle：显示 CPU 空闲时间占用 CPU 总时间的百分比

#    1. 若 %iowait 的值过高，表示硬盘存在I/O瓶颈
#    2. 若 %idle 的值高但系统响应慢时，有可能是 CPU 等待分配内存，此时应加大内存容量
#    3. 若 %idle 的值持续低于1，则系统的 CPU 处理能力相对较低，表明系统中最需要解决的资源是 CPU
```

## 内存

## 内存利用率

```bash
$ sar -r 1 3

# kbmemfree：这个值和free命令中的free值基本一致,所以它不包括buffer和cache的空间.
# kbmemused：这个值和free命令中的used值基本一致,所以它包括buffer和cache的空间.
# %memused：物理内存使用率，这个值是kbmemused和内存总量(不包括swap)的一个百分比.
# kbbuffers和kbcached：这两个值就是free命令中的buffer和cache.
# kbcommit：保证当前系统所需要的内存,即为了确保不溢出而需要的内存(RAM+swap).
# %commit：这个值是kbcommit与内存总量(包括swap)的一个百分比.
```

## 内存分页情况

```bash
$ sar -B 1 3

# pgpgin/s：表示每秒从磁盘或SWAP置换到内存的字节数(KB)
# pgpgout/s：表示每秒从内存置换到磁盘或SWAP的字节数(KB)
# fault/s：每秒钟系统产生的缺页数，即主缺页与次缺页之和(major + minor)
# majflt/s：每秒钟产生的主缺页数
# pgfree/s：每秒被放入空闲队列中的页个数
# pgscank/s：每秒被 kswapd 扫描的页个数
# pgscand/s：每秒直接被扫描的页个数
# pgsteal/s：每秒钟从 cache 中被清除来满足内存需要的页个数
# %vmeff：每秒清除的页(pgsteal)占总扫描页(pgscank + pgscand)的百分比
```

## I/O

```bash
$ sar -b 1 3

# tps：每秒钟物理设备的 I/O 传输总量
# rtps：每秒钟从物理设备读入的数据总量
# wtps：每秒钟向物理设备写入的数据总量
# bread/s：每秒钟从物理设备读入的数据量，单位为：块/s
# bwrtn/s：每秒钟向物理设备写入的数据量，单位为：块/s
```

## 队列长度和平均负载

```bash
$ sar -q 1 3
```
```bash
# runq-sz：运行队列的长度（等待运行的进程数）
# plist-sz：进程列表中进程（processes）和线程（threads）的数量
# ldavg-1 5 15：最后1 5 15分钟的系统平均负载（System load average）
```

## swap 换页信息

```bash
$ sar -W 1 3
```
```bash
# pswpin/s：每秒系统换入的交换页面（swap page）数量
# pswpout/s：每秒系统换出的交换页面（swap page）数量
```

## 块设备状况

```bash
$ sar -d 1 3
```
```bash
# tps: 每秒从物理磁盘 I/O 的次数。多个逻辑请求会被合并为一个 I/O 磁盘请求，一次传输的大小是不确定的
# rd_sec/s: 每秒读扇区的次数
# wr_sec/s: 每秒写扇区的次数
# avgrq-sz: 平均每次设备 I/O 操作的数据大小(扇区)
# avgqu-sz: 磁盘请求队列的平均长度
# await: 从请求磁盘操作到系统完成处理，每次请求的平均消耗时间，包括请求队列等待时间，单位是毫秒(1秒=1000毫秒)
# svctm: 系统处理每次请求的平均时间,不包括在请求队列中消耗的时间.
# %util: I/O请求占CPU的百分比，比率越大，说明越饱和
#     1. avgqu-sz 的值较低时，设备的利用率较高
#     2. 当%util的值接近 1% 时，表示设备带宽已经占满
```

## 其它

```bash
# 输出统计的数据信息
$ sar -o sarfile.log -u 1 3

# 将二进制数据文件转换成数据库可读的格式。
$ sadf -d sarfile.log
$ sadf -d sarfile.log | sed 's/;/,/g' > sarfile.csv

# 从数据文件读取信息
$ sar -f sarfile.log
```