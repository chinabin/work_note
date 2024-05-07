# 0x00. 导读

[最全 ps 命令](https://juejin.cn/post/6844903938144075783)  
[shell 监测进程](https://steeed.gitee.io/shell-process-monitor.html)


# 0x01. 简介

ps 命令的选项可以分为以下 3 类：

- BSD 风格语法，不能以中横线开头；例如 `ps u`

- SYSV 风格语法，必须仅一个中横线开头；例如 `ps -l`

- GNU 风格语法，必须以两个中横线开头；例如 `ps --pid l`

也可以混淆（有时候混淆会出错，这里只是为了说明可以混淆）：
```bash
$ ps -el en   # 等价于 ps -e -l e n
$ ps -ef      # 等价于 ps -e -f
$ ps aux      # 等价于 ps a u x
```

# 0x02. 常用

字段含义不清楚可以 `man ps`
```
-e: 显示所有进程信息
-F：以全格式显示详细信息，包括进程的所有字段。
-P: 可以查看这个进程/线程现在是哪个核上运行。
-L: 参数来显示线程。(LWP = 线程 ID)

PSR 字段是指当前进程被调度到的 CPU 核序号。  
C 字段是 CPU 利用率
```

```bash
$ ps -eFL | grep -e PPID -e SkyRoadTD | grep -v grep
UID         PID   PPID    LWP  C NLWP    SZ   RSS PSR STIME TTY          TIME CMD
root     106350      1 106350 99    3 292492 827956 1 08:55 ?        01:12:30 ./SkyRoadTD 2 sr_vendor.aeg
root     106350      1 106359  0    3 292492 827956 0 08:55 ?        00:00:08 ./SkyRoadTD 2 sr_vendor.aeg
root     106350      1 106367  0    3 292492 827956 3 08:55 ?        00:00:00 ./SkyRoadTD 2 sr_vendor.aeg
root     106354 106350 106354 99    3 292492 841232 2 08:55 ?        01:12:30 ./SkyRoadTD 2 sr_vendor.aeg
root     106354 106350 106373  0    3 292492 841232 0 08:55 ?        00:00:06 ./SkyRoadTD 2 sr_vendor.aeg
root     106354 106350 106375  0    3 292492 841232 4 08:55 ?        00:00:00 ./SkyRoadTD 2 sr_vendor.aeg
```