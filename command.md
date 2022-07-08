## **grep**   
`grep [option] pattern file`   
可以使用 --exclude 指定忽略不搜索特定的文件名，或者使用 --exclude 指定只搜索特定的文件名。 --exclude-dir 选项来排除一些目录。   
```bash
grep -no errorID:[^0].* -r . --include=QMT_2022-01-25_13-00.txt --exclude-dir=fut
```
`-A<行数 x>`：除了显示符合范本样式的那一列之外，并显示该行之后的 x 行内容。   
`-B<行数 x>`：除了显示符合样式的那一行之外，并显示该行之前的 x 行内容。   
`-C<行数 x>`：除了显示符合样式的那一行之外，并显示该行之前后的 x 行内容。   
```bash
grep -C3 "aaa" file_* test_file_*
```
`-c`：统计匹配的行数   
```bash
grep -c "order*" demo.txt
```
`-o`：仅显示匹配到的字符串，而不是完整的整行内容。   
`-s`：不显示不存在或者没有匹配文本的错误信息  
```bash
grep lalala file1 file_1
如果 file1 文件不存在就会出现一条 grep: file1: No such file or directory ，加上 -s 就不会有这个信息了。
```
`-w`: 匹配整个单词，元字符不再被解释为特殊含义。   
`-i`: 忽略字符大小写的差别   
`-n --line-number`: 显示行号   
`-r -R --recursive`: 递归搜索   
```bash
grep "__main__" -r ./
```
`-v`: 反转查找   
`-h`: 查询多文件时不显示文件名   
`-l --file-with-matches`: 只列出符合匹配的文件名，不列出具体的匹配行   
`-L --files-without-match`: 列出不符合匹配的文件名   
`-x`: 匹配整行，即只有当文件中有整行内容与模式匹配时，grep命令才输出该行结果   
   
    
```bash
^    # 锚定行的开始 如：'^grep'匹配所有以grep开头的行。    
$    # 锚定行的结束 如：'grep$' 匹配所有以grep结尾的行。
.    # 匹配一个非换行符的字符 如：'gr.p'匹配gr后接一个任意字符，然后是p。    
*    # 匹配零个或多个先前字符 如：'*grep'匹配所有一个或多个空格后紧跟grep的行。    
.*   # 一起用代表任意字符。   
[]   # 匹配一个指定范围内的字符，如'[Gg]rep'匹配Grep和grep。    
[^]  # 匹配一个不在指定范围内的字符，如：'[^A-FH-Z]rep'匹配不包含A-R和T-Z的一个字母开头，紧跟rep的行。    
\(..\)  # 标记匹配字符，如'\(love\)'，love被标记为1。    
\<      # 锚定单词的开始，如:'\<grep'匹配包含以grep开头的单词的行。    
\>      # 锚定单词的结束，如'grep\>'匹配包含以grep结尾的单词的行。    
x\{m\}  # 重复字符x，m次，如：'0\{5\}'匹配包含5个o的行。    
x\{m,\}   # 重复字符x,至少m次，如：'o\{5,\}'匹配至少有5个o的行。    
x\{m,n\}  # 重复字符x，至少m次，不多于n次，如：'o\{5,10\}'匹配5--10个o的行。   
\w    # 匹配文字和数字字符，也就是[A-Za-z0-9]，如：'G\w*p'匹配以G后跟零个或多个文字或数字字符，然后是p。   
\W    # \w的反置形式，匹配一个或多个非单词字符，如点号句号等。   
\b    # 单词锁定符，如: '\bgrep\b'只匹配grep。  

```


--------
## **echo**   
```bash
> echo {0..9}
0 1 2 3 4 5 6 7 8 9
```
-e 解释转义字符
```bash
> echo "a\tb\tc"
a\tb\tc
> echo -e "a\tb\tc"
a       b       c
```

--------   
    
