# 0x00. 导读

# 0x01. 简介

lsof (list open files) 是一个列出当前系统打开文件的工具。

# 0x02. 命令

字段解释可以看 `man lsof` 然后搜索关键字. 或者 [lsof](https://wangchujiang.com/linux-command/c/lsof.html)

```bash
# 找出谁在使用某个文件
$ lsof /usr/sbin/httpd

# 递归查找某个目录中所有打开的文件
$ lsof +D /usr/local

# 列出某个用户打开的所有文件
$ lsof -u zhangy

# 查找某个程序打开的所有文件
$ lsof -c LTTBif

# 列出所有由某个PID对应的进程打开的文件
$ lsof -p 3738 -P   # 后面的 -P 可以显示数字，而不是字符，例如 *:inovaport1

# 列出所有网络连接
$ lsof -i
$ lsof -i tcp
$ lsof -i @192.168.1.104
$ lsof -i:80
$ lsof -i udp:53
```

```bash
# 只返回 PID
$ kill -9 `lsof -t -u daniel`
```