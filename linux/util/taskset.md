# 0x00. 导读

# 0x01. 简介

绑核，其实就是设定某个进程/线程与某个CPU核的亲和力（affinity）。设定以后，Linux调度器就会让这个进程/线程只在所绑定的核上面去运行。但并不是说该进程/线程就独占这个CPU的核，其他的进程/线程还是可以在这个核上面运行的。如果想要实现某个进程/线程独占某个核，就要使用 cpuset 命令去实现。

# 0x02. 

```bash
-p, --pid               operate on existing given pid
-c, --cpu-list          **display** and specify cpus in list format
```

```bash
# 查看进程 2345 的 CPU 亲和性
$ taskset -cp 2345
# 对运行中的进程，文档上说可以用下面的命令，把 CPU #1 #2 #3 分配给 PID 为 2345 的进程
$ taskset -cp 1,2,3 2345
```