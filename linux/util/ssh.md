# 一、简介

> SSH 之所以能够保证安全，原因在于它采用了公钥加密。需要指出的是，SSH 只是一种协议，存在多种实现，既有商业实现，也有开源实现。我们这里主要针对的是 OpenSSH 工具。

当使用 `ssh` 登录机器的时候，整个过程是这样的：
- （1）远程主机收到用户的登录请求，把自己的公钥发给用户。
- （2）用户使用这个公钥，将登录密码加密后，发送回来。
- （3）远程主机用自己的私钥，解密登录密码，如果密码正确，就同意用户登录。

经常用到的免密登录原理是：  
- 用户将自己的公钥储存在远程主机上。  
- 登录的时候，远程主机会向用户发送一段随机字符串，用户用自己的私钥加密后，再发回来。  
- 远程主机用事先储存的公钥进行解密，如果成功，就证明用户是可信的，直接允许登录shell，不再要求密码。

# 二、常用命令和配置

`ssh-keygen` 生成公钥。  
`ssh-copy-id user@host` 将公钥传送到远程主机 host .

`/etc/ssh/sshd_config` 这个文件，检查下面几行前面"#"注释是否取掉。
```
RSAAuthentication yes
PubkeyAuthentication yes
AuthorizedKeysFile .ssh/authorized_keys
```

重启远程主机的ssh服务:
```
　　// ubuntu系统
　　service ssh restart

　　// debian系统
　　/etc/init.d/ssh restart

# 重启ssh服务
$ systemctl restart sshd
```

登录远程主机，并执行命令：创建（如果不存在）配置文件并且将自己的公钥放进去
```
ssh user@host 'mkdir -p .ssh && cat >> .ssh/authorized_keys' < ~/.ssh/id_rsa.pub
```

**注意权限问题:**
```
chmod 700 ~/.ssh
chmod 600 ~/.ssh/authorized_keys
```

```
$ tree ~/.ssh
├── authorized_keys  # 存储客户端公钥
├── id_rsa           # 私钥
├── id_rsa.pub       # 公钥
└──known_hosts       # 存储服务端公钥(防止中间人攻击|变更则有区别)
```

# 三、转发

## A. 本地转发

> 指定一个本地端口（local-port），所有发向那个端口的请求，都会转发到 SSH 跳板机（tunnel-host），然后 SSH 跳板机作为中介，将收到的请求发到目标服务器（target-host）的目标端口（target-port）。

假定 host1 是 **本地主机** ， host2 是远程主机。由于种种原因，这两台主机之间无法连通。但是，另外还有一台 host3 ，可以同时连通前面两台主机。因此，很自然的想法就是，通过 host3 ，将 host1 连上 host2 。

![1](../../pic/util/ssh-authenticity-and-use-1.png)

```bash
# 连接 host1 的 2121 端口，就等于连上了 host2 的 21 端口。
$ ssh -L 2121:host2:21 host3
# [本地IP:]本地端口:目标主机:目标主机端口 user@server(传说中的 跳板机)
```

**注意：**
```bash
# localhost 是 host3 。因为目标主机是相对 host3 而言的。
$ ssh -L 5900:localhost:5900 host3
```


## B. 远程转发

> 所谓远程端口转发，就是将发送到远程端口的请求，转发到目标端口。这样，就可以通过访问远程端口，来访问目标端口的服务。

host1 与 host2 之间无法连通，必须借助 host3 转发。
但是，特殊情况出现了， host3 是一台内网机器，它可以连接外网的 host1 ，但是反过来就不行，外网的 host1 连不上内网的 host3 。

![2](../../pic/util/ssh-authenticity-and-use-2.png)

解决办法是，既然 host3 可以连 host1 ，那么就从 host3 上建立与 host1 的 SSH 连接，然后在 host1 上使用这条连接就可以了。

我们在 host3 执行下面的命令：
```bash
# 访问 host1 的 2121 端口，所有数据经由 host3 ，转发到 host2 的 21 端口。
# 对于 host3 来说， host1 是远程主机
$ ssh -R 2121:host2:21 host1
# [远程IP]:远程主机端口:目标主机:目标主机端口
```

## C. 动态转发
> 可以将在本地主机 A 发起的请求，转发到远程主机 B ，由 B 去真正地发起请求。  
> 比如，请求地址为 192.168.1.100:3000 ，则通过 SSH 动态转发的请求地址也是 192.168.1.100:3000 。

```bash
$ ssh -D 8080 host
# [本地主机:]本地主机端口
```

# 四、SSH 其它参数

```bash
# N 参数，表示只连接远程主机，不打开远程 shell；
# T 参数，表示不为这个连接分配 TTY。
# 这个两个参数可以放在一起用，代表这个SSH连接只用来传数据，不执行远程操作。
$ ssh -NT 8080 host
```

```bash
# f 参数，表示 SSH 连接成功后，转入后台运行。
# 这样一来，你就可以在不中断 SSH 连接的情况下，在本地 shell 中执行其他操作。
$ ssh -f 8080 host
```