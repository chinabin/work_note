# 0x00. 导读

# 0x01. 简介

stress 命令主要用来模拟系统负载较高时的场景。

# 0x02. 命令

```
常用选项：
-c, --cpu N            产生 N 个进程，每个进程都反复不停的计算随机数的平方根
-i, --io N             产生 N 个进程，每个进程反复调用 sync() 将内存上的内容写到硬盘上
-m, --vm N             产生 N 个进程，每个进程不断 malloc/free
    --vm-bytes B       指定分配内存的大小，默认是 256MB
    --vm-stride B      不断访问内存,以一定的步长(stride)进行跳跃访问，默认是 4096
    --vm-hang N        sleep N secs before free (default none, 0 is inf)
    --vm-keep          redirty memory instead of freeing and reallocating
-d, --hadd N           产生 N 个不断执行 write 和 unlink 函数的进程(创建文件，写入内容，删除文件)
    --hadd-bytes B     指定文件大小，默认是 1GB
-t, --timeout N        在 N 秒后结束程序        
    --backoff N        等待 N 微妙后开始运行
-v, --verbose          显示详细的信息
```