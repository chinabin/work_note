# 0x00. 导读

# 0x01. 简介

主要用于删除文件中控制字符或进行字符转换。

# 0x02. 命令

tr [ -d ] [ -c ] [ -s ] [ 字符串1 ] [ 字符串2 ] 文件名  
其中 字符串1 用于查询， 字符串2 用于处理各种转换。

-c 取代所有不在 字符串1 的字符；  
-d 删除匹配 字符串1 的字符。  
-s 删除所有 **连续重复** 出现字符序列，只保留一个；也可以替换掉最后剩下的那个字符

```bash
$ echo "Hello World" | tr '[a-z]' '[A-Z]'       # or tr a-z A-Z
HELLO WORLD

$ echo "Hello World" | tr def opq
Hpllo Worlo

# 默认情况下，如果 SET1 大于 SET2，超出部分 tr 将重用 SET2  的最后一个字符。
$ echo "Hello World" | tr elmno 123    
H1223 W3r2d
# 使用 -t 选项使得 SET1 的最后四个字符将被删除。 SET1 变为 'elm' ，与 SET2 的长度相同。
$ echo "Hello World" | tr -t elmno 123
H122o Wor2d
```

```bash
# 删除 \n ，只保留一个
$ echo -e "1\n\n2\n\n\n3" | tr -s '\n'
1
2
3

$ echo -e "1\n\n2\n\n\n3" | tr -s '\n' -
1-2-3-

# 删除字符 o 空格 + ，只保留一个
$ echo "Hellooo    C++++++++" | tr -s "[o +]"
Hello C+
```

```bash
# 使用 xy 替换字符串 myfreax 中不是 f 或者 a 的字符。
$ echo 'myfreax' | tr -c 'fa' 'xy'  
yyfyyayy        # 最后多了一个 y ，因为 \n 也被替换了，因为 echo 命令会打印一个不可见的换行符 \n 。

# 删除 0-9 
$ echo "hello 123 world 456" | tr -d '0-9'
hello world
# 除了 0-9 和空格，其余都删除
$ echo "hello 123 world 456" | tr -dc '0-9 '      
 123  456
```