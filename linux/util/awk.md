- awk 动作 文件名   

-F: 参数指定分隔符为冒号
```bash
awk -F ':' '{ print $1 }' demo.txt
```
变量: `$ + 数字` 表示某个字段，`$NF` 是最后一个字段，`$(NF - 1)` 表示倒数第二个字段，`NR` 表示第几行
```bash
awk -F ':' '{print $1, $(NF-1)}' demo.txt
awk -F ':' '{print NR ") " $1}' demo.txt
```

- awk '条件 动作' 文件名

```bash
# 只输出包含usr的行
awk -F ':' '/usr/ {print $1}' demo.txt
# 输出奇数行
awk -F ':' 'NR % 2 == 1 {print $1}' demo.txt
# 输出第一个字段等于指定值的行
awk -F ':' '$1 == "root" || $1 == "bin" {print $1}' demo.txt
# 使用 if else
awk -F ':' '{if ($1 > "m") print $1; else print "---"}' demo.txt
```

```bash
grep ord_status 1.txt | awk '{print $7}' | sort | uniq
``` 

------

```
awk 'BEGIN{ print "start" } pattern{ commands } END{ print "end" }' file

第一步：执行BEGIN{ commands }语句块中的语句；
第二步：从文件或标准输入(stdin)读取一行，然后执行pattern{ commands }语句块，它逐行扫描文件，从第一行到最后一行重复这个过程，直到文件全部被读取完毕。
第三步：当读至输入流末尾时，执行END{ commands }语句块。
```

pattern:   
pattern 可以是以下任意一个：

- /正则表达式/：使用通配符的扩展集。
- 关系表达式：使用运算符进行操作，可以是字符串或数字的比较测试。
- 模式匹配表达式：用运算符~（匹配）和!~（不匹配）。
- BEGIN语句块、pattern语句块、END语句块：参见awk的工作原理

commands:   
由一个或多个命令、函数、表达式组成，之间由换行符或分号隔开，并位于大括号内，主要部分是：

- 变量或数组赋值
- 输出命令
- 内置函数
- 控制流语句

|  变量   | 含义  |
|  ----  | ----  |
|$0      |   当前记录（这个变量中存放着整个行的内容）|
|$1~$n       |   当前记录的第n个字段，字段间由FS分隔|
|FS      |   输入字段分隔符 默认是空格或Tab|
|NF      |   当前记录中的字段个数，就是有列数|
|NR      |   已经读出的记录数，就是行号，从1开始，如果有多个文件话，这个值也是不断累|加中。
|FNR     |   当前记录数，与NR不同的是，这个值会是各个文件自己的行号|
|RS      |   输入的记录分隔符， 默认为换行符|
|OFS     |   输出字段分隔符， 默认也是空格|
|ORS     |   输出的记录分隔符，默认为换行符|
|FILENAME        |   当前输入文件的名字|

注意:
```
awk '{ print NF, NR, NF - 1}'
输出的是当前行的行数、列数
awk '{ print $NF, $NR, $(NF - 1)}'
输出的是第 a b c 列的内容，其中 a 等于 列数，b 等于行数，c 等于列数 - 1
```