# 0x00. 导读

在CentOS7中，实现开机启动程序主要有两种方法：

1. 把要启动的程序配置成自定义的系统服务。

2. 在 /etc/rc.local 脚本文件中编写启动程序的脚本，本文将详细介绍这种方法。

# 0x01. 简介

```bash
$ ls -hl /etc/rc.local                
lrwxrwxrwx. 1 root root 13 Apr 19  2021 /etc/rc.local -> rc.d/rc.local

# 如果没有执行权限记得加上
$ chmod +x /etc/rc.d/rc.local
```

# 0x02. 

rc.local 文件的原始内容

```
#!/bin/bash
# THIS FILE IS ADDED FOR COMPATIBILITY PURPOSES
#
# It is highly advisable to create own systemd services or udev rules
# to run scripts during boot instead of using this file.
#
# In contrast to previous versions due to parallel execution during boot
# this script will NOT be run after all other services.
#
# Please note that you must run 'chmod +x /etc/rc.d/rc.local' to ensure
# that this script will be executed during boot.
#
touch /var/lock/subsys/local


```

大致意思是：
```
# 添加此文件是为了兼容。
# 强烈建议创建自己的systemd服务或udev规则，以便在引导期间运行脚本，而不是使用此文件。
# 与以前版本不同，由于在引导期间并行执行，此脚本不会在所有其他服务之后运行。
# 请注意，必须运行'chmod+x/etc/rc.d/rc.local'，以确保在引导期间执行此脚本。

```
