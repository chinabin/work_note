# 0x00. 导读

# 0x01. 简介

`awk [options] 'program' file`  
options 这个表示一些可选的参数选项。  
program 这个表示 awk 的可执行脚本代码，这个是必须要有的。  
file 这个表示 awk 需要处理的文件。

awk 命令的可执行脚本代码使用单引号括起来，紧接着里面是一对花括号，然后花括号里面就是一些可执行的脚本代码段，例如：   
`awk '{print $0}' /etc/passwd`

# 0x02. 命令

## 2.1 分隔符

```bash
# -F: 参数指定分隔符为冒号
awk -F ':' '{ print $1 }' demo.txt

# 在 -F 参数中使用一对方括号来指定多个分隔符
awk -F '[,|:]' '{ print $1 }' demo.txt      # 使用 ,|: 分隔符
```

## 2.2 变量

- $0 这个表示文本处理时的当前行
- $1 表示文本行被分隔后的第 1 个字段列
- $2 表示文本行被分割后的第 2 个字段列
- $3 表示文本行被分割后的第 3 个字段列
- $n 表示文本行被分割后的第 n 个字段列
- NR 表示文件中的行号，表示当前是第几行
- NF 表示文件中的当前行的列数。
- FS 表示 awk 的输入分隔符，默认分隔符为空格和制表符，你可以对其进行自定义设置
- OFS 表示 awk 的输出分隔符，默认为空格，你也可以对其进行自定义设置
- FILENAME 表示当前文件的文件名称，如果同时处理多个文件，它也表示当前文件名称

```bash
awk -F ':' '{print $1, $(NF-1)}' demo.txt   # 其中加入的逗号表示插入输出分隔符，也就是默认的空格

# 插入转义字符进行格式美化
awk -F ':' '{print $1 "\t" $(NF-1)}' demo.txt       # 一定要记住要使用双引号
```

文件的每一行的每一列的内容除了可以用 print 命令打印输出以外，还可以对其进行赋值
```bash
awk -F':' '{$NF = "lalala"; print $0}' /etc/passwd
```

自定义的变量，awk 声明的变量可以在任何多个花括号脚本中使用
```bash
awk -F':' 'BEGIN {msg="hello, a"; print msg} {print $1} END {print msg}' /etc/passwd
```

## 2.3 多个文件
```bash
awk -F'[:|]' '{print $1}' launch.zj_dma.aeg /etc/passwd
```

## 2.4 BEGIN END 

在脚本代码段前面使用 BEGIN 关键字时，它会在开始读取一个文件之前，运行一次 BEGIN 关键字后面的脚本代码段， BEGIN 后面的脚本代码段只会执行一次。  
**awk 脚本中可以用多个花括号来执行多个脚本代码。**

```bash
awk -F':' 'BEGIN {print "hello, a"} {print $1}' /etc/passwd
```

END 指令和 BEGIN 恰好相反，在 awk 读取并且处理完文件的所有内容行之后，才会执行 END 后面的脚本代码段

```bash
awk -F':' 'BEGIN {print "hello, a"} {print $1} END {print "bye bye"}' /etc/passwd
```

# 2.5 运算和条件

数字，支持以下运算：
- "+" 加法运算符
- "-" 减法运算符
- "*" 乘法运算符
- / 除法运算符
- % 取余运算符
```bash
awk -F'[:]' '{print $3, $4, $3+$4}' /etc/passwd
```

支持以下判断：
- < 小于
- <= 小于或等于
- == 等于
- != 不等于
- > 大于
- >= 大于或等于
- ~ 匹配正则表达式
- !~ 不匹配正则表达式
```bash
awk -F'[:]' '$3 > 1000 {print $3}' /etc/passwd
awk -F'[:]' '$3 > 1000 {print $3}' /etc/passwd
```
注意格式，代表条件满足才会执行 {} 内的代码。和下面的条件跳转语法注意区分。

条件跳转：
```bash
awk -F'[:]' '{if ($3 > 1000) print $3}' /etc/passwd
awk -F'[:]' '{if ($1 == "centos") print $3}' /etc/passwd
```

# 2.6 正则表达式

```bash
# 将包含 centos 的那一行打印
awk '/centos/{print $0}' /etc/passwd
# 以 centos 开头的行
awk '/^centos/{print $0}' /etc/passwd
# 以 zsh 结尾的行
awk '/zsh$/{print $0}' /etc/passwd

# 包含了 alf 中间无数个字母 d 的行打印
awk '/alf.*d/{print $0}' /etc/passwd
# 包含了 alf 中间一个字母 d 的行打印
awk '/alfr.d/{print $0}' /etc/passwd
# * + ?

# 包含了 alfr abcde中的一个 d 的行打印
awk '/alfr[abcde]d/{print $0}' /etc/passwd
# [a-zA-Z], 表示匹配小写的 a 到 z 之间的单个字符，或者大写的 A 到 Z 之间的单个字符
# [^a-z], 符号 ^ 在方括号里面表示取反，也就是非的意思，表示匹配任何非 a 到 z 之间的单个字符
```


# 工作用到

```bash
# 累加
awk '{sum+=$1} END{print sum}'
```

```bash
# 删除以 逗号 分隔的第一列
awk -F',' 'OFS=","{$1="";print $0}' 3.txt | sed 's/^,//g' > 4.txt
```