# 0x00. 导读

# 0x01. 简介

awk 其实是一门编程语言，它支持条件判断、数组、循环等功能。所以，我们也可以把 awk 理解成一个脚本语言解释器，就好像 python、php 等一样。

但是大多数时候，awk 只表现为一个命令，你可以用这个命令来对文本进行格式化处理。

# 0x02. 基础概念

命令完整格式是 `awk [options] '[Pattern]{Action}' /path/to/file1 [/path/to/file2]`, 简洁版就是 `awk '{Action}' file`. 

NF 是 Number of Fields 的缩写，意思是当前行中字段的数量 (比如一共有 3 列，则 NF=3，如果你不加`$`，即 print NF，那结果就是 3，如果加了`$`就表示打印第三列)。

## 2.1 分隔符

输入分隔符，输出分割符。

### 2.1.1 输入分隔符

awk 默认会以空格为输入分隔符，可以使用 `-F` 选项修改，例如改为使用 `:` 和 `,` 符号： `-F':,'`。

也可以通过内置变量（`FS`）来指定：
```bash
$ tail -n 5 /etc/passwd | awk -v FS=':' '{print $1}'
```

在命令行里，要用 -v 来说明这是对内置变量赋值 (v 是 variable，变量)，如果是在代码块里 (花括号里)，就不用 -v 来指定了。

### 2.1.2 输出分隔符

两个输出等价，输出列都是以 空格 分开。
```bash
$ tail -n 5 /etc/passwd | awk '{print $1,$NF}'
$ tail -n 5 /etc/passwd | awk '{print $1 $NF}'
```

输出分隔符无法用选项的方式指定，只有内置变量，输入分隔符的内置变量是 FS(Field Separator)，输出分隔符的内置变量是 OFS(Output Field Separator)。

```bash
$ tail -n 5 /etc/passwd | awk -F: -v OFS=" | " '{print $1,$NF}'
# 也可以在代码块里直接用
$ tail -n 5 /etc/passwd | awk '{print $1" | "$NF}'
```

## 2.2 变量

`man awk` 可以查看所有变量。

- FS: Field Separator，输入字段分隔符，默认为一个空格；
- OFS: Output Field Separator，输出字段分隔符，默认为一个空格；
- NF: Number of Fields，当前行的字段的个数 (即当前行被分割成了几列)；
- NR: Number of Records，当前行的行号 (awk 是逐行处理文本的)；

- RS: Record Separator，输入记录 (即行) 分隔符(awk 分割字符串时，是一行一行分割最后组合起来的)，默认为\n；
- ORS: Output Record Separator，输出记录换行符。一般我们肯定会理所当然的认为，换行符肯定就是\n，但事实上，我非要用其它字符串 (比如空格) 当换行符也不是不可以，通过 ORS 指定即可；
- FNR: Number of Record of File，各文件分别计数的行号 (说 “各文件”，是因为 awk 可以指定处理多个文件，空格隔开)，也就是说，FNR 只有在同时处理两个文件的时候，才会体现它与 NR 的区别，在处理一个文件的时候，它与 NR 没区别；
- FILENAME: 当前文件名 (如果同时处理两个文件，可以用文件名来区分数据属于哪个文件)；
- ARGC: Arguments Count，命令行参数的个数；
- ARGV: Arguments Variables，数组，保存的是命令行所给定的各参数 (命令行后面空格隔开的都属于参数)；
- OFMT: Output ForMaT，数字的输出格式，默认是%.6g；

只能说，awk 比较奇怪，第一个元素 (即 ARGV[0])，对于其它语言来说，第一个元素一般都是命令本身后面的那一个，awk 却是自己，只能说 awk 比较特殊吧
```bash
#打印 ARGV 第一个元素(下标为0)
$ awk 'BEGIN{print ARGV[0]}' aa bb
awk
#打印 ARGV 第二个元素(下标为1)
$ awk 'BEGIN{print ARGV[1]}' aa bb
aa
#打印 ARGV 第三个元素(下标为2)
$ awk 'BEGIN{print ARGV[2]}' aa bb
bb
#打印 ARGC
$ awk 'BEGIN{print ARGC}' aa bb
3
```

## 2.3 awk pattern

回忆一下 `awk [options] '[Pattern]{Action}' /path/to/file1 [/path/to/file2]`, 花括号前面的那一串字符串就是模式（\[\] 可以省略），可以理解为 条件 。

