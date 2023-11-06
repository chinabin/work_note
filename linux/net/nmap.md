# 0x00. 导读

# 0x01. 简介

## 1.1 安装

```bash
$ rpm -vhi https://nmap.org/dist/nmap-7.80-1.x86_64.rpm
```

## 1.2 基本功能 

四项基本功能

- 主机发现（Host Discovery）
- 端口扫描（Port Scanning）
- 应用与版本探测（Version Detection）
- 操作系统侦测（Openrating System Detection）

# 0x02. 功能

首先，我们必需确定我们的目标是在局域网内还是在广域网呢? 当我们与目标身处在同一局域网内时，我们可以直接对其施行完整的端口扫描，无须考虑端口被交换机或路由器屏蔽的问题。

第四层(传输层)，传输层(TPDU,Transport Protocol Data Unit)负责了端对端的连接。该层包含最重要的TCP及UDP协议。

TCP(Transmission Control Protocol)和UDP(User Datagram Protocol)协议皆建立在网络层（第三层）上，而网络层上最常用的协议就是IP。网络层IP提供的是一种不可靠的服务。也就是说，它只是尽可能快地把分组从源结点送到目的结点，但是并不提供任何可靠性保证。虽然TCP使用不可靠的IP服务，但它却提供一种可靠的运输层服务。

TCP内的协议: `HTTP, HTTPs, FTP, SMTP, Telnet`

UDP内的协议: `DNS, DHCP, TFTP, SNMP, RIP, VOIP`


```bash
-A 用于使用进攻性方式扫描
-T4 指定扫描过程使用的时序，（总共6个级别[0-5]），级别越高，扫描速度越快，但容易被防火墙或IDS检测屏蔽掉）。
-v 表示显示冗余信息，显示扫描的细节。
```

## 2.1 扫描网络上的设备（是否在线）

主机发现发现的原理与Ping命令类似，发送探测包到目标主机，如果收到回复，说明目标主机是开启的。

默认情况下： Nmap 会发送四种不同类型的数据包来探测目标主机是否在线。

- ICMP echo request
- a TCP SYN packet to port 443
- a TCP SYN packert to port 80
- an ICMP timestamp request


Namp 扫描的目标可以是 一个主机地址，一个网段，多个地址，或者域名。同时支持 `CIDR` 风格地址（从/0全网到/32当前IP）。支持类似 `10.1-5.1-255.1-255` 的扫描。可同时接收多种类型的目标。

选项：
```bash
-sL：List Scan 列表扫描，仅将指定的目标主机的IP列出来，不进行主机发现。
-sn：Ping Scan 只进行主机发现，不进行端口扫描。
-Pn：将所有指定的主机视作开启的，跳过主机发现的过程。
-PS/PA/PU/PY [portlist]：使用TCPSYN/ACK或SCTP INIT/ECHO方式进行发现。
-PE/PP/PM：使用ICMP echo，timestamp，and netmask 请求包发现主机。
-PO [protocllist]：使用IP协议包探测对方主机是否开启。
-n/-R：-n表示不进行DNS解析；-R表示总是进行DNS解析。
--dns-servers <serv1[，serv2]，...>：指定DNS服务器。
--system-dns：指定使用系统的DNS服务器。
--traceroute：追踪每个路由节点。
```

```bash
nmap scanme.nmap.org
```
该指令将会扫描目标上超过1660个TCP端口。

在Nmap的预设模式中，若当前用户没有足够的权限去使用原生端口， 则会使用 TCP 连接模式(也就是 -sT)。反之，则使用TCP SYN模式(-sS)端口扫描。 

SYN模式比TCP连接模式快(且较为隐密)，因为SYN模式下的Nmap不会完成TCP三次握手连接。所以一次可以对上千个端口快速扫描。

# 2.2 端口扫描

- TCP SYN scanning  

    TCP SYN scanning 是Nmap默认的扫描方式，称作半开放扫描。

    原理：该方式发送SYN到目标端口。

    - 如果收到SYN/ACK回复，那么判断该端口是开发；
    - 如果收到RST包，那么判断该端口是关闭；
    - 如果没收到回复，那么判断该端口是被屏蔽。

- TCP connect scanning

    TCP connect 方式使用系统网络API connect 向目标主机的端口发起连接。

    如果无法连接，说明该端口关闭。  
    优缺点：该方式扫描速度比较慢，而且由于建立完整的TCP连接会在目标机上留下记录信息，不够隐蔽。所以，TCP connect是TCP SYN无法使用才考虑选择的方式。

- TCP ACK scanning

    原理：向目标主机的端口发送ACK包。

    - 如果收到RST包，说明该端口没有被防火墙屏蔽；    
    - 没有收到RST包，说明被屏蔽。  

    优缺点：该方式只能用于确定防火墙是否屏蔽某个端口，可以辅助TCP SYN的方式来判断目标主机防火墙的状况。

- TCP FIN/Xmas/NULL scanning

    原理：FIN扫描向目标主机的端口发送的TCP FIN包或Xmas tree包/Null包

    - 如果收到对方RST回复包，那么说明该端口是关闭的；  
    - 没有收到RST包说明端口可能是开放的或被屏蔽的（open|filtered）。  

    其中Xmas tree包是指flags中FIN URG PUSH被置为1的TCP包；NULL包是指所有flags都为0的TCP包。

- UDP scanning

    UDP扫描方式用于判断UDP端口的情况。

    原理：向目标主机的UDP端口发送探测包。

    - 如果收到回复“ICMP port unreachable”就说明该端口是关闭的；
    - 如果没有收到回复，那说明UDP端口可能是开放的或屏蔽的。



Nmap 扫描后回覆的端口状态有六种：
- open  
    该端口正被应用程序使用。
- closed  
    端口回应了扫描，当前并没有应用程序在使用该端口。
- filtered  
    扫描可能被防火墙设备,路由器规则,或软件防火墙过滤。
- unfiltered  
    扫描未被屏蔽，但Nmap无法决定该端口的状态。这是最少见的情况。
- open/filtered  
    Nmap无法确定端口是开放或屏蔽的。例子:开放的端口不回覆扫描。
- close/filtered  
    Nmap无法确定端口为关闭或未被过滤的。



扫描方式选项：
```bash
-sS/sT/sA/sW/sM：指定使用TCP SYN/Connect()/ACK/Window/Maimon scans的方式对目标主机进行扫描。
-sU：指定使用UDP扫描方式确定目标主机的UDP端口状况。
-sN/sF/sX：指定使用TCP Null,FIN,and Xmas scans 秘密扫描方式来协助探测对方的TCP端口状态。
-sI <zombiehost[:probeport]>：指定使用idle scan方式来扫描目标主机。
-sY/sZ：使用SCTP INIT/COOKIE-ECHO来扫描SCTP协议端口的开放情况。
-sO：使用IP protocol 扫描确定目标主机支持的协议类型。
-b <FTP relay host>：使用FTP bounce scan扫描方式。
--scanflages <flags>：定制TCP包的flags。
```

端口参数与扫描顺序:
```bash
-p <port rages>：扫描指定的端口。
-F：Fast mode 快速模式，仅扫描TOP 100的端口。
-r：不进行端口随机打乱的操。
--top-ports <number>：扫描开放概率最高的number个端口。
--port-ratio <ratio>：扫描指定频率以上的端口。
```
