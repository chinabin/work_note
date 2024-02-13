# 0x00. 导读

# 0x01. 简介

[Hex Packet Decoder](https://hpd.gasmi.net/)

应用层数据 --->  
tcp/udp头部(20字节) + 应用层数据 --->  
ip头部(20字节) + tcp/udp头部(20字节) + 应用层数据 --->  
以太网头部(18字节) + ip头部(20字节) + tcp/udp头部(20字节) + 应用层数据

# 0x02. 

`sk_buff`:This is where a packet is stored

`net_device`: Each network device is represented in the Linux kernel by this data structure, which contains information about both its hardware and its software configuration. 

`sock`: stores the networking information for sockets

![Alt text](../../pic/linux/net/7protocols.png)