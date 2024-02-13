# 0x00. 导读

# 0x01. 简介

用于在 Linux 中设置或查询系统时间、日期和时区等配置。

通常使用此命令来设置或更改当前的日期、时间和时区，或启用自动系统时钟与远程 NTP 服务器同步，以确保 Linux 系统始终保持正确的时间。

# 0x02. 

```
timedatectl [OPTIONS...] COMMAND ...

Options:
  -H --host=[USER@]HOST    在远程主机上操作
  -a --all		   显示所有属性，包括空属性
     --value		   显示属性时，只打印值

Commands:
  status                   显示当前的时间设置。
  set-time TIME            设置系统时间。
  set-timezone ZONE        设置系统时区。
  list-timezones           显示已知时区。
  set-local-rtc BOOL       控制 RTC(硬件时钟) 是否在当地时间。（BOOL 的值可以是 1 / true 或 0 / false）
  set-ntp BOOL             启用或禁用网络时间同步。（BOOL 的值可以是 1 / true 或 0 / false）
  timesync-status          显示 systemd-timesyncd 的状态。
  show-timesync            显示 systemd-timesyncd 的属性。
```

```bash
# 查看日期时间、时区及NTP状态
$ timedatectl
$ timedatectl status

# 查看时区列表
$ timedatectl list-timezones

# 修改时
$ timedatectl set-timezone Asia/Shanghai

# 修改日期时间
$ timedatectl set-time "2015-01-21 11:50:00"

```