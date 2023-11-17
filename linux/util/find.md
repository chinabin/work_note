# 0x00. 导读

# 0x01. 简介

# 0x02. 命令

find <何处> <何物> <做什么>  

何物一般称为 expression ，是可选参数，用于指定查找的条件，可以是文件名、文件类型、文件大小等等。

expression 中可使用的选项有二三十个之多，以下列出最常用的部份：
```
-name pattern：按文件名查找，支持使用通配符 * 和 ?。
-type type：按文件类型查找，可以是 f（普通文件）、d（目录）、l（符号链接）等。
-size [+-]size[cwbkMG]：按文件大小查找，支持使用 + 或 - 表示大于或小于指定大小，单位可以是 c（字节）、w（字数）、b（块数）、k（KB）、M（MB）或 G（GB）。
-mtime days：按修改时间查找，支持使用 + 或 - 表示在指定天数前或后，days 是一个整数表示天数。
-user username：按文件所有者查找。
-group groupname：按文件所属组查找。
```

```bash
# 根据名字查找文件
$ find . -name '*.o' -print
$ find . -name '[a-zA-Z]*.o' -print
# 删除当前目录以及子目录下所有.jpg为后缀的文件，不会有删除提示，因此要慎用
$ find . -name "*.jpg" -delete
# 忽略大小写，.TXT 也行
$ find /home -iname "*.txt"
# 找出/home下不是以.txt结尾的文件
$ find /home ! -name "*.txt"
```  

```bash
# 根据文件类型查找文件
# f 普通文件
# d 目录
# l 软链接
find . -type f -print | xargs ls -l
```

```bash
# 根据时间查找文件
# mtime: 上一次修改时间，写入
# atime: 上一次访问时间，读取，执行
# ctime: 上一次 改变 时间，写入，修改所有者，修改权限，设置软链接
$ find . -mtime 7 -print
# 搜索在过去100天内未被使用过的执行文件
$ find . -type f -atime +100
# 搜索在10天内被创建或者修改过的文件 
$ find . -type f -mtime -10 
```

```bash
# 查找 /home 目录下大于 1MB 的文件：
$ find /home -size +1M
```

```bash
# 当前目录搜索所有文件，文件内容 包含 “140.206.111.111” 的内容
$ find . -type f -name "*" | xargs grep "140.206.111.111"
```