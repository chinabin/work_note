# 0x00. 导读

# 0x01. 简介

Mosh 最大的特点是基于 UDP 方式传输，支持在服务端创建一个临时的 Key 供客户端一次性连接，退出后失效；也支持通过SSH的配置进行认证，但数据传输本身还是自身的UDP方式。

另外，Mosh还有两个我觉得非常有用的功能

- 会话的中断不会导致当前正在前端执行的命令中断，相当于你所有的操作都是在screen命令中一样在后台执行。
- 会话在中断过后，不会立刻退出，而是启用一个计时器，当网络恢复后会自动重新连接，同时会延续之前的会话，不会重新开启一个。

# 0x02. 

```bash
# 没有更改ssh 22 端口的用法
$ mosh root@192.168.99.99

# 没有更改ssh 22 端口，但是想用服务端某个特定的端口
# 这种情况是在防火墙上面只开启了 60000-61000 中间的 1 个或几个端口比如只开启了 60010 这一个端口，那么用的时候在客户端上后面要加 -p 的参数
$ mosh -p 60010 root@192.168.99.99

# 如果改动过ssh端口 22 改为 9999
$ mosh --ssh="ssh -p 9999" root@192.168.99.99

# 如果改动过ssh端口 22 改为9999 还要用服务端 60010 udp 端口
$ mosh -p 60010 --ssh="ssh -p 9999" root@192.168.99.99

# 不用密码使用的秘钥登录
$ mosh --ssh="/usr/bin/ssh -i /home/ivo/.ssh/id_rsa" root@192.168.99.99

# 不用密码使用的秘钥登录,改 22 为 9999
$ mosh --ssh="/usr/bin/ssh -i /home/ivo/.ssh/id_rsa -p 9999" root@192.168.99.99

# 不用密码使用的秘钥登录,改 22 为 9999,且使用服务端 60010 udp 端口
$ mosh -p 60010 --ssh="/usr/bin/ssh -i /home/ivo/.ssh/id_rsa -p 9999" root@192.168.99.99
```