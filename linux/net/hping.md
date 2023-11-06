# 0x00. 导读

# 0x01. 简介

HPING和ping的区别：典型ping程序使用的是ICMP回显请求来测试，而HPING可以使用任何IP报文，包括ICMP、TCP、UDP、RAWSOCKET。

Hping的主要功能有：

- 防火墙测试
- 实用的端口扫描
- 网络检测，可以用不同的协议、服务类型(TOS)、IP分片
- 手工探测MTU(最大传输单元)路径
- 先进的路由跟踪，支持所有的协议
- 远程操作系统探测
- 远程的运行时间探测
- TCP/IP堆栈审计

# 0x02. 选项

```bash
-c --count 指定数据包的次数
-i --interval 指定发包间隔为多少毫秒，如 - i m10：表示发包间隔为 10 毫秒 (附: 秒、毫秒、微秒进率。1s=1000ms(毫秒)=1000000(微秒)，1s=103ms(毫秒)=106μs(微秒))
–fast 与 - i m100 等同，即每秒钟发送 10 个数据包 (hping 的间隔 u 表示微妙，－－fast 表示快速模式，一秒 10 个包。)
-n --numeric 指定以数字形式输出, 表示不进行名称解析。
-q --quiet 退出 Hping
-I --interface 指定 IP，如本机有两块网卡，可通过此参数指定发送数据包的 IP 地址。如果不指定则默认使用网关 IP
-V --verbose 详细模式, 一般显示很多包信息。
-D --debug 定义 hping 使用 debug 模式。
-z --bind 将 ctrl+z 绑定到 ttl，默认使用 DST 端口
-Z --unbind 解除 ctrl+z 的绑定

指定所用的模式：
(缺省使用 TCP 进行 PING 处理)
-0 --rawip 裸 IP 方式, 使用 RAWSOCKET 方式。
-1 --icmp ICMP 模式
-2 --udp UDP 模式
-8 --scan 扫描模式. (例: hping --scan 1-30,70-90 -S www.target.host)
-9 --listen 监听模式，会接受指定的信息。侦听指定的信息内容。

IP 选项：

-a --spoof 源地址欺骗
–rand-dest 随机目的地址模式
–rand-source 随机源目的地址模式
-t --ttl ttl 值，默认为 64
-N --id 指定 id，默认是随机的
-W --winid 使用 win * 的 id 字节顺序, 针对不同的操作系统。
-r --rel 相对的 id 区域
-f --frag 将数据包分片后传输 (可以通过薄弱的 acl(访问控制列表))
-x --morefrag 设置更多的分片标记
-y --dontfrag 设置不加分片标记
-g --fragoff 设置分片偏移
-m --mtu 设置虚拟 MTU, 当数据包 > MTU 时要使用–frag 进行分片
-o --tos 指定服务类型，默认是 0x00,，可以使用–tos help 查看帮助
-G --rroute 包含 RECORD_ROUTE 选项并且显示路由缓存
–lsrr 释放源路记录
–ssrr 严格的源路由记录
-H --ipproto 设置协议范围，仅在 RAW IP 模式下使用

ICMP 选项

-C --icmptype 指定 icmp 类型（默认类型为回显请求）
-K --icmpcode 指定 icmp 编码（默认为 0）
–force-icmp 发送所有 ICMP 数据包类型（默认只发送可以支持的类型） --icmp-gw 针对 ICMP 数据包重定向设定网关地址（默认是 0.0.0.0）
–icmp-ts 相当于–icmp --icmptype 13（ICMP 时间戳）
–icmp-addr 相当于–icmp --icmptype 17（ICMP 地址掩码）
–icmp-help 显示 ICMP 的其它帮助选项

UDP/TCP 选项

-s --baseport 基本源端口（默认是随机的）
-p --destport 目的端口（默认为 0），可同时指定多个端口
-k --keep 仍然保持源端口
-w --win 指定数据包大小，默认为 64
-O --tcpoff 设置假的 TCP 数据偏移
-Q --seqnum 仅显示 TCP 序列号
-b --badcksum 尝试发送不正确 IP 校验和的数据包 (许多系统在发送数据包时使用固定的 IP 校验和，因此你会得到不正确的 UDP/TCP 校验和.)
-M --setseq 设置 TCP 序列号
-L --setack 使用 TCP 的 ACK（访问控制列表）
-F --fin 使用 FIN 标记 set FIN flag
-S --syn 使用 SNY 标记
-R --rst 使用 RST 标记
-P --push 使用 PUSH 标记
-A --ack 使用 ACK 标记
-U --urg 使用 URG 标记
-X --xmas 使用 X 未用标记 (0x40)
-Y --ymas 使用 Y 未用标记 (0x80)
–tcpexitcode 最后使用 tcp->th_flags 作为退出代码
–tcp-timestamp 启动 TCP 时间戳选项来猜测运行时间

常规选项

-d --data 数据大小，默认为 0
-E --file 从指定文件中读取数据
-e --sign 增加签名
-j --dump 以十六进行形式转存数据包
-J --print 转存可输出的字符
-B --safe 启用安全协议
-u --end 当通过 - -file 指定的文件结束时停止并显示，防止文件再从头开始
-T --traceroute 路由跟踪模式
–tr-stop 在路由跟踪模式下当收到第一个非 ICMP 数据包时退出
–tr-keep-ttl 保持源 TTL，对监测一个 hop 有用
–tr-no-rtt 使用路由跟踪模式时不计算或显示 RTT 信息

ARS 数据包描述 (新增加的内容，暂时还不稳定)
–apd-send 发送用描述 APD 的数据包
```