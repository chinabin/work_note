# 0x00. 导读

# 0x01. 简介

timeout 是用来控制程序运行的时间，运行指定的命令。如果在指定时间后仍在运行，则杀死该进程。使用 timeout 命令可以让程序在指定的时间内仍然运行则强制退出。

# 0x02. 

默认时间单位是 秒 。
```bash
# s 秒
# m 分钟
# h 小时
# d 天
# 5 分钟后结束 ping
$ timeout 5m ping www.baidu.com
```

```bash
# -s : --signal=信号
# 可以用 kill -l 查看信号列表
# 使用 man 7 signal 命令查看系统对每个信号作用的描述：
$ timeout -s SIGKILL 5s ping www.baidu.com
```

```bash
# -k, --kill after
# 当超过时间限制时发送的默认信号可以被某些进程捕获或忽略。在这种情况下，进程在发送终止信号后继续运行。
# 要确保被执行的的命令终止，请使用 -k 选项，后面加一个时间。当达到给定的时间限制后会强制结束。
# timeout命令运行一分钟，如果命令没有结束，将在10秒后终止命令：
$ timeout -k 10s 1m sh test.sh
```

```bash
# 运行在前台
$ timeout --foreground 5m ./script.sh
```