# 0x00. 导读

# 0x01. 简介

Linux 内部共有五种对频率的管理策略 `userspace` ， `conservative` ， `ondemand` ， `powersave`  和  `performance` 。

- performance: 省电模式，通常以最低频率运行

- Userspace: 用户自定义模式，系统将变频策略的决策权交给了用户态应用程序，并提供了相应的接口供用户态应用程序调节CPU 运行频率使用

- powersave: 性能模式！只有最高频率，从来不考虑消耗的电量

- ondemand: 系统默认的超频模式，按需调节，内核提供的功能，不是很强大，但有效实现了动态频率调节，平时以低速方式运行，当系统负载提高时候自动提高频率。以这种模式运行不会因为降频造成性能降低，同时也能节约电能和降低温度

- conservative: 保守模式，类似于 ondemand，但调整相对较缓

## 1.1 cpupower

cpupower 是一组为辅助 CPU 调频而设计的用户空间工具。linux 内核支持调节 CPU 主频，降低功耗，已到达节能的效果。

通过选择系统空闲状态不同的电源管理策略，可以实现不同程度降低服务器功耗。但是，更低的功耗策略意味着 CPU 唤醒更慢对性能影响更大。对于对时延和性能要求高的应用。

对于服务器，一般都是接着电源的，而且要求性能比较高，因此，建议关闭 CPU 的动态调节功能，禁止 CPU 休眠，并把 CPU 频率固定到最高。

一般在服务器 BIOS 中修改电源管理为 Performance，如果发现 CPU 模式为 conservative 或者 powersave，可以使用 cpupower 设置 CPU Performance 模式，效果也是相当显著的。

查看当前策略
```bash
$ cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
```

查看可用
```bash
$ cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors
```

查看 CPU 频率
```bash
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
```
CPU 实时频率查看
```bash
$ watch -n 1 cpupower monitor
```

# 1.2 

cpufreq 是一个动态调整 cpu 频率的模块，系统启动时生成一个文件夹 `/sys/devices/system/cpu/cpu0/cpufreq/` ，里面有几个文件，其中 `scaling_min_freq` 代表最低频率， `scaling_max_freq` 代表最高频率， `scaling_governor` 代表 cpu 频率调整模式，用它来控制 CPU 频率。

默认的 CPU governor 配置是 performance ，这个设置可以从 `/etc/sysconfig/cpupower` 环境配置文件得到：
```
# See 'cpupower help' and cpupower(1) for more info
CPUPOWER_START_OPTS="frequency-set -g performance"
CPUPOWER_STOP_OPTS="frequency-set -g ondemand"
```
意思是 cpupower.service 启动将设置 governor 为 performance ，停止时则设置 ondemand 。

# 0x02. 

```
$ cpupower help command
```

```
Usage:  cpupower [-d|--debug] [-c|--cpu cpulist ] <command> [<args>]
Supported commands are:
        frequency-info
        frequency-set
        idle-info
        idle-set
        set
        info
        monitor
```

查看所有 CPU 的信息：  
`[root@lmlphp.com ~]# cpupower frequency-info`  
`[root@lmlphp.com ~]# cpupower -c all frequency-info`

设置所有 CPU 为性能模式：  
`[root@lmlphp.com ~]# cpupower -c all frequency-set -g performance`

设置所有 CPU 为节能模式：  
`[root@lmlphp.com ~]# cpupower -c all frequency-set -g powersave`

cpupower 设置 performance：  
`[root@lmlphp.com ~]# cpupower frequency-set -g performance`

