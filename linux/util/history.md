# 0x00. 导读

在命令前额外多加一个 空格，这样的命令是不会被记录到历史记录的，感觉是不是很酷

# 0x01. 简介

Linux 命令的历史记录，会持久化存储，默认位置是当前用户家目录的 `.bash_history` 文件。

当 Linux 系统启动一个 Shell 时，Shell 会从 `.bash_history` 文件中，读取历史记录，存储在相应内存的缓冲区中。

我们平时所操作的 Linux 命令，都会记录在 **缓冲区** 中。而不是直接操作 `.bash_history` 文件。

当我们退出 Shell，比如按下 Ctrl+D 时，Shell 进程会把历史记录缓冲区的内容，写回到 `.bash_history` 文件中去。

# 0x02. 

## 2.1 选项

正常情况下，只有在 Shell 正常退出时，才会将缓冲区内容保存到文件。如果你想主动保存缓冲区的历史记录，执行 -w 选项即可

`$ history -w`

当然，如果你执行了一些敏感的命令操作，可以执行 -c 将缓冲区内容直接删除

`$ history -c`

## 2.2 设置

命令行修改只在当前 Shell 环境生效，如果需要永久生效，需要写入配置文件
```bash
$ echo "export HISTSIZE=10000" >> ~/.bash_profile
```

显示时间戳  
```bash
$ export HISTTIMEFORMAT='%F %T '
$ history 3
 46  2021-04-18 15:21:33 curl baidu.com
 47  2021-04-18 15:21:35 pwd
 48  2021-04-18 15:21:39 history 3
```
进阶版详细
```bash
$ export HISTTIMEFORMAT="%F %T `who -u am i 2>/dev/null| awk '{print $NF}'|sed \-e 's/[()]//g'` `whoami` "
  784  2023-12-11 10:15:07 192.168.1.104 alfred netstat -ant | grep 61001
  785  2023-12-11 10:15:07 192.168.1.104 alfred df -h
  786  2023-12-11 10:15:07 192.168.1.104 alfred ip a
```