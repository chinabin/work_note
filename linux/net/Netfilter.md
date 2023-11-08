# 0x00. 导读

# 0x01. 简介

# 0x02. 

Netfilter 是报文在内核协议栈必然会通过的路径，我们从下面这张图就可以看到，Netfilter 在内核的 5 个地方设置了 HOOK 点，用户可以通过配置 iptables 规则，在 HOOK 点对报文进行过滤、修改等操作。

![Alt text](../../pic/linux/net/netfilter_hook.png)

