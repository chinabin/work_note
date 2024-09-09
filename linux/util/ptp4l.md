# 0x00. 导读

# 0x01. 简介

```
/etc/ptp4l.conf
```

man ptp4l

# 0x02.

```bash
$ ptp4l -h
  时间戳选项
  -H  			使用硬件时间戳（默认）
  -S  			使用软件时间戳					

  其他选项
  -f	[file]	从指定文件file中读取配置。 默认情况下不读取任何配置文件。
  -i	[dev]	选择PTP接口设备，例如eth0（可多次指定）必须至少使用此选项或配置文件指定一个端口。
  -s			slaveOnly mode，从时钟模式（覆盖配置文件）。因为不设置的话默认是 master 模式
  -t			透明时钟模式
  -l	[num]	将日志记录级别设置为'num'，默认是6
  -m			将消息打印到stdout
  -q			不打印消息到syslog
  -v			打印软件版本并退出
  -h			help
```

通常，ptp4l 会非常频繁地写入消息。您可以使用 summary_interval 指令降低该频率。其值的表达式为 2 的 N 次幂。例如，要将输出频率降低为每隔 1024（等于 2^10）秒，请将下面一行添加到 /etc/ptp4l.conf 文件。
```
summary_interval 10
```
您还可以使用 -u SUMMARY-UPDATES 选项降低 phc2sys 命令的更新频率。