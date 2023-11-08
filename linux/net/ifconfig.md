# 0x00. 导读

# 0x01. 简介

在 Linux 中, 我们可以通过 `ifconfig -a` 或者 `ip addr` 看到主机上的所有网络接口，它有两种来源，一种是物理网卡的驱动程序创建的，另一种是内核自己或者用户主动创建的虚拟接口。

# 0x02. 

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