模式的两种类型
1. 数学及逻辑运算符，包括：`<`、`<=`、`==`、`>`、`>=`、`!=`、`&&`、`||`、`！`
2. 正则表达式匹配，只有两个：`~`(正则匹配返回 true)、`!~`(正则不匹配返回 true)。  
    正则表达式的 `~` 和 `!~` 这两个符号并不是必须的，当不使用这两个符号时，表示对整行进行匹配，否则可以对指定列进行匹配，另外正则表达式必须写在两个斜杠里 (/这是正则表达式/)

当模式为非正则时 (即没有双斜杠括起来时)，它的值一定是布尔值，也就是只有“真” 和“假”两种情况(比如你用什么大于号小于号，最终它运算出来的也是布尔值)。但是 awk 并没有 true 和 false 这两个布尔值，只有 0 和 1。

```bash
#打印第三列以2开头的行(“~”符号的使用)
$ awk '$3~/^2/{print $NR, $0}' test.txt

#打印第三列不以2开头的行(“!~”符号的使用)
$ awk '$3!~/^2/ {print NR, $0}' test.txt

```

不使用 `~` 或 `!~` 符号，直接对整行进行匹配，如果 2 前面加了 `^` 就什么都不会输出，因为没有哪一行是以 2 开头的。
```bash
# 如果 行 里有 2 ，则输出
$ awk '/2/ {print NR, $0}' test.txt

# 如果 行 是以 2 开头，则输出
$ awk '/^2/ {print NR, $0}' test.txt
```

### 2.3.1 两个特殊模式 (BEGIN 和 END)

前面提到过，其实 awk 花括号里的内容，相当于有一个隐藏的循环，不断的一行一行的处理并输出。

但是，如果花括号用 `BEGIN` 和 `END` 这两种模式定义了，则不会循环，它有特殊含义。`BEGIN` 定义的模式，故名思义，只会在循环开始的时候执行一次，而 `END` 自然就是循环结束后，再执行。

```bash
$ awk -v OFS='|' 'BEGIN{print "姓名","性别","年龄","地址","\n------------"} {print $1,$2,$3,$4} END{print "-----------"}' test.txt

```

相当于
```
#BEGIN
print "姓名","性别","年龄","地址","\n------------"
#Action
while(!END){
    {print $1,$2,$3,$4};
}
#END
print "-----------"

```

### 2.3.2 正则行范围模式

其实 awk 的模式里，是可以同时写两个正则的，以逗号隔开。正则行范围模式，就是从第一个正则匹配的第一行开始 (包含该行) 到第二个正则匹配到的第一行结束(包含该行)，之间的所有行都会输出。

```bash
$ awk '/李四/,/小玉/ {print NR, $0}' test.txt
```

awk 默认使用的正则是扩展正则，如果要使用 POSIX 标准正则，需要使用 `--posix` 来指定，扩展正则用 `--re-interval` 指定 (如果不指定默认就是它)。

## 2.4 格式化

```bash
$ awk '{printf "%s %s %s\n",$1,$2,$3}' test.txt

#加个5表示输出宽度为5，不足空格补，超过了会直接显示所有，不会截取
$ awk '{printf "%5s %s %s\n",$1,$2,$3}' test.txt

#前面再加个-号，表示左对齐(注意-号表示左对齐，但+号并不代表右对齐，因为不用-号默认就是右对齐)
$ awk '{printf "%-5s %s %s\n",$1,$2,$3}' test.txt

```

## 2.5 组合 action

所谓的 “动作（action）”，其实就是编程语言的语句。

```bash
$ awk '{print $1}{print $2}{print $3}' test.txt
```
花括号用于把多个动作括起来，表示一组组合动作。

比如，一个花括号里有两个语句，语句之间用;分隔，这样更能体现花括号是用于括住多个语句的，换句话说，我们写具体语句其实是在花括号里面写的。

注意：语句与语句之间的输出，默认是换行的。

前面我们输出过偶数行，是这么输出的，是在模式里添加条件的
```bash
$ awk 'NR%2==0 {print NR, $0}' test.txt
```
现在我们知道了还有 if 语句，所以还可以这么写
```bash
$ awk '{if(NR%2==0) print NR, $0}' test.txt
$ awk '{if(NR%2==0){print NR, $0}}' test.txt
```
可以看到，效果跟用模式添加条件是一样的。

