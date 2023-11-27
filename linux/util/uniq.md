# 0x00. 导读

# 0x01. 简介
 
当没有选项时，邻近的重复行将合并为一个。

# 0x02. 参数

```
-c, --count                在每列旁边显示该行重复出现的次数。
-d, --repeated             所有邻近的重复行只被打印一次。
-D                         所有邻近的重复行将全部打印。
-f, --skip-fields=N        跳过对前N个列的比较。
-i, --ignore-case          忽略大小写的差异。
-s, --skip-chars=N         跳过对前N个字符的比较。
-u, --unique               默认选项
-w, --check-chars=N        只对每行前N个字符进行比较。
```

```bash
$ cat test
baidu 102 7000
baidu 102 7000
google 110 5000
baidu 102 7000
sohu 100 4500
guge 50 3000
baidu 102 7000
```    

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