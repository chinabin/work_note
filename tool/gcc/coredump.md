# 0x00. 导读

# 0x01. 简介

当程序发生内存越界访问等行为时，会触发 OS 的保护机制，此时 OS 会产生一个信号 (signal) 发送给对应的进程。当进程从内核态到用户态切换时，该进程会处理这个信号。此类信号（比如 SEGV ）的默认处理行为生成一个 coredump 文件。

```
Signal     Value     Action   Comment
──────────────────────────────────────────────────────────
SIGHUP        1       Term    Hangup detected on controlling terminal
                              or death of controlling process
SIGINT        2       Term    Interrupt from keyboard
SIGQUIT       3       Core    Quit from keyboard
SIGILL        4       Core    Illegal Instruction
SIGABRT       6       Core    Abort signal from abort(3)
SIGFPE        8       Core    Floating point exception
SIGKILL       9       Term    Kill signal
SIGSEGV      11       Core    Invalid memory reference
SIGPIPE      13       Term    Broken pipe: write to pipe with no
                              readers
SIGALRM      14       Term    Timer signal from alarm(2)
SIGTERM      15       Term    Termination signal
SIGUSR1   30,10,16    Term    User-defined signal 1
SIGUSR2   31,12,17    Term    User-defined signal 2
SIGCHLD   20,17,18    Ign     Child stopped or terminated
SIGCONT   19,18,25    Cont    Continue if stopped
SIGSTOP   17,19,23    Stop    Stop process
SIGTSTP   18,20,24    Stop    Stop typed at terminal
SIGTTIN   21,21,26    Stop    Terminal input for background process
SIGTTOU   22,22,27    Stop    Terminal output for background process
```
3、4、6、8、11 都能产生的信号， 默认都会产生 core 行为。

# 0x02. 

查看是否设置了 core 文件大小限制, core file size
```bash
$ ulimit -a  
core file size          (blocks, -c) unlimited
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 380334
max locked memory       (kbytes, -l) unlimited
max memory size         (kbytes, -m) unlimited
open files                      (-n) 102400
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 10240
cpu time               (seconds, -t) unlimited
max user processes              (-u) 4096
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```

如果没有就用命令 `ulimit -c unlimited`

## 2.1 配置

`/proc/sys/kernel/core_uses_pid` 可以控制 core 文件的文件名中是否添加 pid 作为扩展。  
```bash
$ echo "1" > /proc/sys/kernel/core_uses_pid 
```


`/proc/sys/kernel/core_pattern` ，可以控制 core 文件保存位置和文件名格式，缺省值是 core，表示将 core 文件存储到当前目录。

```
echo "/corefile/core-%e-%p-%t" > core_pattern，可以将core文件统一生成到/corefile目录下，产生的文件名为core-命令名-pid-时间戳

以下是参数列表:
%p - insert pid into filename 添加pid
%u - insert current uid into filename 添加当前uid
%g - insert current gid into filename 添加当前gid
%s - insert signal that caused the coredump into the filename 添加导致产生core的信号
%t - insert UNIX time that the coredump occurred into filename 添加core文件生成时的unix时间
%h - insert hostname where the coredump happened into filename 添加主机名
%e - insert coredumping executable name into filename 添加命令名

```


`/proc/$pid/coredump_filter` 设置那些内存会被 dump 出来，需要在程序启动之后进行设置。
```
bit 0  Dump anonymous private mappings.
bit 1  Dump anonymous shared mappings.
bit 2  Dump file-backed private mappings.
bit 3  Dump file-backed shared mappings.
bit 4 (since Linux 2.6.24)
       Dump ELF headers.
bit 5 (since Linux 2.6.28)
       Dump private huge pages.
bit 6 (since Linux 2.6.28)
       Dump shared huge pages.
```
默认设置是 33(0010 0001), 也就是保留了 bit 位对应的 0 和 5 对应的内存。另外， core 文件的 save 遵循如下原则：内存映射的 IO 页不会被 dump；vdso 始终会被dump。

