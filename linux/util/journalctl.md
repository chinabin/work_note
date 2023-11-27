# 0x00. 导读

# 0x01. 简介

journalctl 命令来自英文词组 journal control 的缩写，其功能是用于查看指定的日志信息。Systemd 服务统一管理了所有服务的启动日志，带来的好处就是可以只用 journalctl 一个命令，查看到全部的日志信息了。

# 0x02. 详解

默认情况下，journalctl 会在 pager 内显示输出结果。如果大家希望利用文本操作工具对数据进行处理，则需要使用标准输出。在这种情况下，我们需要使用 `--no-pager` 选项。

```bash
$ sudo journalctl --no-pager
```

可以用 `-o` 格式化美化一下，-o 选项支持的类型如下：
```
    short
       这是默认的格式，即经典的 syslog 输出格式。
    short-iso
       与 short 类似，强调 ISO 8601 时间戳。
    short-precise
       与 short 类似，提供微秒级精度。
    short-monotonic
       与 short 类似，强调普通时间戳。
    verbose
       显示全部字段，包括通常被内部隐藏的字段。
    export
       适合传输或备份的二进制格式。
    json
       标准 json 格式，每行一条记录。
    json-pretty
       适合阅读的 json 格式。
    json-sse
       经过包装可以兼容 server-sent 事件的 json 格式。
    cat
       只显示信息字段本身。
```

例如，`journalctl --no-pager -n 10 -o json-pretty`

```
-a 显示所有字段信息 
-b 显示本次系统启动日志 
-f 追踪日志内容 
-k 显示内核日志 
-n 设置日志条数 
-u 显示指定服务的日志
-p 显示具有指定优先级的条目
```

## 2.1 匹配机制

可以通过 "FIELD=VALUE" 的格式来匹配具体的日志记录, 如：
其中 FIELD 可以是 MESSAGE、MESSAGE_ID、_PID、_UID、_HOSTNAME、_SYSTEMD_UNIT 等等(通过 man 7 systemd.journal-fields 可以查看所有可用的 match 字段)
```bash
$ journalctl _SYSTEMD_UNIT=cron.service
```

可以同时添加多个字段进行匹配，它们之间是与的关系，就是同时符合多个条件的记录才会被匹配，比如添加 PRIORITY 字段的匹配条件：
```bash
$ journalctl _SYSTEMD_UNIT=cron.service PRIORITY=6
```
注意各个字段的取值，比如为 PRIORITY 设置 debug、info 是不工作的，必须设置为对应的数字。可以通过 -F 选项来查看某个字段的可选值：
```bash
$ journal -F PRIORITY 
```
具体的对应方式如下：
```
0: emerg
1: alert
2: crit
3: err
4: warning
5: notice
6: info
7: debug
```

```bash
$ sudo journalctl -p err
```

使用 "+" 号可以对多个匹配字段执行或操作：
```bash
$ journalctl _SYSTEMD_UNIT=cron.service + _PID=28097
```
上面的命令会输出 cron.service 的日志和进程 28097 的日志。

## 2.2 日志

systemd-journald 服务收集到的日志默认保存在 `/run/log` 目录中，重启系统会丢掉以前的日志信息。 我们可以通过两种方式让 systemd-journald 服务把所有的日志都保存到文件中，这样重新启动后就不会丢掉以前的日志。
- 方法一：创建目录 /var/log/journal，然后重启日志服务 systemd-journald.service。
    ```bash
    $ sudo mkdir /var/log/journal
    $ sudo chown root:systemd-journal /var/log/journal
    $ sudo chmod 2775 /var/log/journal
    $ sudo systemctl restart systemd-journald.service
    # 之后 /run/log 下面就没有 journal 的日志了，日志文件被保存在 /var/log/journal 目录下
    ```
- 方法二：修改配置文件 /etc/systemd/journald.conf，把 Storage=auto 改为 Storage=persistent，并取消注释，然后重启日志服务 systemd-journald.service。

查看日志占据的磁盘空间
```bash
$ sudo journalctl --disk-usage
```

清理日志

`/var/log/journal` 存放着日志, rm 应该能工作. 或者使用 journalctl, 
```bash
# 硬性指定日志的总体体积，意味着其会不断删除旧有记录直到所占容量符合要求
# 清理日志使总大小小于 100M:
$ journalctl --vacuum-size=100M
# 任何早于这一时间点的条目都将被删除
# 清理最早两周前的日志.
$ journalctl --vacuum-time=2weeks
```

## 2.3 

```bash
# 显示指定单元的所有消息
$ journalctl -u sshd
$ journalctl -u man-db.service

# 查看最近 10 条日志信息
$ journalctl -n 10

# 持续追踪最新的日志信息，保持刷新内容
$ journalctl -f

# 先查看系统重启记录
$ journalctl --list-boots
# 显示本次启动的信息
$ journalctl -b -0
# 显示上次启动的信息
$ journalctl -b -1
# 显示上上次启动的信息
$ journalctl -b -2
$ journalctl -b 9eaabbc25fe343999ef1024e6a16fb58

# 显示从某个日期 ( 或时间 ) 开始的消息
$ journalctl --since="2012-10-30 18:17:16"
# 显示从某个时间 ( 例如 20分钟前 ) 的消息
$ journalctl --since "20 min ago"
# --until 选项设定时间段，负责指定给定时间之后的日志记录
$ $ journalctl --since 09:00 --until "1 hour ago"

# 显示特定进程的所有消息:
$ journalctl _PID=1

# 按可执行文件的路径过滤
# 如果在参数中指定某个可执行文件(二进制文件或脚本文件)，则 journalctl 会显示与该可执行文件相关的全部条目。比如可以显示 /usr/lib/systemd/systemd 程序产生的日志：
$ sudo journalctl /usr/lib/systemd/systemd
#也可以显示 /usr/bin/bash 程序产生的日志：
$ sudo journalctl /usr/bin/bash
```