当 if 里有多条语句时，像大多数编程语言一样，也是可以用花括号括起来的 (其实只有一条语句也是可以括起来的，只不过也可以省略而已)

## 2.6 数组

awk 只有关联数组 (associative array)，关联数组的意思是，数组的键是字符串，即使你写的是数字，内部也把这个数字认为是字符串。

awk 创建一个数组，需要逐个键赋值
```bash
$ awk 'BEGIN{websites["google"]="www.google.com";websites["baidu"]="www.baidu.com";websites["bing"]="www.bing.com"; print websites["google"]}'
```
打印一个不存在的数组元素，不会报错
```bash
$ awk 'BEGIN{print testArr[0]}'
```
其实上它会默认把不存在的这个元素初始化为空字符串

**字符串与数字相加，字符串会被转成数字 0**

`for…in` 的使用 (因为数组的键不是数字，所以无法用 for/while/do…while 等数字增加的方式来遍历)
```bash
$ awk 'BEGIN{websites["google"]="www.google.com";websites["baidu"]="www.baidu.com";websites["bing"]="www.bing.com"; for(key in websites){print websites[key]}}'
```

使用length()函数统计数组元素字数 
```bash
# key 是数字，其实也会是字符串
$ awk 'BEGIN{websites[0]="www.google.com";websites[1]="www.baidu.com";websites[2]="www.bing.com"; total=length(websites);for(i=0;i<total;i++){print websites[i]}}'

# key 是数字字符串，也可以用for循环遍历(虽然此时键本身上已经是字符串，但看上去它是有类型自动转换的，这也是弱类型语言的特点)
$ awk 'BEGIN{websites["0"]="www.google.com";websites["1"]="www.baidu.com";websites["2"]="www.bing.com"; total=length(websites);for(i=0;i<total;i++){print websites[i]}}'

```

应用，`ip.txt` 如下
```
www.google.com 192.168.1.1
www.google.com 192.168.1.2
www.google.com 192.168.1.2
www.google.com 192.168.1.1
www.google.com 192.168.1.3
www.google.com 192.168.1.3
www.google.com 192.168.1.2
www.google.com 192.168.1.4
www.google.com 192.168.1.2
www.google.com 192.168.1.3
```

统计 ip 次数
```bash
$ awk 'BEGIN{print"    ip    ","   count"} {ipCount[$2]++} END{for(key in ipCount){print key, "\t"ipCount[key]}}' ips.txt

# 进阶
$ awk '{ipCount[$2]++} END{for(key in ipCount){print key, "\t"ipCount[key]}}' ips.txt | sort -nr -k2 | head -n 3
```

```bash
#打印奇数行
$ awk 'i=!i {print NR,$0}' test.txt
#打印偶数行
$ awk '!(i=!i) {print NR,$0}' test.txt
```
`i=!i`，未定义的变量默认是空字符串，所以 `!i` 就相当于对空字符串取非，而空字符串会默认转成 0，再加个非，就是 1，也就是说，第一个循环 (前面多数说过隐藏循环)，`i=1`，为真，所以会打印，但是第二个循环的时候，因为 i 之前是 1，再加个非就变成 0，所以第二个循环不会打印，以此类推，i 不断的在真假之间切换，于是出现只打印奇数行的效果。
而能出现奇数行效果，如果要打印偶数行，只需要在外边再套一个非，即可让真假切换倒过来，即第一次为假，后面反复切换。

## 2.7 函数

- rand: 随机获取0~1之间的小数
- srand: 设置随机种子
- int: 截断数字的整数部分
- substr(string, position, length)/substr(string, position): 取子字符串
- index(s, t): 字符t出现的位置，如果没有返回0
- match(s,r): 匹配r字符串出现的位置
- split(source,des,delimiter): 通过分隔符将字符串切割，存放倒数组des
- sub(r, t, s): 按r匹配到的第一个字符串替换为t，s默认为$0
- gsub(r, t, s): 跟sub类似，替换全部匹配到的字符
- sprintf(fmt, expr, …): 格式化数据写入字符串
- system(cmd): 执行命令，并返回状态码。可以执行其他linux命令
- tolower(str): 返回全小写的字符串
- toupper(str): 返回全大写的字符串

# 0x0

备忘录：[linux awk命令详解](https://handerfly.github.io/linux/2019/04/05/%E4%B8%89%E5%89%91%E5%AE%A2%E4%B9%8BAWK/)