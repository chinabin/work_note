# 0x00. 导读

# 0x01. 简介

**OpenSSL** 是一个强大的安全套接字层密码库，囊括主要的密码算法、常用的密钥和证书封装管理功能及 SSL 协议，并提供丰富的应用程序供测试或其它目的使用。

- OpenSSL 功能强大，可以进行加解密
- OpenSSL 也可以进行 PKI 的认证机制

OpenSSL 有两种运行模式

- 交互模式
    - 直接输入 openssl 回车进入交互模式
- 批处理模式
    - 输入带命令选项的 openssl 进入批处理模式

openssl 的三个组件
- openssl
    - 多用途的命令行工具
- libcrypto
    - 公共加密库，实现了各种各样的加密算法
- libssl
    - 也是一个库，实现了 ssl 及 tls

# 0x02. OpenSSL 命令

分为三类

- 标准命令
- 消息摘要命令
- 加密命令

## 2.1 加密例子

enc 是 openssl 的一个子命令，可以用来实现对称加密。更多命令 `man openssl`
- -e加密
- -d解密
- -salt表示添加杂质进去
- -in需要加密或者解密的文件
- -out需要输出的文件文件名称

```bash
[root@localhost tset]# cat mima
I love you

[root@localhost tset]# openssl enc -e -des3 -a -salt -in mima -out jiami
enter des-ede3-cbc encryption password:
Verifying - enter des-ede3-cbc encryption password:

[root@localhost tset]# cat jiami
U2FsdGVkX1+OK5GvMnyeZpST9ncIuIXU9zM+FzdKJmM=

[root@localhost tset]# openssl enc -d -des3 -a -salt -in jiami -out jiemi
enter des-ede3-cbc decryption password:

[root@localhost tset]# cat jiemi
I love you
```