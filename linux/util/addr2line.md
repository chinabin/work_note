# 0x00. 导读

# 0x01. 简介

# 0x02.

动态库调试方法，用崩的 ip 减去库的 ip：
```bash
[alfred@bbro-XHZCH0 skyroad]$ dmesg -T | tail
...
[Tue Aug  6 21:00:11 2024] SkyRoadTD[3238]: segfault at 7f17b8e8d078 ip 00000000004153ab sp 00007f1baf870be0 error 4 in SRMUEsunnyV10[404000+2c000]
[alfred@bbro-XHZCH0 skyroad]$ addr2line -e /usr/lib64/libonload.so 113AB
/root/onload-8.1.2.26/src/lib/transport/unix/internal.h:820

# 00000000004153ab - 404000 = 113AB
```

程序调试方法，直接用崩的 ip:
```bash
[ 4709.210137] traps: div[2500] trap divide error ip:400524 sp:7ffcd27fd630 error:0 in div[400000+1000]
$ addr2line -e div 400524
```

```
刚刚查了下这个11A3B是如何得到的，给您分享下：
addr2line -e /usr/lib64/libonload.so 113AB 这部分指令告诉 addr2line 工具去查找 /usr/lib64/libonload.so 库中偏移地址为 113AB 的符号信息。
具体来说，113AB 是一个十六进制数，它是在程序计数器值（IP，指令指针） 00000000004153ab 的末尾去掉前导零后的值。在这里，IP 值是 00000000004153ab，去掉前导零后就是 4153ab，而 addr2line 工具需要的是相对于库的偏移地址，也就是用十六进制的4153ab减去404000，得到113AB，因此这里直接使用 113AB。
addr2line 工具的作用是将地址转换为文件名和行号，帮助开发人员找出在程序崩溃时出错的具体位置。通过提供库文件和偏移地址，它可以查找对应的源代码文件和行号，以便更好地理解和修复问题。
总结起来，113AB 是通过将 IP 地址转换为库文件中的偏移地址来得到的，用于在调试过程中定位错误发生的位置。

```