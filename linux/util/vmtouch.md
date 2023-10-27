# 0x00、导读

github: [vmtouch](https://github.com/hoytech/vmtouch)

how many pages of a target file Page Cache contains.

# 0x01、用法

```bash
# -v 输出详细信息
# 看看文件有多少在内存中
$ vmtouch launch.gx_dma.aeg
           Files: 1
     Directories: 0
  Resident Pages: 1/1  4K/4K  100%
         Elapsed: 8e-05 seconds

# 文件的页数 ，文件大小。
```

```bash
# 查看目录有多少内容在内存中
$ vmtouch /data/work_sp/bifrost/bin/                 
           Files: 451
     Directories: 52
  Resident Pages: 263/279896  1M/1G  0.094%
         Elapsed: 0.010254 seconds
```

```bash
# 把文件清除出内存
$ vmtouch -e launch.gx_dma.aeg
```

```bash
# 把文件加载到内存
$ vmtouch -t launch.gx_dma.aeg
```