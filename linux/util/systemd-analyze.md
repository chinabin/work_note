# 0x00. 导读

# 0x01. 简介

# 0x02. 

启动的几个主要部分耗费的时间
- 防火墙(BIOS)、内核启动、装载运行(initrd)、用户空间
```bash
$ systemd-analyze
Startup finished in 7.038s (firmware) + 32.527s (loader) + 2.323s (kernel) + 22.704s (userspace) = 1min 4.594s
graphical.target reached after 22.695s in userspace
```

哪个 systemd 单元的初始化时间最长
- 结果按照初始化时间长短排序，从多到少
```bash
$ systemd-analyze blame
1min 26.562s fstrim.service
     21.255s plymouth-quit-wait.service
      4.325s NetworkManager-wait-online.service
       838ms snapd.service
       602ms apt-daily-upgrade.service
       ......
```

启动过程中发生的时间关键的事件链
- 前面有 + 的数字显示单元启动所需的时间
- 前面有 @ 的数字表示单元激活开始启动所使用的绝对秒数
```bash
$ systemd-analyze critical-chain
graphical.target @22.695s
└─multi-user.target @22.695s
  └─plymouth-quit-wait.service @1.440s +21.255s
    └─systemd-user-sessions.service @1.435s +2ms
      └─network.target @1.432s
        └─NetworkManager.service @1.309s +122ms
          └─dbus.service @1.308s
            ......
```

生成一个矢量图文件显示在引导和启动过程发生的事件
- 时间轴上零点的左边是引导阶段，零点的右边是启动阶段
- 这一小部分显示了内核、initrd 和 initrd 启动的进程
```bash
# svg
$ systemd-analyze plot > /tmp/bootup.svg

# dot
$ time systemd-analyze dot | dot -Tsvg > /tmp/test.svg
```