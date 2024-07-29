# 0x00. 导读

# 0x01. 简介

[官网](https://nmon.sourceforge.io/pmwiki.php)

# 0x02.

```bash
$ sudo yum install nmon
# or
$ wget http://sourceforge.net/projects/nmon/files/nmon16p_32_binaries_feb_2024.tar.gz
```

交互模式下:
```bash
# 再按一次命令就会去除显示
$ nmon

```

## 2.1 命令行采集数据

采集数据然后分析。

```bash
# 会自动后台运行，别怕
$ nmon -s 10 -c 60 -f -m /usr/local
# -s 10 每 10 秒采集一次数据。
# -c 60 采集 60 次，即为采集十分钟的数据。
# -f 生成的数据文件名中包含文件创建的时间。
# -m 生成的数据文件的存放目录。
# -t Includes Top Processes stats (-T also collects command arguments)
```