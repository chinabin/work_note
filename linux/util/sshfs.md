# 0x00. 导读

```bash
$ yum install -y epel-release
$ yum install sshfs
```

# 0x01. 简介

SSHFS(Secure SHell FileSystem) 是一个客户端，可以让我们通过 SSH 文件传输协议 (SFTP) 挂载远程的文件系统并且在本地机器上和远程的目录和文件进行交互。

SFTP 是一种通过 SSH 协议提供文件访问、文件传输和文件管理功能的安全文件传输协议。因为 SSH 在网络中从一台电脑到另一台电脑传输文件的时候使用数据加密通道，并且 SSHFS 内置在 FUSE（用户空间的文件系统）内核模块，允许任何非特权用户在不修改内核代码的情况下创建他们自己的文件系统。

也就是将远程文件系统挂载到本地计算机上。

# 0x02. 

sshfs -o OPTION username@host:REMOTE_PATH LOCAL_PATH

```
-C
equivalent to '-o compression=yes'

-o allow_other         allow access to other users
如果不指定本参数，则只有挂载本文件系统的用户才可以访问；如果指定本参数，则允许系统中的其他用户访问；

-o reconnect           reconnect to server
断开连接后，会自动重连远程服务器，对于网络不太稳定的环境比较有用；

-o ro
挂载后，文件内容为只读
```

```bash
$ mkdir /opt/share
$ setfacl -m u:ops:rwx /opt/share
$ setfacl -m u:alfred:rwx /opt/share
$ setfacl -m u:user01:rwx /opt/share
$ sshfs -C -o allow_other,reconnect DS:/opt/shared/ /opt/share &

# 卸载
$ umount /opt/share
```