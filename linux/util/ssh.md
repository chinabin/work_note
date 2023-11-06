# 0x00. 导读

# 0x01. 简介

> SSH 之所以能够保证安全，原因在于它采用了公钥加密。需要指出的是，SSH 只是一种协议，存在多种实现，既有商业实现，也有开源实现。我们这里主要针对的是 OpenSSH 工具。

当使用 `ssh` 登录机器的时候，整个过程是这样的：
- （1）远程主机收到用户的登录请求，把自己的公钥发给用户。
- （2）用户使用这个公钥，将登录密码加密后，发送回来。
- （3）远程主机用自己的私钥，解密登录密码，如果密码正确，就同意用户登录。

经常用到的免密登录原理是：  
- 用户将自己的公钥储存在远程主机上。  
- 登录的时候，远程主机会向用户发送一段随机字符串，用户用自己的私钥加密后，再发回来。  
- 远程主机用事先储存的公钥进行解密，如果成功，就证明用户是可信的，直接允许登录shell，不再要求密码。

# 0x002. 免密登录

`ssh-keygen` 生成公钥。  
`ssh-copy-id user@host` 将公钥传送到远程主机 host .

`/etc/ssh/sshd_config` 这个文件，检查下面几行前面"#"注释是否取掉。
```bash
RSAAuthentication yes
PubkeyAuthentication yes
AuthorizedKeysFile .ssh/authorized_keys
```

重启远程主机的ssh服务:
```bash
# ubuntu系统
$ service ssh restart

# debian系统
$ /etc/init.d/ssh restart

# 重启ssh服务
$ systemctl restart sshd
```

登录远程主机，并执行命令：创建（如果不存在）配置文件并且将自己的公钥放进去
```
ssh user@host 'mkdir -p .ssh && cat >> .ssh/authorized_keys' < ~/.ssh/id_rsa.pub
```

**注意权限问题:**
```bash
$ chmod 700 ~/.ssh
$ chmod 600 ~/.ssh/authorized_keys
```

```
$ tree ~/.ssh
├── authorized_keys  # 存储客户端公钥
├── id_rsa           # 私钥
├── id_rsa.pub       # 公钥
└──known_hosts       # 存储服务端公钥(防止中间人攻击|变更则有区别)
```

# 0x03. 端口转发

首先，需要理解清楚一件事，`A 可以连接 B` 与 `A 和 B 可以传数据` 的区别：  
- A 和 B 可以传数据，表示 A 可以给 B 发送字节，B 也可以给 A 发
- A 可以连接 B ，表示 A 可以主动发起对 B 的连接，一旦连上了 A 和 B 就可以传数据，即使 B 不可以连接 A 。

这是我之前一直不理解远程端口转发的原因所在。

----

配置文件注意事项：

1. 默认转发到远程主机上的端口绑定的是 127.0.0.1 ，如要绑定 0.0.0.0 需要打开 sshd_config 里的 GatewayPorts 选项。

2. sshd_config 里要打开 AllowTcpForwarding 选项，否则 -R 远程端口转发会失败。

`127.0.0.1` 和 `0.0.0.0` 的区别：  
本机 IP，你可以理解为本机有三块网卡，一块网卡叫做 loopback （这是一块虚拟网卡），另外一块网卡叫做 ethernet （这是你的有线网卡），另外一块网卡叫做 wlan （这是你的无线网卡）。

你的本机 IP 是你真实网卡的 IP，具体来说有线无线各有一个，而 127.0.0.1 是那块叫做 loopback 的虚拟网卡的 IP。

0.0.0.0 代表本机的所有 IP 地址。

## 3.1 本地转发

**本地转发是在本地计算机建立的转发规则。**  

指定一个本地端口 (local-port) ，所有发向那个端口的请求，都会转发到 SSH 跳板机 (tunnel-host) ，然后 SSH 跳板机作为中介，将收到的请求发到目标服务器 (target-host) 的目标端口 (target-port) 。

假定 host1 是 **本地主机** ， host2 是远程主机。由于种种原因，这两台主机之间无法连通。但是，另外还有一台 host3 ，可以同时连通（ Host1 和 Host3 可以互相通信，Host2 和 Host3 可以互相通信）前面两台主机。因此，很自然的想法就是，通过 host3 ，将 host1 连上 host2 。

