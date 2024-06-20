# 0x00. 导读

# 0x01. 简介

# 0x02. 命令

```
find [OPTION]... [查找路径] [查找条件] [处理动作]
```

- 查找路径：

    - 指定具体目标路径
    - 默认为当前目录
- 查找条件：

    - 指定的查找标准，可以文件名、大小、类型、权限等标准进行
    - 默认为找出指定路径下的所有文件
- 处理动作：

    - 对符合条件的文件做什么操作
    - 默认输出至屏幕

## 2.1 根据文件名查找

- -name <范本样式>  
    支持使用 glob（*/?/[]/[^]）
- -iname <范本样式>  
    效果和指定“-name”参数类似，不区分字母大小写
- -regex "PATTERN"  
    以 PATTERN 匹配整个文件路径字符串，而不仅仅是文件名称
- -iregex "PATTERN"  
    效果和指定“-regexe”参数类似，但忽略字符大小写的差别
- -prune  
    不寻找字符串作为寻找文件或目录的范本样式;

```bash
# 根据名字查找文件
$ find . -name '*.o' -print     # 等同于 find -name '*.o'
$ find . -name '[a-zA-Z]*.o' -print
# 删除当前目录以及子目录下所有.jpg为后缀的文件，不会有删除提示，因此要慎用
$ find . -name "*.jpg" -delete
# 忽略大小写，.TXT 也行
$ find /home -iname "*.txt"
```  

## 2.2 根据文件类型查找

- -type TYPE
    - f: 普通文件
    - d: 目录文件
    - l: 符号链接文件
    - s：套接字文件
    - b: 块设备文件
    - c: 字符设备文件
    - p: 管道文件

```bash
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

## 2.3 根据文件大小来查找

- -size [+|-]#UNIT
- 常用单位：k, M, G
    - #UNIT: (#-1, #]
    - -#UNIT：[0,#-1]
    - +#UNIT：(#,oo)

```bash
# 查找 /home 目录下大于 1MB 的文件：
$ find /home -size +1M
```

## 2.4 根据时间戳

以“天”为单位

- -atime [+|-]#
    - #: [#,#+1)
    - +#: [#+1,oo]
    - -#: [0,#)
- -mtime [+|-]#
    - #: [#,#+1)
    - +#: [#+1,oo]
    - -#: [0,#)
- -ctime [+|-]#
    - #: [#,#+1)
    - +#: [#+1,oo]
    - -#: [0,#)

以“分钟”为单位

- -amin [+|-]#
    - #: [#,#+1)
    - +#: [#+1,oo]
    - -#: [0,#)
- -mmin [+|-]#
    - #: [#,#+1)
    - +#: [#+1,oo]
    - -#: [0,#)
- -cmin [+|-]#
    - #: [#,#+1)
    - +#: [#+1,oo]
    - -#: [0,#)

```bash
# 删除大于 30 天以前的日志
$ find /home/alfred/skyroad/skyroad_td.202* -type f -mtime +30 -delete
```

## 2.5 查找深度
- -path <范本样式>
    - 指定字符串作为寻找目录的范本样式
- -ipath <范本样式>
    - 此参数的效果和指定“-path”参数类似，但忽略字符大小写的差别
- -mindepth <目录层级>
    - 指定开始遍历搜索的最小深度。最小目录层级
- -maxdepth <目录层级>
    - 指定遍历搜索的最大深度。最大目录层级 
- -depth
    - 在查找文件时，首先查找当前目录中的文件，然后再在其子目录中查找

```bash
$ find . -path "*tool/gcc*" -name '*.md'
./tool/gcc/1.md
./tool/gcc/2.md

# depth 举例，注意看结果的目录层数
$ find -maxdepth 2 -name passwd
./etc/passwd
$ find / -maxdepth 3 -name passwd
./usr/bin/passwd
./etc/pam.d/passwd
./etc/passwd
./etc/passwd
# 在第二层子目录和第四层子目录之间查找 passwd 文件。
$ find -mindepth 3 -maxdepth 5 -name passwd
./usr/bin/passwd
./etc/pam.d/passwd
```

## 2.6 处理动作
- -print
    - 默认的处理动作，显示至屏幕
- -print0
    - 将文件或目录名称列出到标准输出，格式为全部的名称皆在同一行
- -printf <输出格式>
    - 将文件或目录名称列出到标准输出，格式可以自行指定
- -ls
    - 类似于对查找到的文件执行“ls -l”命令
- -delete
    - 删除查找到的文件
- -fls /path/to/somefile
    - 查找到的所有文件的长格式信息保存至指定文件中
- -ok COMMAND {} \;
    - 对查找到的每个文件执行由 COMMAND 指定的命令
    - 对于每个文件执行命令之前，都会交互式要求用户确认
- -exec COMMAND {} \;
    - 对查找到的每个文件执行由 COMMAND 指定的命令
{}用于引用查找到的文件名称自身

## 2.7 组合条件
- 与：-a
- 或：-o
- 非：-not/!

```bash
# 找出/home下不是以.txt结尾的文件
$ find /home ! -name "*.txt"
```

# 0x03. 实用

```bash
# 当前目录搜索所有文件，文件内容 包含 “140.206.111.111” 的内容
$ find . -type f -name "*" | xargs grep "140.206.111.111"
```

```bash
# 删除大于 30 天以前的日志
$ find /home/alfred/skyroad/skyroad_td.202* -type f -mtime +30 -delete
```

```bash
# -path 过滤的目录路径 -prune -o 参数来进行过滤
# 其中 -prune 类似于if判断，如果-prune之前的语句为真，就不再执行后面-o跟的语句了，如果没有找到则执行后面的语句。这样就做到了排除效果！
# 其中的"-o" 是 "-or" 的意思！

# 比如查找 /data/* 路径下的的目录，并统计目录大小，以 M 为单位进行排序（默认为降序），并统计前10个大小的目录。命令如下：
$ find /data/* -maxdepth 0 -type d -exec /usr/bin/du -sh {} \;|grep '[0-9]M'|sort -rh|head -10
 
# 查找 /data/* 路径下除 work_note 目录之外的目录，并统计目录大小，以 M 为单位进行排序（默认为降序），并统计前10个大小的目录。命令如下
$ find /data/* -maxdepth 0 -path /data/work_note -prune -o -type d -exec /usr/bin/du -sh {} \;|grep '[0-9]M'|sort -rh|head -10
```