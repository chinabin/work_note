# 0x00. 导读

# 0x01. 简介

paste 指令会把每个文件以列对列的方式，一列列地加以合并。

# 0x02. 

```bash
-d<间隔字符>或–delimiters=<间隔字符> 　用指定的间隔字符取代空格字符。
-s或–serial 　多行变一行
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
$ paste name city      
1 xiaoming a:1  1 beijing  beijing cc:1
2 xiaowang a:2  2 hubei   wuhan  cc:2
3 xiaoliu a:3   3 hunan   changsha cc:3
4 tang a:4      4 wangcheng     shit cc:4
5 jiang a:5

$ paste name city -d'+'
1 xiaoming a:1+1 beijing  beijing cc:1
2 xiaowang a:2+2 hubei   wuhan  cc:2
3 xiaoliu a:3+3 hunan   changsha cc:3
4 tang a:4+4 wangcheng     shit cc:4
5 jiang a:5+

$ paste name city -d'+' -s
1 xiaoming a:1+2 xiaowang a:2+3 xiaoliu a:3+4 tang a:4+5 jiang a:5
1 beijing  beijing cc:1+2 hubei   wuhan  cc:2+3 hunan   changsha cc:3+4 wangcheng     shit cc:4
```

```bash
$ paste -s name           
1 xiaoming a:1  2 xiaowang a:2  3 xiaoliu a:3   4 tang a:4      5 jiang a:5
```