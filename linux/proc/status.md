
/proc/[pid]/status 进程内存相关的信息

```bash
[alfred@bbro-ZJDG1 fpga_radar]$ cat /proc/37730/status 
Name:   adk-ioe-rxactor
Umask:  0022
State:  R (running)
Tgid:   37703
Ngid:   37730
Pid:    37730
PPid:   1
TracerPid:      0
Uid:    0       0       0       0
Gid:    0       0       0       0
FDSize: 256
Groups: 0 
VmPeak: 39574712 kB
VmSize: 39574712 kB
VmLck:         0 kB
VmPin:         0 kB
VmHWM:  38590960 kB
VmRSS:  31393652 kB
RssAnon:         1870308 kB
RssFile:        26377604 kB
RssShmem:        3145740 kB
VmData:  2647752 kB
VmStk:       132 kB
VmExe:       132 kB
VmLib:     49508 kB
VmPTE:     75720 kB
VmSwap:        4 kB
Threads:        16
SigQ:   0/513302
SigPnd: 0000000000000000
ShdPnd: 0000000000000000
SigBlk: 0000000000000000
SigIgn: 0000000000001006
SigCgt: 0000008180000000
CapInh: 0000000000000000
CapPrm: 0000001fffffffff
CapEff: 0000001fffffffff
CapBnd: 0000001fffffffff
CapAmb: 0000000000000000
NoNewPrivs:     0
Seccomp:        0
Speculation_Store_Bypass:       thread vulnerable
Cpus_allowed:   ffffff,ffffffff
Cpus_allowed_list:      0-55
Mems_allowed:   00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000003
Mems_allowed_list:      0-1
voluntary_ctxt_switches:        2506
nonvoluntary_ctxt_switches:     15976
```


- Name：进程的名称
- Pid:PID。
- VmPeak：进程使用的最大虚拟内存，通常情况下它等于进程的内存描述符mm中的total_vm
- VmSize：进程使用的虚拟内存，它等于mm->total_vm
- VmLck：进程锁住的内存，它等于m->locked_vm，这里指使用 mlock()锁住的内存。
- VmPin：进程固定住的内存，它等于mm->pinned_vm，这里指使用get_user_page()固定住的内存。
- VmHWM：进程使用的最大物理内存，它通常等于进程使用的匿名页面、文件映射页面以及共享内存页面的大小总和
- VmRSS：进程使用的最大物理内存，它常常等于VmHWM，计算公式为 VmRSS = RssAnon + RssFile + RssShmem
- RssAnon：进程使用的匿名页面，通过 get_mm_counter（mm， MM_ANONPAGES）获取。
- RssFile：进程使用的文件映射页面，通过 get_mm_counter（mm， MM_FILEPAGES）获取
- RssShmem：进程使用的共享内存页面，通过 get_mm_counter（mm， MM_SHMEMPAGES）获取。
- VmData：进程私有数据段的大小，它等于mm->data_vm
- VmStk：进程用户栈的大小，它等于mm->stack_vm
- VmExe：进程代码段的大小，通过内存描述符mm中的 start_code和 end_code两个成员获取
- VmLib：进程共享库的大小，通过内存描述符mm中的 exec_vm和 VmExe计算。
- VmPTE：进程页表大小，通过内存描述符mm中的 pgtables_byes成员获取。
- VmSwap：进程使用的交换分区的大小，通过 get_mm_counter(mm， MM_SWAPENTS)获取
- HugetlbPages：进程使用巨页的大小，通过内存描述符mm中的 hugetlb_usage成员获取