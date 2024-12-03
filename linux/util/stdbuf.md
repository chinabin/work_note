# 0x00. 导读

# 0x01. 简介

# 0x02. 

缓冲类型分为三种：
- 无缓冲
- 行缓冲
- 全缓冲

stderr 默认缓冲就是无缓冲。而 stdout 的缓冲类型与输出介质有关：
- 屏幕或者终端：行缓冲
- 重定向文件、管道：全缓

一般情况下程序输出介质都是屏幕或者终端,采用的都是行缓冲，也就是实时输出。但是当程序输出介质为重定向文件或者管道时，内核为了性能优化，可能变成非实时的。究其原因也就是因为 pipe 的缓冲区问题。

## 2.1 

```
stdbuf OPTION... COMMAND
Run COMMAND, with modified buffering operations for its standard streams.

Mandatory arguments to long options are mandatory for short options too.
  -i, --input=MODE   调整标准输入流缓冲区
  -o, --output=MODE  调整标准输出流缓冲区
  -e, --error=MODE   调整标准错误流缓冲区

若模式是 "L"，则相应的流将是行缓冲模式。标准输入流下此选项无效。

若模式是 "0"，则相应的流将不缓冲。

否则模式应当是一个数字，后面可以加上以下单位之一：KB 1000、K 1024、MB 1000*1000、M 1024*1024，G、T、P、E、Z、Y 以此类推。也可以使用二进制前缀：KiB=K、MiB=M，以此类推。在此情况下，相应的流将使用完全缓冲模式，缓冲区大小为指定模式个字节。
```

```bash
$ stdbuf -eL -oL ./SkyRoadTD
```