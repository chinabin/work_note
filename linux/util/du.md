# 0x00. 导读

# 0x01. 简介

du 显示文件或目录所占的磁盘空间。

# 0x02. 命令

```bash
命令格式：
du [option] 文件/目录

-h 以容易理解的格式(给人看的格式)输出文件系统分区使用情况，例如 10kB、10MB、10GB 等。
-s 显示文件或整个目录的大小，默认单位为 kB。
--max-depth=1 显示层级

$ du –hs xxxx          显示目录总大小，不会列出目录中的每一个文件
$ du –hs xxxx/*        列出 xxxx下每个目录和文件所占容量
```

```bash
$ sudo du -sh *              
4.0K    blaklist.py
8.0K    command.md
48K     cpp
24K     CPU02.md
73M     linux
```