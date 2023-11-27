# 0x00. 导读

# 0x01. 简介

将一个文件分割成数个。

该指令将大文件分割成较小的文件，在默认情况下将按照每1000行切割成一个小文件。

# 0x02. 

```
-l num : 指定每多少行切成一个小文件
-b byte : 指定每多少字节切成一个小文件
-d：使用数字作为后缀。
-a：指定后缀长度(默认为2)。
[输出文件名] : 设置切割后文件的前置文件名， split会自动在前置文件名后再加上编号
```

```bash
# 切割成多个以"x"开头的小文件


# 将README文件每六行分割成一个文件
$ split -6 README       
$ split -l 6 README       

# 分割成大小为 10KB 的小文件
$ split -b 10k date.file 
$ ls
date.file  xaa  xab  xac  xad  xae  xaf  xag  xah  xai  xaj
$ split -b 10k date.file -d -a 3
$ ls
date.file  x000  x001  x002  x003  x004  x005  x006  x007  x008  x009

# 为分割后的文件指定文件名的前缀, 10m
$ split -b 10k date.file -d -a 3 split_file
$ ls
date.file  split_file000  split_file001  split_file002  split_file003  split_file004  split_file005  split_file006  split_file007  split_file008  split_file009
```




```bash
$ cat DJIs* > DJI_2.MOV
```