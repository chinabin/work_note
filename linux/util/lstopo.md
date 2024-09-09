# 0x00. 导读

# 0x01. 简介

# 0x02. 

lstopo命令是用来显示系统的拓扑结构的。它提供了关于NUMA内存节点、共享缓存、CPU包、处理器内核和线程等信息。

```bash
$ yum install hwloc hwloc-gui
```

它渲染由 hwloc 发现的机器拓扑结构，有两种主要模式：文本渲染或图形渲染。
```bash
# 文本渲染，有时候没有找到 hwloc-gui 的话可以用
$ lstopo-no-graphics [OPTIONS]...[FILENAME]

# 图形渲染
$ lstopo [OPTIONS]...[FILENAME]
```

然后运行

```bash
$ lstopo --logical --output-format png > `hostname`.png
```

![Alt text](../../pic/linux/lstopo.png)