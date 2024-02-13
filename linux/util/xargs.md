# 0x00. 导读

# 0x01. 简介

xargs（英文全拼： eXtended ARGuments）是给命令传递参数的一个过滤器，也是组合多个命令的一个工具。

xargs 可以将管道或标准输入（stdin）数据转换成命令行参数，也能够从文件的输出中读取数据。

xargs 也可以将单行或多行文本输入转换为其他格式，例如多行变单行，单行变多行。

xargs 默认的命令是 echo，这意味着通过管道传递给 xargs 的输入将会包含换行和空白，不过通过 xargs 的处理，换行和空白将被空格取代。

xargs 是一个强有力的命令，它能够捕获一个命令的输出，然后传递给另外一个命令。

之所以能用到这个命令，关键是由于很多命令不支持|管道来传递参数，而日常工作中有有这个必要，所以就有了 xargs 命令，例如：
```bash
find /sbin -perm +700 |ls -l       #这个命令是错误的
find /sbin -perm +700 |xargs ls -l   #这样才是正确的
```
xargs 一般是和管道一起使用。

命令格式：

`somecommand |xargs -item  command`

# 0x02. 

主要参数
```
-d 为输入指定一个定制的分割符
-i 用 {} 代替 传递的数据
-I string 用 string 来代替传递的数据
-n 选项限制单个命令行的参数个数
-t 打印出要执行的命令并不询问用户是否要执行
-p 打印出要执行的命令并询问用户是否要执行
```

```bash
#将当前目录下php文件,改名字
$ ls |grep .php |xargs -i mv {} {}.bak     
```

```bash
$ echo {0..9} | xargs -n 2 echo
0 1
2 3
4 5
6 7
8 9
```

```bash
$ echo "nameXnameXnameXname" | xargs -dX

name name name name

# 指定制表符\t作为分隔符
$ echo -e "a\tb\tc" | xargs -d "\t" echo
a b c
```

```bash
$ ls | xargs -t -I{} echo {}
```