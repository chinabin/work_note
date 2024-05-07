# 0x00. 导读

需要额外安装，并不是原生自带. （GFDG0 上有~）

```
yum install sysstat
```

# 0x01. 简介

pidstat 用于监控全部或指定进程的 cpu、内存、线程、设备IO 等系统资源的占用情况。

pidstat 首次运行时显示自系统启动开始的各项统计信息，之后运行 pidstat 将显示自上次运行该命令以后的统计信息。用户可以通过指定统计的次数和时间来获得所需的统计信息。

# 0x02. 命令

```
pidstat --help
Usage: pidstat [ options ] [ <interval> [ <count> ] ]
Options are:
[ -d ] [ -h ] [ -I ] [ -l ] [ -r ] [ -s ] [ -t ] [ -U [ <username> ] ] [ -u ]
[ -V ] [ -w ] [ -C <command> ] [ -p { <pid> [,...] | SELF | ALL } ]
[ -T { TASK | CHILD | ALL } ]
```

常用的参数：
```
-u：默认的参数，显示各个进程的cpu使用统计  
-C comm: 只显示那些包含字符串（可是正则表达式）comm的命令的名字
-p：指定进程号

-r：显示各个进程的内存使用统计  
    minflt/s		#每秒次缺页错误次数(minor page faults)，次缺页错误次数意即虚拟内存地址映射成物理内存地址产生的page fault次数
    majflt/s		#每秒主缺页错误次数(major page faults)，当虚拟内存地址映射成物理内存地址时，相应的page在swap中，这样的page fault为major page fault，一般在内存使用紧张时产生
    VSZ			    #该进程使用的虚拟内存(以kB为单位)
    RSS			    #该进程使用的物理内存(以kB为单位)
    %MEM	        #当前任务使用的有效内存的百分比
-d：显示各个进程的IO使用情况  
    PID			    #进程号
    kB_rd/s			#每秒此进程从磁盘读取的千字节数
    kB_wr/s			#此进程已经或者将要写入磁盘的每秒千字节数
    kB_ccwr/s	    #由任务取消的写入磁盘的千字节数
-t: 显示与所选任务相关的线程的统计数据
-T { TASK | CHILD | ALL }: 指定必须监测的内容：
    TASK是默认的，单个任务的报告；
    CHILD：指定的进程和他们的子进程的全局报告
    ALL：相当于TASK和CHILD
-w：显示每个进程的上下文切换情况 
    cswch/s			#每秒自愿上下文切换（进程无法获取所需资源导致的上下文切换）
    nvcswch/s		#每秒非自愿的上下文切换（时间片轮流等系统强制调度）
    Command			#命令 

-l：显示命令名和所有参数
```

# 0x03. 例子

```bash
# 找到包含 LT 字符串的命令，显示它的线程信息
$ pidstat -C LT -t
```