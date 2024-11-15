# 0x00. 导读

# 0x01. 简介

# 0x02. 

```
1（HUP）：重新加载进程。
9 (KILL)：杀死进程。
15（TERM）：完美地停止一个进程。
```

## 2.1 kill


kill pid // 发送信号 15 安全停止进程

## 2.1 pkill

当 pkill 命令行语法中不包含任何信号时，使用的缺省信号为 –15 (SIGKILL)。

pkill 其实是用 pgrep 去找到进程 kill 的。pgrep 找到目标进程是通过 /proc/[pid]/stat 文件。这个文件中的进程名字其实是只有 **15** 个字符长度的。

```bash
$ pkill -u mark,danny //结束mark,danny用户的所有进程
$ w  //#使用w命令查询本机已经登录的用户
$ pkill -9 -t pts/1  //#强制杀死从pts/1虚拟终端登陆的进程
$ pkill -xe SkyRoadTD  // x 精确匹配，e 显示被杀的进程
```