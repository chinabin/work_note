# 简介

TSC是一个64位的寄存器，从Intel Pentium开始，在所有的x86平台上均会提供。它存放的是CPU从启动以来执行的指令周期数。通过rdtsc指令，可以将TSC的数值存放在EDX:EAX中，示例代码如下：
```c
// 推荐这种方法
#include "x86intrin.h"

static uint64_t get_rdtsc() 
{ 
  unsigned int dummy;
  uint64_t t = __rdtscp(&dummy);
  return t;
}

// 这个也推荐
#include <iostream>

uint64_t now() {
   return __builtin_ia32_rdtsc();
}

// release 模式可能被优化出问题
static uint64_t get_tscp()
{
    uint64_t a, d;
    __asm__ volatile("rdtscp" : "=a"(a), "=d"(d));
    return (d << 32) | a;
}

// 假如差值是 10 (这个 10 是指 CPU 10 个嘀嗒)，CPU 2.5GHz ，则时间是 10 / 2.5 = 4ns
```

> 系统启动后， Linux 从 RTC（Real Time Clock）获取当前时间时。RTC 是一个由主板纽扣电池供电的硬件时钟，因此即使机器断电，它也能继续运行。在大多数情况下，RTC 不是特别准确，因为它是由廉价的晶体振荡器驱动的，其频率会根据温度和其他环境因素变化。 Linux 系统从RTC 查询得到的启动时间，并存储在内核内存中，稍后将启动时间用作偏移量，合并TSC 保留的滴答计数推导出（当前的）挂钟时间（Wall-clock time）。

> 系统启动的同时， TSC（Time-Stamp Counter 时间戳计数器）开始运行。TSC 是一个寄存器计数器，它由生成CPU 的时钟脉冲的晶体振荡器驱动，所以它和 CPU 的频率相同，例如 2GHz （CPU的 TSC）时钟每纳秒会滴答两次。

测量时间有三点需要考虑：
- 代价 - 这可能是一个高频操作，获取时间的代价不能太高。
- 精度 - 至少是微秒级别。
- 稳定 - 如果使用的时钟抖动误差很大，那测量结果往往是不可靠的。



# 条件

1. Linux 内核 2.6.18 或更高版本
```bash
cat /proc/version 
```

2. 有 tsc 时钟源（第一步已经能达到了）
```bash
# 查看系统上可用的时钟源
cat /sys/devices/system/clocksource/clocksource0/available_clocksource
# 查看正在使用哪个：
$ cat /sys/devices/system/clocksource/clocksource0/current_clocksource
# 更改时钟源
$ echo tsc > /sys/devices/system/clocksource/clocksource0/current_clocksource
```

3. 有 rdtscp
```bash
cat /proc/cpuinfo | grep rdtscp
```

# 优点

极高精度，极低开销的取时间的方法。

# 缺点

## 1. CPU 频率变化
    Q: 
        有的CPU会根据机器负载情况动态调节工作频率， 那么单位时间CPU的指令周期数就会发生变化，也就很难将其转换成时间。另外，CPU进入休眠再次重启后，TSC 会清零。

    A: 
        1. 恒定的TSC （constant TSC）使系统中所有 CPU 核心上 TSC 保持同步
        2. 不变（或不间断）的TSC （nonstop_tsc）使 TSC 保持在固定速率下运行。
        $ cat /proc/cpuinfo | grep -E "constant_tsc|nonstop_tsc"
        flags：... tsc rdtscp constant_tsc nonstop_tsc ...

## 2. 多核 TSC 不同步
    Q: 
        在同一处理器的多个核心之间，以及不同处理器的不同核心之间，rdtsc 的结果是否是同步的呢？例如核心 A 是 t1 开始计时，B 是 t2 开始计时，两个值就没有比较的意义。

    A: 
        参考上面的回答。
        注意，程序最好是绑核，不是百分百确定多个 CPU 之间能同步。

## 3. 指令乱序
    Q:
        乱序执行，导致程序读取的TSC结果可能不准。例如测试的代码被优化到 tsc 的范围外了。
    A:
        使用 RDTSCP 命令来代替 RDTSC ，开销略大一丁点。

[ClockTimeAnalysis](https://gitlab.com/chriscox/CppPerformanceBenchmarks/-/wikis/ClockTimeAnalysis)