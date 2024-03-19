
    
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
    
    
## **route**  
1. 路由器是一个网关，但网关不一定是路由器。路由器是网关的一种。

2. 网关是一个概念，不具体特指一类产品，只要连接两个不同的网络的设备都可以叫网关；
而路由器一般特指能够实现路由寻找和转发的特定类产品，路由器很显然能够实现网关的功能。   
网关是默认的数据出口。如果你的数据不知道往哪里走的话，那么他会去默认网关那里报道。路由器包含许多这样的网关，其中之一是默认网关。也就是说数据到了路由器以后，如果不知道自己下一步该怎么走了，那么他会去默认网关那里去报道。但路由器还包含许多其他的网关，比如数据要去买饮料，那么路由器就送数据去小卖铺。如果数据内急，那么路由器送数据去卫生间。这种起点－终点的对应就叫路由（routing）。这也是路由器（router）名字的由来。

3. 默认网关事实上不是一个产品而是一个网络层的概念，PC本身不具备路由寻址能力，所以PC要把所有的IP包发送到一个默认的中转地址上面进行转发，也就是默认网关。
这个网关可以在路由器上，可以在三层交换机上，可以在防火墙上，可以在服务器上，所以和物理的设备无关。

**直接在命令行下执行route命令来添加路由，不会永久保存，当网卡重启或者机器重启之后，该路由就失效了；可以在 /etc/rc.local 中添加route命令来保证该路由设置永久有效。**

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














uptime 快速查看平均负载
dmesg | tail
vmstat 1    可以动态地了解系统资源的使用情况，以及查看系统中是哪一个环节最占用系统资源。
mpstat -P ALL 1 用来检查 CPU 分工状态
pidstat 1
iostat -xz 1
free -m
sar -n DEV 1    网卡吞吐量
sar -n TCP,ETCP 1
top

dstat -cdngym

strace -p / pstack
perf 看进程/CPU:
    perf stat -p 123456
    perf stat -C 1,2,3-5
    perf top
    perf record
进程优先级

top: 看 us sy wa ，看 load average(可以按数字 1 看 CPU 个数) ，看 %CPU > 100%  
    top -p pid -H 看线程占用
    看 VIRT（虚拟内存kb） RES（物理内存kb）
    按 f 选择 swap ，按 s 可以使用 swap 排序