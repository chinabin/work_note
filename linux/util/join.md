# 0x00. 导读

# 0x01. 简介

默认的内容连接区块是由第一个空白符代表的分界符号。

join 命令用于将两个文件中，指定栏位内容相同的行连接起来。其首先找出两个文件中指定栏位内容相同的行，并加以合并，再输出到标准输出设备。

# 0x02. 选项

```bash
-a 1 or -a 2：数据库中的左连接、右连接、全连接
-v 1 or -v 2：跟-a相同，反着输出
-o<格式>：按照指定的格式来显示结果；
-i ：忽略大小写
-t CHAR ：使用 CHAR 作为列的分隔符，默认是空格
-1 or -2：-1用来设置文件1连接的域，-2用来设置文件2连接的域
-j FIELD : Equivalent to "-1 FIELD -2 FIELD".
```

```bash
$ cat name 
1 xiaoming a:1
2 xiaowang a:2
3 xiaoliu a:3
4 tang a:4
5 jiang a:5

$ cat city 
1 beijing  beijing cc:1
2 hubei   wuhan  cc:2
3 hunan   changsha cc:3
4 wangcheng     shit cc:4
```

```bash
# 忽略不匹配的行
# 等于 join -1 1 -2 1 name city
$ join name city
1 xiaoming a:1 beijing beijing cc:1
2 xiaowang a:2 hubei wuhan cc:2
3 xiaoliu a:3 hunan changsha cc:3
4 tang a:4 wangcheng shit cc:4
```

```bash
# 显示左边所有数据，右边文件没有匹配的久显示空白
# 还可以 join -a1 -a2 name city, 这样会显示所有数据。和数据库的全连接一样
$ join -a1 name city
1 xiaoming a:1 beijing beijing cc:1
2 xiaowang a:2 hubei wuhan cc:2
3 xiaoliu a:3 hunan changsha cc:3
4 tang a:4 wangcheng shit cc:4
5 jiang a:5
```

```bash
# 把 name 文件的第 2 列和 city 文件的第 3 列拼接起来
$ join -o 1.2 2.3 name  city 
xiaoming beijing
xiaowang wuhan
xiaoliu changsha
```

```bash
$ join -t: -j2 name city   
1:1 xiaoming a:1 beijing  beijing cc
2:2 xiaowang a:2 hubei   wuhan  cc
3:3 xiaoliu a:3 hunan   changsha cc
4:4 tang a:4 wangcheng     shit cc
```