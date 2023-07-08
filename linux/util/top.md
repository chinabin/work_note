```bash
$ top
top - 13:43:40 up 3 days, 21:23,  6 users,  load average: 60.34, 60.62, 58.03
Tasks: 522 total,   4 running, 515 sleeping,   0 stopped,   3 zombie
%Cpu(s): 85.0 us,  5.7 sy,  0.0 ni,  0.0 id,  9.3 wa,  0.0 hi,  0.0 si,  0.0 st
%Node0 : 85.5 us,  8.5 sy,  0.0 ni,  0.0 id,  6.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Node1 : 84.5 us,  2.8 sy,  0.0 ni,  0.0 id, 12.7 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem : 13150206+total,   375732 free, 16477400 used, 11464893+buff/cache
KiB Swap: 33554428 total, 32846588 free,   707840 used. 82336672 avail Mem 

  PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND                                                                                               
54442 root      20   0  501.3g 172.1g 171.1g S  3743  137   1604:07 AegisTrade
```

# 0x01. 输出解释

## 1. 13:43:40 up 3 days, 21:23,  6 users

当前系统时间  
系统已经运行了 3 天  
6 个用户当前登录

## 2. **load average: 60.34, 60.62, 58.03**
系统负载，即任务队列的平均长度，三个数值分别为 1分钟、5分钟、15分钟前到现在的平均值。

计算公式：  
![Alt text](../../pic/linux/top1.png)

![Alt text](../../pic/linux/top1.png)

例子：

![Alt text](../../pic/linux/top1.png)

## 3. Tasks: 522 total,   4 running, 515 sleeping,   0 stopped,   3 zombie
总进程数  
正在运行的进程数  
睡眠的进程数  
停止的进程数  
冻结进程数 

## 4. %Cpu(s): 85.0 us,  5.7 sy,  0.0 ni,  0.0 id,  9.3 wa,  0.0 hi,  0.0 si,  0.0 st
us: 用户空间占用CPU百分比  
sy: 内核空间占用CPU百分比  
id: 空闲CPU百分比  
wa: cpu运行时在等待io的时间  
hi：硬件中断占用CPU百分比
si：软中断占用百分比

## 5. KiB Mem : 13150206+total,   375732 free, 16477400 used, 11464893+buff/cache
物理内存总量  
空闲内存总量  
使用的物理内存总量  
用作内核缓存的内存量  

## 6. KiB Swap: 33554428 total, 32846588 free,   707840 used. 82336672 avail Mem
交换区总量  
空闲交换区总量  
使用的交换区总量  
缓冲的交换区总量  

# 列解释

|  列名  | 含义  |
|  ----  | ----  |
|PR|      进程的调度优先级，显示为 rt 的表示这些进程运行在实时态  |
|NI|      nice值，进程的优先级。负值表示高优先级，正值表示低优先级  |
|%CPU|    上次更新到现在的CPU时间占用百分比  |
|TIME|    进程使用的CPU时间总计，单位秒  |
|TIME+|   进程使用的CPU时间总计（动后到现在所使用的全部CPU时间），单位1/100秒  |
|%MEM|    进程使用的物理内存百分比  |
|VIRT|    进程使用的虚拟内存总量，单位kb。VIRT=SWAP+RES  |
|SWAP|    进程使用的虚拟内存中，被换出的大小，单位kb。  |
|RES|     进程使用的、未被换出的物理内存大小，单位kb。RES=CODE+DATA  |
|CODE|    可执行代码占用的物理内存大小，单位kb  |
|DATA|    可执行代码以外的部分(数据段+栈)占用的物理内存大小，单位kb  |
|SHR|     共享内存大小，单位kb  |
|nFLT|    页面错误次数  |
|nDRT|    最后一次写入到现在，被修改过的页面数。  |
|S|       进程状态(D=不可中断的睡眠状态,R=运行,S=睡眠,T=跟踪/停止,Z=僵尸进程)  |
|COMMAND| 命令名/命令行  |
|WCHAN|   若该进程在睡眠，则显示睡眠中的系统函数名  |
|Flags|   任务标志，参考 sched.h  |

# 使用

-d <秒数>：指定 top 命令的刷新时间间隔，单位为秒。  
-n <次数>：指定 top 命令运行的次数后自动退出。  
-p <进程ID>：仅显示指定进程ID的信息。  
-u <用户名>：仅显示指定用户名的进程信息。  
-H：在进程信息中显示线程详细信息。  
-i：不显示闲置（idle）或无用的进程。  
-c：显示完整的命令行而不截断。  

# 交互

M: 以 内存占用率 大小的顺序排序进程列表  
C: 以 CPU 占用率大小的顺序排序进程列表  
N: 以 PID 大小的顺序排序进程列表  
T: 以 使用时间 大小的顺序排序进程列表  
n: 指定显示的行数，默认为满屏
b: 高亮显示，一般为 运行状态 的进程被高亮  

1(数字): 显示 CPU 详细信息，每核显示一行  
l(字母): 显示/隐藏第一行负载信息  
t: 显示/隐藏第2~3行CPU信息  
m: 显示/隐藏第4~5行内存信息  
i: 忽略闲置和僵死进程  
c: 切换显示完整命令行  