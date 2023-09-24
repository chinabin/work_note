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