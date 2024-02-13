# 0x00. 导读

ntpd 的实现同步时间的逻辑：把时间的周期缩短，举个比较极限的例子，假设两台服务器时间相差1小时，它的思想就是将自己现有的时间周期缩短，从而间接追上时间服务器的时间。比如时间服务器跑一分钟需要60秒，而 ntpd 的思想是跑一分钟使用 30 秒甚至 1 秒，这样随着时间的推移一定会追上服务器时间的。这种方式的确是可以追到服务器时间，但是为了追到服务器时间会付出一定时间的代价，这也是 ntpd 之所以被淘汰的根本原因。

默认 centos 7.x 已安装并开启。

chrony 服务本身是兼容 ntpd 服务的:
`123/UDP` 是传统的 NTP 服务所默认监听的端口，而 `323/UDP` 是 chrony 所默认监听的端口。

# 0x01. 简介

chrony 是网络时间协议（NTP）的通用实现。
  
chrony 包含两个程序：  
- chronyd 是一个后台运行的守护进程，用于调整内核中运行的系统时钟和时钟服务器同步。它确定计算机增减时间的比率，并对此进行补偿。  
- chronyc 提供了一个用户界面，用于监控性能并进行多样化的配置。它可以在chronyd实例控制的计算机上工作，也可以在一台不同的远程计算机上工作。可以用 `chronyc help` 查看帮助。

# 0x02. 安装和启动

```bash
# 安装服务，默认 centos7 已安装
$ yum install -y chrony

# 启动服务
$ systemctl start chronyd.service

# 设置开机自启动，默认就是enable的
$ systemctl enable chronyd.service
```

防火墙配置
```bash
# 因NTP使用123/UDP端口协议，所以允许NTP服务即可
$ firewall-cmd --add-service=ntp --permanent
$ firewall-cmd --reload
$ chronyc -a makestep
```

设置时区
```bash
# 查看日期时间、时区及NTP状态
$ timedatectl

# 查看时区列表
$ timedatectl list-timezones

# 修改时
$ timedatectl set-timezone Asia/Shanghai

# 修改日期时间
$ timedatectl set-time "2015-01-21 11:50:00"

# 设置完时区后，强制同步下系统时钟
$ chronyc -a makestep
```

# 0x03. 配置 

当 Chrony 启动时，它会读取 `/etc/chrony.conf` 配置文件中的设置，配置内容格式和 ntpd 服务基本相似。

配置选项与含义解释
```
server: 该参数以 server 开头可以多次用于添加时钟服务器

stratumweight: 设置当 chronyd 从可用源中选择同步源时，每个层应该添加多少距离到同步距离；默认情况下，CentOS 中设置为 0，让 chronyd 在选择源时忽略源的层级

driftfile: chronyd 程序的主要行为之一；根据实际时间计算出服务器增减时间的比率，然后记录到一个文件中，在系统重启后为系统做出最佳时间补偿调整

rtcsync: 将启用一个内核模式，在该模式中，系统时间每 11 分钟会拷贝到实时时钟

makestep: 通常，chronyd 将根据需求通过减慢或加速时钟，使得系统逐步纠正所有时间偏差。在某些特定情况下，系统时钟可能会漂移过快，导致该调整过程消耗很长的时间来纠正系统时钟。该指令强制 chronyd 在调整期大于某个阀值时步进调整系统时钟，但只有在因为 chronyd 启动时间超过指定限制（可使用负值来禁用限制），没有更多时钟更新时才生效

allow/deny: 这里你可以指定一台主机、子网，或者网络以允许或拒绝 NTP 连接到扮演时钟服务器的机器

cmdallow/cmddeny: 跟上面相类似，只是你可以指定哪个 IP 地址或哪台主机可以通过 chronyd 使用控制命令

bindcmdaddress: 该指令允许你限制 chronyd 监听哪个网络接口的命令包（由 chronyc 执行）；该指令通过 cmddeny 机制提供了一个除上述限制以外可用的额外的访问控制等级

logdir: 指定日志文件的目录
```

# 0x04. 查看状态

检查 ntp 源服务器状态
```bash
$ chronyc sourcestats
210 Number of sources = 3
Name/IP Address            NP  NR  Span  Frequency  Freq Skew  Offset  Std Dev
==============================================================================
dns.sjtu.edu.cn             4   3   302      6.440     90.221    +13ms   694us
dns1.synet.edu.cn           0   0     0      0.000   2000.000     +0ns  4000ms
202.118.1.130               7   5   323     -0.174      7.323  -8406ns   303us
```

设置硬件时间
```bash
# 硬件时间默认为UTC
$ timedatectl set-local-rtc 1

# 启用或关闭NTP时间同步
$ timedatectl set-ntp yes|flase

# 校准时间服务器
$ chronyc tracking
```