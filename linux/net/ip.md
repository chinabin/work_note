# 0x00. 导读

# 0x01. 简介

# 0x02. 

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