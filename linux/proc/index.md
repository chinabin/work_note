/proc目录下有几个有用的

/proc/cmdline 显示kernel的启动命令行
/proc/partitions 显示所有存储device，和分区。
/proc/modules 显示当前被载入的linux kernel modules
/proc/sys/ 目录，主要用来调优
/proc/sys/dev 设备调优
/proc/sys/fs 虚拟文件系统调优
/proc/sys/kernel 内核接口调优
/proc/sys/net 网络调优
/proc/sys/sunrpc NFS调优
/proc/sys/vm 虚拟内存调优
对/proc/sys下修改的参数都是临时的，不是持久性的

sysctl