## **xargs**   
Unix 命令都带有参数，有些命令可以接受"标准输入"（stdin）作为参数。例如   
`ps -ef | grep Bif`    
管道命令将左侧命令（ps -ef）的标准输出转换为标准输入，提供给右侧命令（grep Bif）作为参数。   
但是，大多数命令都不接受标准输入作为参数，只能直接在命令行输入参数，这导致无法用管道命令传递参数。例如   
`echo "hello" | echo`   
会失败，因为 echo 不接受管道传参。   
xargs命令的作用，是将标准输入转为命令行参数。   
`echo "hello" | xargs echo`   
xargs 命令格式是 `xargs [-options] [command]` ，真正执行的命令，紧跟在xargs后面，接受xargs传来的参数。   
xargs后面的命令默认是echo。   
```bash
> xargs
# 等同于
>xargs echo
```
输入 xargs 按下回车以后，命令行就会等待用户输入，作为标准输入。你可以输入任意内容，然后按下 Ctrl d ，表示输入结束，这时 echo 命令就会把前面的输入打印出来。
```bash
> xargs
hello (Ctrl + d)
hello
```
默认情况下，xargs 将换行符和空格作为分隔符，把标准输入分解成一个个命令行参数。`-d` 参数可以更改分隔符。
```bash
> echo -e "a\tb\tc" | xargs -d "\t" echo
a b c
```
`-p` 参数打印出要执行的命令，询问用户是否要执行。输入 y 才会真的执行。   
```bash
> echo 'one two three' | xargs -p touch
touch one two three ?...
```
`-t` 参数则是打印出最终要执行的命令，然后直接执行，不需要用户确认。   
如果标准输入包含多行，`-L`参数指定多少行作为一个命令行参数。
```bash
> xargs find -name
"*.txt"   
"*.md"
find: paths must precede expression: `*.md'
> xargs -L 1 find -name
"*.txt"
./foo.txt
./hello.txt
"*.md"
./README.md
```
`-L` 参数虽然解决了多行的问题，但是有时用户会在同一行输入多项。`-n` 参数指定每次将多少项，作为命令行参数。
```bash
> echo {0..9} | xargs -n 2 echo
0 1
2 3
4 5
6 7
8 9
```
如果 xargs 要将命令行参数传给多个命令，可以使用 `-I` 参数。`-I` 指定每一项命令行参数的替代字符串。
```bash
> cat foo.txt
one
two
three
> cat foo.txt | xargs -I file sh -c 'echo file; mkdir file'
one 
two
three
```
使用 `-I file` 表示 `file` 是命令行参数的替代字符串。

--------   
    
## **find**   
`find . -print`    
使用正则表达式的时候注意单引号的添加。     
根据名字查找文件
```bash
find . -name '*.o' -print
find . -name '[a-zA-Z]*.o' -print
```
根据文件类型查找文件
```bash
find . -type f -print | xargs ls -l
f 普通文件
d 目录
l 软链接
```
根据时间查找文件
```
mtime: 上一次修改时间，写入
atime: 上一次访问时间，读取，执行
ctime: 上一次 改变 时间，写入，修改所有者，修改权限，设置软链接
find . -mtime 7 -print
```

--------   
    

## **awk**
- awk 动作 文件名   

-F: 参数指定分隔符为冒号
```bash
awk -F ':' '{ print $1 }' demo.txt
```
变量: `$ + 数字` 表示某个字段，`$NF` 是最后一个字段，`$(NF - 1)` 表示倒数第二个字段，`NR` 表示第几行
```bash
awk -F ':' '{print $1, $(NF-1)}' demo.txt
awk -F ':' '{print NR ") " $1}' demo.txt
```

- awk '条件 动作' 文件名

```bash
# 只输出包含usr的行
awk -F ':' '/usr/ {print $1}' demo.txt
# 输出奇数行
awk -F ':' 'NR % 2 == 1 {print $1}' demo.txt
# 输出第一个字段等于指定值的行
awk -F ':' '$1 == "root" || $1 == "bin" {print $1}' demo.txt
# 使用 if else
awk -F ':' '{if ($1 > "m") print $1; else print "---"}' demo.txt
```

```bash
grep ord_status 1.txt | awk '{print $7}' | sort | uniq
``` 


-------
## **ssh**  
1. SSH（Secure Shell 的缩写）是一种网络协议，用于加密两台计算机之间的通信，并且支持各种身份验证机制。OpenSSH 是 ssh 协议的开源实现。SSH 的软件架构是服务器-客户端模式（Server - Client）。在这个架构中，SSH 软件分成两个部分：向服务器发出请求的部分，称为客户端（client），OpenSSH 的实现为 ssh；接收客户端发出的请求的部分，称为服务器（server），OpenSSH 的实现为 sshd。另外，OpenSSH 还提供一些辅助工具软件（比如 ssh-keygen 、ssh-agent）和专门的客户端工具（比如 scp 和 sftp）。

2. 
    ```bash
    -N 表示只连接远程主机，不打开远程 shell 执行命令，因为默认是连接上去要执行命令的。不加会报错，Cannot fork into background without a command to execute.
    -T 表示不为这个连接分配TTY。
    -f 参数表示 SSH 连接成功后，转后台运行。
    -p 参数指定 SSH 客户端连接的服务器端口。
    ```

3. SSH 客户端的全局配置文件是 `/etc/ssh/ssh_config`，用户个人的配置文件在 `~/.ssh/config`，优先级高于全局配置文件。

4. ssh 服务端，sshd 的配置文件在 `/etc/ssh` 目录，主配置文件是 `sshd_config`。[sshd config 解释](https://wangdoc.com/ssh/server.html)。
    ```bash
    # 启动
    $ sudo systemctl start sshd.service

    # 停止
    $ sudo systemctl stop sshd.service

    # 重启
    $ sudo systemctl restart sshd.service
    ```

5. **动态转发**
    ```bash
    在 A 机器上执行

    ssh -D local-port user@host -N

    SSH会建立一个socket，去监听 local-port 端口。一旦有数据传向那个端口，就自动把它转移到 SSH 连接上面，发往远程主机。可以想象，如果 local-port 原来是一个不加密端口，现在将变成一个加密端口。
    ```
    注意，这种转发采用了 SOCKS5 协议。访问外部网站时，需要把 HTTP 请求转成 SOCKS5 协议，才能把本地端口的请求转发出去。举个例子:
    ```bash
    > ssh -D 2121 tab@123.123.123.123 -N
    > curl -x socks5://localhost:2121 http://www.example.com
    curl 的 -x 参数指定代理服务器，即通过 SOCKS5 协议的本地 2121 端口，访问http://www.example.com。
    ```

6. **本地转发**
    本地转发（local forwarding）指的是，SSH 服务器作为中介的跳板机，建立本地计算机与特定目标之间的加密连接。本地转发指定一个本地端口（local-port），所有发向那个端口的请求，都会转发到 SSH 跳板机（tunnel-host），然后 SSH 跳板机作为中介，将收到的请求发到目标服务器（target-host）的目标端口（target-port）。
    ```bash
    > ssh -L local-port:target-host:target-port tunnel-host
    -L 参数表示本地转发，local-port 是本地端口，target-host 是你想要访问的目标服务器，target-port 是目标服务器的端口，tunnel-host 是 SSH 跳板机。
    ```
    举个例子，你的机器是 A ，你能够访问 B ， B 能够访问 C ，但 A 不能访问 C 。于是在 A 的机器上执行命令: `ssh -L 1234:ip-C:2222 user@ip-B` 就搞定，每次你访问本地的 1234 端口就相当于你访问了 C 的 2222 端口。   
    把我本地的请求，转发出去。白话：访问本机器（执行命令的机器）的 local-port 端口，就等同于，通过 ssh 去到 tunnel-host 机器上之后，访问 target-host 机器的 target-port 端口。

7. **远程转发**
    ```bash
    > ssh -R remote-port:target-host:target-port remotehost
    -R 参数表示远程端口转发，remote-port 是远程计算机的端口，target-host 和 target-port 是目标服务器及其端口，remotehost 是远程计算机。
    ```
    举个例子，你的机器是 A ，还有两台机器 B 和 C ，B 可以访问 A ，B 也可以访问 C 。但是 A 不能访问 B 也不能访问 C ，A 想访问 C 。在机器 **B** 上执行: `ssh -R 1234:ip-C:8000 user@ip-A` 搞定，每次有请求到达 A 的 1234 端口，就相当于请求到达 C 的 8000 端口。   
    把远程的请求，转发进来。白话：访问 A 机器的 1234 端口，就等同于，那个请求通过 ssh 去到 B 机器，然后通过 ssh 去到 C 机器，访问 C 机器的 8000 端口。注意，这个命令是在 B 机器上执行的。

--------   
    
## **free**   
默认等于 `free -k`   
`man free`
```bash
              total        used        free      shared  buff/cache   available
