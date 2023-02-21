```
1,tan,bin,23,10000000
2,xiao,li,25,2300
3,liu,gang,26,12000
4,li,lei,23,99999
1,tan,bin,23,10000000
```

    sort将文件/文本的每一行作为一个单位相互比较，比较原则是从首字符向后依次按ASCII码值进行比较，最后将他们按升序输出。

```bash
# 去重
sort -u test

# 指定 , 作为分隔符
sort -t, test

# n 是根据数字排序(不然会出现 10 比 2 小)，默认是根据字母。 k 是指定列。
sort -nk 5 -t, test

# 逆序
sort -rnk 5 -t, test
```

```
baidu 100 5000
sohu 100 4500
baidu 101 6000
guge 50 3000
baidu 102 7000
google 110 5000
```
`-k` 选项还有高端玩法:   
`-k FStart.CStart Modifier,FEnd.CEnd Modifier`   
简化一下   
`-k FStart.CStart,FEnd.CEnd`   
表示 按照分隔符划定的列，第 FStart 列的第 CStart 字符 ， 到 第 FEnd 列的第 CEnd 字符 ，作为排序关键字

```bash
# 按照公司名称的第二个字母 开始 排序
$ sort -t ' ' -k 1.2 company.txt

baidu 100 5000
baidu 101 6000
baidu 102 7000
sohu 100 4500
google 110 5000
guge 50 3000
```

```bash
# 只针对公司英文名称的第二个字母进行排序，如果相同的按照员工工资进行降序排序
$ sort -t ' ' -k 1.2,1.2 -k 3,3nr company.txt

baidu 102 7000
baidu 101 6000
baidu 100 5000
google 110 5000
sohu 100 4500
guge 50 3000
```