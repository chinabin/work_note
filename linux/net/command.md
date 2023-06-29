# 一、基础知识

## 1.1 性能指标解释

- 带宽，表示链路的最大传输速率，单位是 b/s （比特 / 秒），带宽越大，其传输能力就越强。
- 延时，表示请求数据包发送后，收到对端响应，所需要的时间延迟。不同的场景有着不同的含义，比如可以表示建立 TCP 连接所需的时间延迟，或一个数据包往返所需的时间延迟。
- 吞吐率，表示单位时间内成功传输的数据量，单位是 b/s（比特 / 秒）或者 B/s（字节 / 秒），吞吐受带宽限制，带宽越大，吞吐率的上限才可能越高。
- PPS，全称是 Packet Per Second（包 / 秒），表示以网络包为单位的传输速率，一般用来评估系统对于网络的转发能力。

# 二、命令

## 1. 查看网卡
    命令： ifconfig 或者 ip
    显示网口的配置以及收发数据包的统计信息。

### 1.1 ifconfig
```bash
$ ifconfig [网卡]
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.1.90  netmask 255.255.255.0  broadcast 192.168.1.255
        inet6 fe80::fcfc:feff:fe4b:55a0  prefixlen 64  scopeid 0x20<link>
        ether fe:fc:fe:4b:55:a0  txqueuelen 1000  (Ethernet)
        RX packets 87887372  bytes 22361735202 (20.8 GiB)
        RX errors 0  dropped 3180591  overruns 0  frame 0
        TX packets 20526885  bytes 7943113708 (7.3 GiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

# UP: 网卡开启状态
# BROADCAST: 支持广播。后面的 broadcast 就是广播地址
# RUNNING: 网线被接上了
# MULTICAST: 支持组播
```

### 2.2 ip
```bash
# ip [ OPTIONS ] OBJECT { COMMAND | help }
# OBJECT 常用的是: link/l 网络接口，addr/a IP信息，route/r 路由信息

# -s: 显示详细信息
# 获取指定接口的IP地址信息，ip addr show 接口设备名称
$ ip -s addr show dev eth0
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether fe:fc:fe:4b:55:a0 brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.90/24 brd 192.168.1.255 scope global noprefixroute eth0
       valid_lft forever preferred_lft forever
    inet6 fe80::fcfc:feff:fe4b:55a0/64 scope link 
       valid_lft forever preferred_lft forever
    RX: bytes  packets  errors  dropped overrun mcast   
    22363219262 87893806 0       3180812 0       0       
    TX: bytes  packets  errors  dropped carrier collsns 
    7943600906 20529082 0       0       0       0 
```

第一，网口的连接状态标志。其实也就是表示对应的网口是否连接到交换机或路由器等设备，如果 ifconfig 输出中看到有 RUNNING，或者 ip 输出中有 LOWER_UP，则说明物理网络是连通的，如果看不到，则表示网口没有接网线。

第二，网络包收发的统计信息。通常有网络收发的字节数、包数、错误数以及丢包情况的信息，如果 TX（发送） 和 RX（接收） 部分中 errors、dropped、overruns、carrier 以及 collisions 等指标不为 0 时，则说明网络发送或者接收出问题了，这些出错统计信息的指标意义如下：

- errors 表示发生错误的数据包数，比如校验错误、帧同步错误等；
- dropped 表示丢弃的数据包数，即数据包已经收到了 Ring Buffer（这个缓冲区是在内核内存中，更具体一点是在网卡驱动程序里），但因为系统内存不足等原因而发生的丢包；
- overruns 表示超限数据包数，即网络接收/发送速度过快，导致 Ring Buffer 中的数据包来不及处理，而导致的丢包，因为过多的数据包挤压在 Ring Buffer，这样 Ring Buffer 很容易就溢出了；
- carrier 表示发生 carrirer 错误的数据包数，比如双工模式不匹配、物理电缆出现问题等；
collisions 表示冲突、碰撞数据包数；

-------------

## 2. 查看 socket 
    命令： netstat 或者 ss
    查看 socket、网络协议栈、网口以及路由表的信息。