![1](../../pic/util/ssh-authenticity-and-use-1.png)

```bash
# 连接 host1 的 2121 端口，就等于连上了 host2 的 21 端口。
$ ssh -L 2121:host2:21 host3
# [bind_address:]本地端口:目标主机:目标主机端口 user@server(传说中的 跳板机)
# [bind_address:] 是执行 ssh 命令的机器的 [bind_address:]
```

注意，假设命令 `$ ssh -L 1234:localhost:21 MM` ，localhost 是 MM 的

![Alt text](../../pic/linux/ssh_L.png)

如果经常使用本地转发，可以将设置写入 SSH 客户端的用户个人配置文件 (`~/.ssh/config`) 。
```bash
Host test.example.com
LocalForward client-IP:client-port server-IP:server-port
```

## 3.2 远程转发

**远程转发指的是在远程 SSH 服务器建立的转发规则。**

继续前面的例子，现在特殊情况出现了， host3 可以连 host1 ，但是反过来就不行，现在的目标还是希望 Host1 能访问 Host2 。

![2](../../pic/util/ssh-authenticity-and-use-2.png)

解决办法是，既然 host3 可以连 host1 ，那么就从 host3 上建立与 host1 的 SSH 连接，然后在 host1 上使用这条连接就可以了（因为 Host3 一旦连上了 Host1 那么它们两者之间就可以互发数据了，自然，发给 Host1 的数据就可以发给 Host3 ，然后 Host3 就发给 Host2 ）。

我们在 host3 执行下面的命令：
```bash
# 访问 host1 的 2121 端口，所有数据经由 host3 ，转发到 host2 的 21 端口。
$ ssh -R 2121:host2:21 host1
# [bind_address:]:远程主机端口:目标主机:目标主机端口 远程主机 。对于 远程主机:远程主机端口 的访问，等于访问 目标主机:目标主机端口
# [bind_address:] 是远程主机的 [bind_address:]
```

再来一个例子，内网某台服务器 localhost 在 80 端口开了一个服务，可以通过远程转发将这个 80 端口，映射到具有公网 IP 地址的 my.public.server 服务器的 8080 端口，使得访问 my.public.server:8080 这个地址，就可以访问到那台内网服务器的 80 端口。  

在内网localhost服务器上执行命令：
```bash
$ ssh -R 8080:localhost:80 -N my.public.server
```
建立从 localhost 到 my.public.server 的 SSH 隧道。运行以后，用户访问 my.public.server:8080 ，就会自动映射到 localhost:80 。

![Alt text](../../pic/linux/ssh_R.png)

## 3.3 动态转发
> 可以将在本地主机 A 发起的请求，转发到远程主机 B ，由 B 去真正地发起请求。  
> 比如，请求地址为 192.168.1.100:3000 ，则通过 SSH 动态转发的请求地址也是 192.168.1.100:3000 。

```bash
$ ssh -D 8080 host
# [本地主机:]本地主机端口
```

# 四、SSH 其它参数

```bash
# -N: 不发送任何命令，只用来建立连接。没有这个参数，会在 SSH 服务器打开一个 Shell。
# -T 参数，表示不为这个连接分配 TTY。
# 这个两个参数可以放在一起用，代表这个SSH连接只用来传数据，不执行远程操作。
$ ssh -NT 8080 host
```

```bash
# -f: 将 SSH 连接放到后台。没有这个参数，暂时不用 SSH 连接时，终端会失去响应。
# 这样一来，你就可以在不中断 SSH 连接的情况下，在本地 shell 中执行其他操作。
$ ssh -f 8080 host
```

# 0x04. 配置

```bash
ssh -L <local host>:<local port>:<remote host>:<remote port> <user>@<hostname>

Host server
    HostName <hostname>
    User <user>
    LocalForward <local host>:<local port> <remote host>:<remote port>



ssh -R <remote host>:<remote port>:<local host>:<local port> <user>@<hostname>

Host server
    HostName <hostname>
    User <user>
    RemoteForward <remote host>:<remote port> <local host>:<local port>


ssh -D <local host>:<local port> <user>@<hostname>

Host server
    HostName <hostname>
    User <user>
    DynamicForward <local host>:<local port>
```