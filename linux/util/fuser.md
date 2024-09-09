# 0x00. 导读

yum install -y psmisc

# 0x01. 简介

显示哪些进程使用指定的文件、端口或文件系统。

# 0x02. 

```
-k：杀掉访问文件的进程。如果没有指定-signal就会发送SIGKILL信号。  
-i：杀掉进程之前询问用户，如果没有-k这个选项会被忽略。  
-m：name 指定一个挂载文件系统上的文件或者被挂载的块设备（名称name）。这样所有访问这个文件或者文件系统的进程都会被列出来。如果指定的是一个目录会自动转换成"name/",并使用所有挂载在那个目录下面的文件系统。  
-u: 列出名字
-n：space 指定一个不同的命名空间(space).这里支持不同的空间文件(文件名，此处默认)、tcp(本地tcp端口)、udp(本地udp端口)。对于端口， 可以指定端口号或者名称，如果不会引起歧义那么可以使用简单表示的形式，例如：name/space (即形如:80/tcp之类的表示)。  
```

例子:

```bash
# 列出使用 /etc/passwd 文件的本地进程的进程号
$ fuser /etc/passwd

# 列出使用/etc/filesystems文件的进程的进程号和用户登录名
fuser -u /etc/filesystems
```

```bash
$ fuser -um /dev/shm 
/dev/shm:              930m(centos)   996m(centos)   997m(centos)  1000m(centos) 11617m(centos)


$ fuser -v -n tcp 13599
                     USER        PID ACCESS COMMAND
13599/tcp:           centos     3988 F.... LTTBif
```
