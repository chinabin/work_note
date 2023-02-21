[最全 ps 命令](https://juejin.cn/post/6844903938144075783)

-----

# 一、简介

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

# 二、常用

```bash
# -e: 显示所有进程信息

# -P: 可以查看这个进程/线程现在是哪个核上运行。
# PSR 字段是指当前进程被调度到的 CPU 核序号。
   
# -L: 参数来显示线程。(LWP = 线程 ID)


$ ps -ePL | grep LTTBif
  PID   LWP PSR TTY          TIME CMD
 5707  5707   1 pts/4    00:00:00 LTTBif
 5707  5708   2 pts/4    00:00:00 LTTBif
 5707  5709   3 pts/4    00:00:00 LTTBif
 5707  5710   3 pts/4    00:00:11 LTTBif
 5707  5711   0 pts/4    00:00:10 LTTBif

```