# 0x00. 导读

# 0x01. 简介

我们时常需要查询系统相关的信息，比如页面大小，整数大小之类，如果编写程序去计算会比较繁琐，这里有一个很有用的命令，可以用来获取系统相关信息。它就是getconf。

# 0x02.

```bash
# 查看系统内存分页大小
$ getconf PAGE_SIZE 

# 看linux是32位还是64位最简单的方法
$ getconf LONG_BIT 

# 查看全部系统变量
$ getconf -a 

# 类型大小
$ getconf INT_MAX              
2147483647
$ getconf ULONG_MAX      
18446744073709551615
```