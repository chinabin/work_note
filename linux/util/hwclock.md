# 0x00. 导读

# 0x01. 简介

显示与设定硬件时钟。

```bash
$ ls -hl /usr/sbin/clock                   
lrwxrwxrwx. 1 root root 7 Apr 19  2021 /usr/sbin/clock -> hwclock
```

# 0x02. 

```
不加参数，显示硬件时钟

--adjust 　hwclock每次更改硬件时钟时，都会记录在/etc/adjtime文件中。使用--adjust参数，可使hwclock根据先前的记录来估算硬件时钟的偏差，并用来校正目前的硬件时钟。
--debug 　显示hwclock执行时详细的信息。
--directisa 　hwclock预设从/dev/rtc设备来存取硬件时钟。若无法存取时，可用此参数直接以I/O指令来存取硬件时钟。
-s --hctosys 　硬件时钟同步到系统时钟上，把系统时间调成和硬件时钟一样。 hc = hard clock
-w --systohc 　系统时钟同步到硬件时钟上，把系统时间调成和硬件时钟一样。 hc = hard clock
--set --date=<日期与时间> 　设定硬件时钟。
--show 　显示硬件时钟的时间与日期。
--test 　仅测试程序，而不会实际更改硬件时钟。
--utc 　若要使用格林威治时间，请加入此参数，hwclock会执行转换的工作。
```