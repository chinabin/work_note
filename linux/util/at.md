# 0x00. 导读

# 0x01. 简介

在指定时间执行一个任务

注意，需要 `atd` 启动
```bash
# 激活的方法
$ /etc/init.d/atd start
$ /etc/init.d/atd restart

# 
$ chkconfig atd on
```

# 0x02. 

```
at [选项] [日期时间]

-f：指定包含具体指令的任务文件
-q：指定新任务的队列名称
-l：显示待执行任务的列表
-d：删除指定的待执行任务
-m：任务执行完成后向用户发送 E-mail
```

atq 查看任务队列  
atrm n 删除任务队列的某个任务

要以 root 身份运行 at 作业，只需将 sudo 与你的 at 命令一起使用：
```bash
$ sudo at
```

# 0x03. 实例

Ctrl + d 结束
```bash
# 一分钟后执行 ls
$ at now +1 minute
at> /bin/ls
at> <EOT>
```

```bash
$ at 10:56
```

-t 选项，以这个格式 [[CC]YY]MMDDhhmm[.ss] 指定一个时间和日期。
```bash
$ at -t 202401141047.32
at> touch /data/work_note/test.txt
at> <EOT>
```

计划任务在 12:30 10 21 2020 运行：
```bash
$ at 12:30 102120
```

# 0x04. 其它

`/etc/at.deny` 和 `/etc/at.allow` 文件允许你控制哪些用户有权使用at或者batch命令来创建任务。这个文件包含了一列用户名，一个用户一行。

默认情况下，仅仅 /etc/at.deny 文件存在，并且是空的，这意味着所有用户可以使用at命令。如果你想禁止某一个用户的权限，将他的用户名添加到这个文件中。

如果 /etc/at.allow 文件存在，仅仅只有在该文件中列出的用户才能使用at命令。

如果这两个文件都不存在，那么仅仅只有有管理权限的用户可以使用at命令。