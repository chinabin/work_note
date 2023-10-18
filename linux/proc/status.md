
/proc/[pid]/status 进程内存相关的信息

```bash
[alfred@bbro-ZJDG1 fpga_radar]$ cat /proc/37730/status 
Name: Log2Hostflush
Umask: 0022
State: D (disk sleep)-----------------------表示此时线程处于sleeping，并且是uninterruptible状态的wait。

Tgid: 157-----------------------------------线程组的主pid为157。
Ngid: 0
Pid: 159------------------------------------线程自身的pid为159。
PPid: 1-------------------------------------线程组是由init进程创建的。
TracerPid: 0
Uid: 0 0 0 0
Gid: 0 0 0 0
FDSize: 256---------------------------------表示到目前为止进程使用过的描述符总数。
Groups: 0 10
VmPeak: 1393220 kB--------------------------虚拟内存峰值大小。
VmSize: 1390372 kB--------------------------当前使用中的虚拟内存，小于VmPeak。
VmLck: 0 kB
VmPin: 0 kB
VmHWM: 47940 kB-----------------------------RSS峰值。
VmRSS: 47940 kB-----------------------------RSS实际使用量=RSSAnon+RssFile+RssShmem。
RssAnon: 38700 kB
RssFile: 9240 kB
RssShmem: 0 kB
VmData: 366648 kB--------------------------进程数据段共366648KB。
VmStk: 132 kB------------------------------进程栈一共132KB。
VmExe: 84 kB-------------------------------进程text段大小84KB。
VmLib: 11488 kB----------------------------进程lib占用11488KB内存。
VmPTE: 1220 kB
VmPMD: 0 kB
VmSwap: 0 kB
Threads: 40-------------------------------进程中一个40个线程。
SigQ: 0/3142------------------------------进程信号队列最大3142，当前没有pending状态的信号。
SigPnd: 0000000000000000------------------没有进程pending，所以位图为0。
ShdPnd: 0000000000000000
SigBlk: 0000000000000000
SigIgn: 0000000000000006------------------被忽略的信号，对应信号为SIGINT和SIGQUIT，这两个信号产生也不会进行处理。
SigCgt: 0000000180000800------------------已经产生的信号位图，对应信号为SIGUSR2、以及实时信号32和33。
CapInh: 0000000000000000
CapPrm: 0000003fffffffff
CapEff: 0000003fffffffff
CapBnd: 0000003fffffffff
CapAmb: 0000000000000000
Cpus_allowed: 1---------------------------仅在第1个cpu上执行。
Cpus_allowed_list: 0
voluntary_ctxt_switches: 2377-------------线程主动切换2377次，被动切换5次。
```