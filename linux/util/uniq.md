```
baidu 102 7000
baidu 102 7000
google 110 5000
baidu 102 7000
sohu 100 4500
guge 50 3000
baidu 102 7000
```

    将输入文件（或标准输入）中邻近的重复行写入到输出文件（或标准输出）中。
    当没有选项时，邻近的重复行将合并为一个。

```bash
# 打印 相邻的重复行 的次数
$ uniq -c test
```

```bash
# 如果存在 相邻的重复行
# -d 打印 相邻的重复行 的一条
$ uniq -d test
# -D 打印 相邻的重复行 的所有
$ uniq -D test
```

```bash
# -u 是 -D 的相反，只打印不重复的邻近行
```

```bash
# -f n 跳过前 n 个 列
$ uniq -f 1 test
# -s n 跳过前 n 个字符
$ uniq -s 10 test
# -w n 只对前 n 个字符比较
$ uniq -w 10 test
```

```bash
```