### 2.1 netstat
```bash
# -a: 显示所有连接
# -n: 不显示别名，以数字形式显示 ip 和端口
# -l: 只显示 listen 状态的 socket
# -u: 显示 udp 连接
# -t: 显示 tcp 连接
# -p: 显示进程信息
$ netstat -nlp
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
tcp        0      0 0.0.0.0:13579           0.0.0.0:*               LISTEN      7536/./LTTBif

# -s: 协议栈统计
$ netstat -s

# -g: 显示组播组的关系。
# 224.0.0.0～239.255.255.255 所有组播地址
# 224.0.0.0～224.0.0.255 为预留的组播地址（永久组地址），地址224.0.0.0保留不做分配，其它地址供路由协议使用；
# 224.0.0.1 同一网段所有主机
# 224.0.0.2 同一网段所有组播路由器
# 224.0.1.0～238.255.255.255 公用组播地址，可以用于Internet；
# 239.0.0.0～239.255.255.255 私网组播地址

$ netstat -g
IPv6/IPv4 Group Memberships
Interface       RefCnt Group
--------------- ------ ---------------------
lo              1      all-systems.mcast.net
ens20f0         1      all-systems.mcast.net
ens20f1         1      all-systems.mcast.net
ens20f2         1      all-systems.mcast.net
ens20f3         1      all-systems.mcast.net
enp130s0f0      1      all-systems.mcast.net
enp130s0f1      1      239.8.8.9
enp130s0f1      1      239.8.8.8
enp130s0f1      1      all-systems.mcast.net
ens2f0          1      224.1.2.12
ens2f0          2      224.1.1.24
ens2f0          3      224.1.2.8
ens2f0          2      224.1.2.11
ens2f0          1      224.1.1.2
ens2f0          1      239.6.6.6
ens2f0          1      all-systems.mcast.net
ens2f1          1      all-systems.mcast.net
lo              1      ff02::1
lo              1      ff01::1
ens20f0         1      ff02::1:ff3a:983c
ens20f0         1      ff02::1
ens20f0         1      ff01::1
ens20f1         1      ff02::1
ens20f1         1      ff01::1
ens20f2         1      ff02::1
ens20f2         1      ff01::1
ens20f3         1      ff02::1
```

### 2.2 ss
```bash
# -a: 显示所有连接
# -n: 不显示别名，以数字形式显示 ip 和端口
# -t: 只显示 tcp 连接
# -u: 只显示 udp 连接
# -l: 只显示 listen 状态的 socket
# -p: 显示进程信息
$ ss -nlpt
State      Recv-Q Send-Q Local Address:Port               Peer Address:Port
LISTEN     0      100          *:13579                    *:*                   users:(("LTTBif",pid=26224,fd=17))

# -s: 协议栈统计
$ ss -s
```

------

## 3. 查看网络吞吐率和 PPS
    命令： sar

### 3.1 ethtool
```bash
# 查看带宽
$ ethtool enp130s0f1 | grep Speed
        Speed: 10000Mb/s
# 注意这里小写字母 b ，表示比特而不是字节。我们通常提到的千兆网卡、万兆网卡等，单位也都是比特（bit）。
# enp130s0f1 是万兆网卡
```

```bash
-i 显示网卡驱动的信息，如驱动的名称、版本等
-S 查看网卡收发包的统计情况
-g/-G 查看或者修改RingBuffer的大小
-l/-L 查看或者修改网卡队列数
-c/-C 查看或者修改硬中断合并策略
```

### 3.2 sar

详见 [sar](../perf/command/sar.md)
```bash
# 显示网口的统计数据；
$ sar -n DEV
# 显示关于网口错误的统计数据；
$ sar -n EDEV
# 显示 TCP 的统计数据
$ sar -n TCP

# rxpck/s 和 txpck/s 分别是接收和发送的 PPS，单位为包 / 秒。
# rxkB/s 和 txkB/s 分别是接收和发送的吞吐率，单位是 KB/ 秒。
# rxcmp/s 和 txcmp/s 分别是接收和发送的压缩数据包数，单位是包 / 秒。
```