Mem:       16266140     1976840     9208308       32636     5080992    13918424
Swap:       2113532      462304     1651228
```
第一行是内存情况：   
total: 物理内存大小，就是机器实际的内存   
used: 表示已使用，这个值包括了 cached 和 应用程序实际使用的内存   
free: 表示没有使用   
shared: 共享内存大小，是进程间通信的一种方式   
buff/cache: 表示缓存部分，例如高速缓冲区部分   
available: 是从另外的角度来看空闲内存，因为 buff/cache 虽然是缓存了数据的内存，但是当系统缺内存的时候，它们是可以拿来牺牲的，所以理论上来说 available = free + buff/cache      

> cached 列表示当前的页缓存（Page Cache）占用量，buffers 列表示当前的块缓存（buffer cache）占用量。用一句话来解释：Page Cache 用于缓存文件的页数据，buffer cache 用于缓存块设备（如磁盘）的块数据。页是逻辑上的概念，因此 Page Cache 是与文件系统同级的；块是物理上的概念，因此 buffer cache 是与块设备驱动程序同级的。

第二行是交换空间：   
swap space 是磁盘上的一块区域，可以是一个分区，也可以是一个文件。所以具体的实现可以是 swap 分区也可以是 swap 文件。当系统物理内存吃紧时，Linux 会将内存中不常访问的数据保存到 swap 上，这样系统就有更多的物理内存为各个进程服务，而当系统需要访问 swap 上存储的内容时，再将 swap 上的数据加载到内存中，这就是常说的换出和换入。交换空间可以在一定程度上缓解内存不足的情况，但是它需要读写磁盘数据，所以性能不是很高。


--------   
    
## **strace**   
它可以捕获和记录进程的所有系统调用，以及这个进程接收的所有信号。

基本用法:
```bash
$ strace df -h
```

根据进程PID进行追踪:
```bash
$ strace -p 3569
```

得到进程的汇总信息:
```bash
$ strace -c -p 3569
```

显示每一次系统调用的时候的指令指针:
```bash
$ strace -i df -h
```

显示时间戳:
```bash
$ strace -t df -h
```

显示系统调用的耗时:
```bash
$ strace -T df -h
```

只追踪特定的系统调用，signal, abbrev, verbose, raw, read 和 write等几种类型:
```bash
$ strace -e trace=write df -h
$ strace -e trace=open,close df -h
$ strace -e trace=open,close,read,write df -h
$ strace -e trace=all df -h
```

基于特定条件的系统调用(process、file、memory、network、signal):
```bash
$ strace -q -e trace=process df -h
$ strace -q  -e trace=file df -h	
$ strace -q  -e trace=memory df -h	
$ strace -q  -e trace=network df -h	
$ strace -q  -e trace=signal df -h	
```

将追踪结果写入到文件:
```bash
$ strace -o df_debug.txt df -h
```


--------   
    
## **taskset**   


绑核，其实就是设定某个进程/线程与某个CPU核的亲和力（affinity）。设定以后，Linux调度器就会让这个进程/线程只在所绑定的核上面去运行。但并不是说该进程/线程就独占这个CPU的核，其他的进程/线程还是可以在这个核上面运行的。如果想要实现某个进程/线程独占某个核，就要使用 cpuset 命令去实现。

```bash
-c, --cpu-list
-p, --pid
taskset -c 1,2,3 /etc/init.d/mysql start
```

设置绑核有两种方法:
- 掩码形式   
    将掩码转换为二进制形式，从最低位到最高位代表物理CPU的#0、#1、……、#n号核。某位的值为0表示不绑该核，1表示绑。比如：0x00000001的二进制为0000...0001，只有第0号核的位置是1，所以表示只绑0号核；0x00000003的二进制为0000...0011，第0和1号核的位置是1，所以表示绑CPU的0号和1号核；再比如0xFFFFFFFF的二进制为1111...1111，所有32个核的位置都为1，所以表示绑CPU的0~31核。
    ```bash
    taskset -p mask pid
    ```
- 列表形式
    ```bash
    # cpu-list 可以是 0,2,5-11 这样的形式
    taskset -cp cpu-list pid
    ```

taskset 只能设置进程级别的CPU绑定


--------   
    
## **OpenOnload**   
一个对延迟要求很高（比如个位数微秒级延迟）的实时任务是不能触碰内核的。所以需要 kernel bypass ，也就是绕过内核，为什么呢？因为内核处理不仅慢而且延迟不稳定。例如 solarflare 的高性能网卡，DPDK 框架。这里的避免触碰是一个比 bypass 更高的要求：不能以任何方式进入内核，不能直接或间接的执行系统调用(trap)，不能出现page fault(exception)，不能被中断(interrupt)。 trap 和 exception 是主动进入内核的方式，可以在用户程序中避免。关键点在于避免关键线程被中断。Linux 并没有提供一个简单的选项让用户完全关闭中断，也不可能这么做，我们只能想法设法避免让关键任务收到中断。我们知道，中断是 cpu core 收到的，我们可以让关键线程绑定在某个 core 上，然后避免各种中断源(IRQ)向这个 core 发送中断。绑定可以通过 taskset 或 sched_setaffinity 实现。



正常情况下，CPU 按照它内部程序计数器 (Program counter) 所指的指令 (Instruction) 顺序执行，或者如果所指的指令是跳转类指令，比如x86类的CALL或者JMP，跳转到指令所指的地方继续顺序执行。

只有四种情况，CPU 不会顺序执行
- 硬件中断
- Trap 
- Fault
- Abort。

硬件中断时，CPU 会转而执行硬件中断处理函数。Trap 则是被 x86 的INT指令触发，它是内核系统调用 (

System call
) 的实现机制，它会使得 CPU 跳转到操作系统内核的系统函数 (Exception Table)。Fault 则是除 0 报错，缺页中断 (

Page fault
)，甚至内存保护错误实现所依靠的。

不会顺序执行的意思是，CPU 会马上放下手头的指令，转而去处理这四种情况，在把这四种情况处理完之后再依靠 CPU 跳回到刚才的指令继续处理。



避免 IRQ 向某个 core 发中断可以通过改写 `/proc/irq/*/smp_affinity` 来实现。这个操作对硬件中断（比如硬盘和网卡）都是有效的，但对软中断无效（比如 local timer interrupt 和 work queue ），对于 work queue 的屏蔽可以通过改写 `/sys/devices/virtual/workqueue/*/cpumask` 来实现。

OpenOnload有两种模式。

- Spinning模式（busy-wait）
    加速进程的每个线程会占用一整个 CPU core ，始终处于 waiting 状态。通过 htop 可以看到该 CPU 的使用率为 100% 。 spinning 模式速度更快，但是要注意 CPU core 的数量。
- Interrupt模式
    线程不会占用满一个 CPU core ，但可以将中断 Interrupt 放在一个 CPU core 。 interrupt 模式也有加速效果，理论上比 spinning 略差一些。当服务器上总线程数大于 CPU core 的数量时， interrupt 可能优于 spinning ，需要测试来论证。



--------   
    
## **vmstat**  
vmstat 的常规用法是 `vmstat interval times`，即每隔 interval 秒采样一次，共采样 times 次，如果省略times，则一直采集数据到用户手动停止。
```bash
centos@localhost ~/m/w/test (master)> vmstat 3 3
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 5  0 437960 1548296    0 7640940    0    0     1    16    1    1 14  9 77  0  0
 2  0 437960 1548916    0 7640940    0    0     0     0 3098 1756 37 18 46  0  0
 2  0 437960 1548804    0 7640944    0    0     0     1 4340 4742 47 25 28  0  0
