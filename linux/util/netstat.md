# 0x00. 导读

# 0x01. 简介

显示网络连接，路由表，接口状态，伪装连接，网络链路信息和组播成员组。

利用 netstat 指令可让你得知整个 Linux 系统的网络情况。

**在2001年的时候netstat 1.42版本之后就没更新了，之后取代的工具是ss命令**

# 0x02. 命令

```
netstat [-acCeFghilMnNoprstuvVwx][-A<网络类型>][--ip]

无选项时, netstat 显示打开的套接字. 如果不指定任何地址族，那么打印出所有已配置地址族的有效套接字。

--route , -r
显示内核路由表。

--groups , -g
显示IPv4 和 IPv6的IGMP组播组成员关系信息。 

--interface=iface , -i
显示所有网络接口列表或者是指定的 iface 。 

--statistics , -s
显示每种协议的统计信息。 



--numeric , -n
显示数字形式地址而不是去解析主机、端口或用户名。

-c, --continuous
将使 netstat 不断地每秒输出所选的信息。

-p, --programs
显示套接字所属进程的PID和名称。

-l, --listening
只显示正在侦听的套接字(这是默认的选项)

-a, --all
显示所有正在或不在侦听的套接字。加上 --interfaces 选项将显示没有标记的接口。

delay
netstat将循环输出统计信息，每隔 delay 秒。

-u
显示 udp 连接

-t
显示 tcp 连接
```

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