# 0x00. 导读

# 0x01. 简介

cut 命令从文件的每一行剪切字节、字符和字段并将这些字节、字符和字段写至标准输出。  
必须指定 -b、-c 或 -f 标志之一。

-b 和 -c ，当遇到中文的时候，用 -c 则会以字符为单位，输出正常；而 -b 只会以字节（8位二进制位）来计算，输出就是乱码。

# 0x02. 命令

-b ：以字节为单位进行分割。  
-c ：以字符为单位进行分割。  
-d ：自定义分隔符，默认为制表符。  
-f, --fields=LIST ：与 -d 一起使用，指定显示哪个区域。  
LIST 由 一个 范围 (range) 或 逗号 隔开的 多个 范围 组成. 范围 是 下列 形式 之一:  

    N: 第 N 个 字节, 字符 或 字段, 从 1 计数 起   
    N-: 从 第 N 个 字节, 字符 或 字段 直至 行尾   
    N-M: 从 第 N 到 第 M (并包括 第M) 个 字节, 字符 或 字段   
    -M: 从 第 1 到 第 M (并包括 第M) 个 字节, 字符 或 字段   
--output-delimiter: 指定输出的分隔符。输入的分隔符由 -d 来指定，而输出分隔符和输入分隔符默认是一样的。

```bash
$ cat /etc/passwd | head -5          
root:x:0:0:root:/root:/bin/bash
bin:x:1:1:bin:/bin:/sbin/nologin
daemon:x:2:2:daemon:/sbin:/sbin/nologin
adm:x:3:4:adm:/var/adm:/sbin/nologin
lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
```
```bash
$ cat /etc/passwd | cut -b 1 |head -5      #输出文件的第一个字节
r
b
d
a
l
```

```bash
$ cat /etc/passwd | cut -c 1-4 |head -5    #输出文件的前四个字符
root
bin:
daem
adm:
lp:x
```

```bash
$ cat /etc/passwd | cut -f1 -d ':' |head -5   #以:分割文件，输出第一个字段
root
bin
daemon
adm
lp

$ cut -d: -f1,7 --output-delimiter '-' /etc/passwd | head -5
root-/bin/bash
bin-/sbin/nologin
daemon-/sbin/nologin
adm-/sbin/nologin
lp-/sbin/nologin
```