## 2.2 core 产生原因

`kill -l` 可以查看信号，使用 man 7 signal 命令查看系统对每个信号作用的描述
```
1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL
 5) SIGTRAP      6) SIGABRT      7) SIGBUS       8) SIGFPE
 9) SIGKILL     10) SIGUSR1     11) SIGSEGV     12) SIGUSR2
13) SIGPIPE     14) SIGALRM     15) SIGTERM     17) SIGCHLD
18) SIGCONT     19) SIGSTOP     20) SIGTSTP     21) SIGTTIN
22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ
26) SIGVTALRM   27) SIGPROF     28) SIGWINCH    29) SIGIO
30) SIGPWR      31) SIGSYS      34) SIGRTMIN    35) SIGRTMIN+1
36) SIGRTMIN+2  37) SIGRTMIN+3  38) SIGRTMIN+4  39) SIGRTMIN+5
40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8  43) SIGRTMIN+9
44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12 47) SIGRTMIN+13
48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14 51) SIGRTMAX-13
52) SIGRTMAX-12 53) SIGRTMAX-11 54) SIGRTMAX-10 55) SIGRTMAX-9
56) SIGRTMAX-8  57) SIGRTMAX-7  58) SIGRTMAX-6  59) SIGRTMAX-5
60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2  63) SIGRTMAX-1
64) SIGRTMAX
```

当程序接收到以下信号时会产生 core 文件：
```
 SIGABRT ：异常终止(abort)时发出的信号  
       说明：  
              调用abort函数时产生此信号。进程异常终止。  
  
SIGBUS ：硬件发生故障时发出的信号  
       说明：  
              指示一个实现定义的硬件故障。  
  
SIGFPE ：算术异常时发出的信号  
       说明：  
              此信号表示一个算术运算异常，例如除以0，浮点溢出等。  
  
SIGILL ：遇到非法硬件指令时发出的信号  
       说明：  
              此信号指示进程已执行一条非法硬件指令。4.3BSD由abort函数产生此信号。现在 abort() 函数用来生成 SIGABRT 信号。  
  
SIGIOT ：硬件故障时发出的信号  
       说明：  
              IOT这个名字来自于PDP-11对于 输入/输出 TRAP(input/output TRAP)指令的缩写。系统V的早期版本，由abort函数产生此信号。SIGABRT现在被用于此。  
  
SIGQUIT ：终端退出时发出的信号  
       说明：  
              当用户在终端上按退出键（一般采用Ctrl-\）时，产生此信号，并送至前台进程组中的所有进程。此信号不仅终止前台进程组（如SIGINT所做的那样），同时产生一个core文件。  
  
SIGSEGV ：无效存储访问发出的信号  
       说明：  
              进程进行了一次无效的存储访问。字SEGV表示“段违例（segmentation violation）”。  
  
SIGSYS ：无效的系统调用时发出的信号  
       说明：  
              进行了一个无效的系统调用。由于某种未知原因，进程执行了一条系统调用指令，但其指示系统调用类型的参数却是无效的。  
  
SIGTRAP ：硬件故障时发出的信号  
       说明：  
              此信号名来自于 PDP-11 的TRAP指令。  
  
SIGXCPU ：超过CPU限制(setrlimit)时发出的信号  
       说明：  
              SVR4 和 4.3+BSD 支持资源限制的概念。如果进程超过了其软 CPU 时间限制，则产生此信号。      
              XCPU 是 "exceeded CPU time“ 的缩写。  
  
SIGXFSZ ：超过文件长度限制(setrlimit)时发出的信号  
       说明：  
              如果进程超过了其软文件长度限制时发出此信号。  
```

## 2.3. 调试方法

```bash
$ gdb --core=core.9128
(gdb) file ./a.out
```

```bash
$ gdb a.out core.1234
```