```
第一行的值是显示了自系统启动以来的平均值，第二行开始展示现在正在发生的情况。

| 列 | 子列 | 含义 |
| :-----:| :----: | :----: |
| procs |  |  |
|  | r | 显示了多少进程正在等待cpu |
|  | b | 显示多少进程正在不可中断的休眠（通常意味着它们在等待IO ，例如磁盘，网络，用户输入，等等）。 |
| memory |  |  |
|  | swapd | 显示了多少块被换出到了磁盘（页面交换）。 |
|  | free | 显示了多少块是空闲的（未被使用） |
|  | buff | 显示了多少块正在被用作缓冲区 |
|  | cache | 显示了多少正在被用作操作系统的缓存。 |
| swap |  | 显示页面交换活动，大部分时间我们希望看到si 和so 列是0,并且我们很明确不希望看到每秒超过10个块 |
|  | si | 每秒有多少块正在被换入（从磁盘）。 |
|  | so | 每秒有多少块正在被换出（到磁盘）。 |
| io |  | 通常反映了硬盘I/O。 |
|  | bi | 显示了多少块从块设备读取。 |
|  | bo | 显示了多少块从块设备写出。 |
| system |  | 除非上下文切换超过100 000次或更多，一般不用担心上下文切换。 |
|  | in | 显示了每秒中断。 |
|  | cs | 上下文切换的数量。 |
| cpu |  | 显示所有的CPU时间花费在各类操作的百分比。 |
|  | us | 用户代码（非内核）。 |
|  | sy | 执行系统代码（内核）。 |
|  | id | 空闲。 |
|  | wa | 等待IO。 |
|  | st | 如果正在使用虚拟化，显示了从虚拟机中”偷走”的百分比。 |

内存不足的表现：free memory 急剧减少，回收 buffer 和 cache 也无济于事，大量使用交换分区（swpd），页面交换(swap)频繁，读写磁盘数量(io)增多，缺页中断(in)增多，上下文切换(cs)次数增多，等待IO的进程数(b)增多，大量 CPU 时间用于等待IO(wa)。

cpu 密集型服务器的 vmstat 输出通常在 us 列会有一个很高的值，报告了花费在非内核代码上的 cpu 时钟。

在 IO 密集型工作负载下，CPU 花费大量时间在等待 IO 请求完成。这意味着 vmstat 会显示很多处理器在非中断休眠(b列)状态，并且在 wa 这一列的值很高。



--------   
    
## **route**  
1. 路由器是一个网关，但网关不一定是路由器。路由器是网关的一种。

2. 网关是一个概念，不具体特指一类产品，只要连接两个不同的网络的设备都可以叫网关；
而路由器一般特指能够实现路由寻找和转发的特定类产品，路由器很显然能够实现网关的功能。   
网关是默认的数据出口。如果你的数据不知道往哪里走的话，那么他会去默认网关那里报道。路由器包含许多这样的网关，其中之一是默认网关。也就是说数据到了路由器以后，如果不知道自己下一步该怎么走了，那么他会去默认网关那里去报道。但路由器还包含许多其他的网关，比如数据要去买饮料，那么路由器就送数据去小卖铺。如果数据内急，那么路由器送数据去卫生间。这种起点－终点的对应就叫路由（routing）。这也是路由器（router）名字的由来。

3. 默认网关事实上不是一个产品而是一个网络层的概念，PC本身不具备路由寻址能力，所以PC要把所有的IP包发送到一个默认的中转地址上面进行转发，也就是默认网关。
这个网关可以在路由器上，可以在三层交换机上，可以在防火墙上，可以在服务器上，所以和物理的设备无关。

**直接在命令行下执行route命令来添加路由，不会永久保存，当网卡重启或者机器重启之后，该路由就失效了；可以在/etc/rc.local中添加route命令来保证该路由设置永久有效。**

```bash
# 列出当前路由表信息
> route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         192.168.79.2    0.0.0.0         UG    100    0        0 ens33
192.168.79.0    0.0.0.0         255.255.255.0   U     100    0        0 ens33
192.168.122.0   0.0.0.0         255.255.255.0   U     0      0        0 virbr0

