# 0x00. 导读

# 0x01. 简介

用于查看和修改二进制文件的运行时路径（runtime path，rpath）和运行时搜索路径（run-time search path，runpath）信息

# 0x02. 选项

```bash
# 查看文件的 rpath
$ chrpath -l ffplay
```

```bash
# 修改
$ chrpath -r . ffplay
ffplay: RUNPATH=/home/wuxi/lib-ffmpeg/lib
ffplay: new RUNPATH: .
$ chrpath -l ffplay
ffplay: RUNPATH=.

# Convert rpath to runpath
$ chrpath -c application
application: RUNPATH=/usr/local/lib
```