Destination: 目标网络(network)或者目标主机(host)
Gateway: 网关地址，*表示并未设置网关地址；
Genmask: 目标网络。其中’255.255.255’用于指示单一目标主机，’0.0.0.0’用于指示默认路由
Metric: 路由距离，到达指定网络所需的中转数。当前Linux内核并未使用，但是routing daemons可能会需要这个值
Ref: 路由项引用次数（linux内核中没有使用）
Use: 此路由项被路由软件查找的次数
Iface: 当前路由会使用哪个接口来发送数据

U (route is up)： 路由是活动的
H (target is a host)： 目标是一个主机而非网络
G (use gateway)： 需要透过外部的主机（gateway)来转递封包
C (cache entry)： 缓存的路由信息
```
三种路由类型：
- 主机路由   
    主机路由是路由选择表中指向单个IP地址或主机名的路由记录。主机路由的Flags字段为 `H`。例如，在下面的示例中，本地主机通过IP地址192.168.1.1的路由器到达IP地址为10.0.0.10的主机：   
    ```
    Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
    -----------     -----------     --------------  ----- ------ ---    --- ------
    10.0.0.10       192.168.1.1     255.255.255.255 UH    100    0        0 eth0
    ```
- 网络路由   
    网络路由是代表主机可以到达的网络。网络路由的Flags字段为 `G`。例如，在下面的示例中，本地主机将发送到网络192.19.12.0的数据包转发到IP地址为192.168.1.1的路由器上：
    ```
    Destination    Gateway       Genmask        Flags    Metric    Ref     Use    Iface
    -----------    -------       -------        -----    -----     ---     ---    -----
    192.19.12.0   192.168.1.1    255.255.255.0   UG      0         0       0      eth0
    ```
- 默认路由
    当主机不能在路由表中查找到目标主机的IP地址或网络路由时，数据包就被发送到默认路由（默认网关）上。默认路由的Flags字段为 `G`。例如，在下面的示例中，默认路由是指IP地址为192.168.1.1的路由器：
    ```
    Destination    Gateway       Genmask    Flags     Metric    Ref    Use    Iface
    -----------    -------       -------    -----    ------     ---    ---    -----
    default       192.168.1.1    0.0.0.0    UG        0         0      0      eth0
    ```














uptime
dmesg | tail
vmstat 1
mpstat -P ALL 1
pidstat 1
iostat -xz 1
free -m
sar -n DEV 1
sar -n TCP,ETCP 1
top