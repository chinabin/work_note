# 0x00. 导读

有两种类型的 cpu: 
- 一种只需要给定调频范围，cpu 会在该范围内自行确定运行频率；
- 另一种需要软件指定具体的运行频率。

对第一种 cpu, cpufreq policy 中会指定频率范围 policy->{min, max}, 之后通过 setpolicy 接口，使其生效即可。

对第二种 cpu, cpufreq policy 在指定频率范围的同时，会指明使用的 governor. governor 在启动后，会动态的（例如启动一个 timer ，监测系统运行情况，并根据负荷调整频率），或者静态的（直接设置为某一个合适的频率值），设定 cpu 运行频率。

[Documentation\cpu-freq\governors.txt](https://www.kernel.org/doc/Documentation/cpu-freq/governors.txt)
```

CPU can be set to switch independently	 |	   CPU can only be set
        within specific "limits"		 |       to specific frequencies

                                 "CPUfreq policy"
        		    consists of frequency limits (policy->{min,max})
          		         and CPUfreq governor to be used
        			     /		      \
        			    /		       \
        		       /		       the cpufreq governor decides
        		      /			       (dynamically or statically)
        		     /			       what target_freq to set within
        		    /			       the limits of policy->{min,max}
        		   /			            \
        		  /				             \
        	Using the ->setpolicy call,		 Using the ->target/target_index/fast_switch call,
        	    the limits and the			 the frequency closest
        	     "policy" is set.			 to target_freq is set.
        						             It is assured that it
        						             is within policy->{min,max}
```

# 0x01. 简介

[Docs/The Linux kernel user’s and administrator’s guide/Power Management/Working-State Power Management](https://www.kernel.org/doc/html/v4.19/admin-guide/pm/working-state.html)

- **CPU Performance Scaling**
- intel_pstate CPU Performance Scaling Driver

# 0x02. 什么是 CPU 性能扩展(CPU Performance Scaling)

大多数现代处理器能够在多种不同的时钟频率和电压配置下运行，这些时钟频率和电压通常称为运行性能点( Operating Performance Points )或 P 状态（在 ACPI 术语中）。一般来说，时钟频率越高、电压越高，CPU在单位时间内可以 retired 的指令就越多，但时钟频率越高、电压越高，单位时间内消耗的能量就越多。 CPU 在给定 P 状态下的时间单位（或消耗的电量更多）。因此，**CPU 容量( CPU capacity: 单位时间内可以执行的指令数量 )**和 CPU 消耗的功率之间存在权衡。

硬件接口允许 CPU 在不同的频率/电压配置之间切换以进入不同的 P 状态。硬件接口与算法一起估计所需的 CPU 容量，从而决定将 CPU 置于哪些 P 状态。当然，由于系统的利用率通常会随着时间的推移而变化，因此必须定期重复进行。发生这种情况的活动称为 **CPU 性能缩放**或 CPU 频率缩放（因为它涉及调整 CPU 时钟频率）。

## 2.1 Linux 中的 CPU 性能扩展

Linux 内核通过 CPUFreq 子系统支持 CPU 性能缩放，该子系统由三层代码组成：**核心、缩放调节器( scaling governors )和缩放驱动程序( scaling drivers )**。也叫 **DVFS(Dynamic Voltage Frequency Scaling)**.

cpufreq driver 是机制， cpufreq governor 是策略， core 对机制和策略进行管理。

>
>机制与策略：  
>策略 call 机制。层次上来看，策略在上，机制在下。例如：游戏提供了可玩的机制，你在此基础之上，用自己的策略获得高分。
>
>机制相当于是规则，策略则是在规则下实现目的的方法。


- core

    内核为所有支持 CPU 性能扩展的平台提供通用代码基础架构和用户空间接口。它定义了其他组件运行的基本框架。

- scaling governors

    实现算法来估计所需的 CPU 容量。通常，每个 governors 都会实现一种缩放算法。

- scaling drivers

    与硬件对话。为 scaling governors 提供可用 P 状态的信息，并访问特定于平台的硬件接口以根据 scaling governors 的请求更改 CPU P 状态。

原则上，所有可用的 scaling governors 都可以与每个 scaling drivers 一起使用。然而，不适用于基于硬件本身提供的信息的性能扩展算法，因为该信息通常特定于它所来自的硬件接口，并且不容易在抽象平台中表示。因此， CPUFreq 允许扩展驱动程序绕过 scaling governors 实现自己的性能扩展算法。 **intel_pstate scaling drivers 就是这样的**。

## 2.2 Policy Interface in sysfs



**我们可以大致梳理出 CPUFreq 系统的构成和工作方式:**

1. 首先, CPU 的硬件特性决定了这个CPU的最高和最低工作频率, 所有的频率调整数值都必须在这个范围内, 它们用 `cpuinfo_xxx_freq` 来表示.

2. 然后, 我们可以在这个范围内再次定义出一个软件的调节范围, 它们用 `scaling_xxx_freq` 来表示

3. 同时, 根据具体的硬件平台的不同, 我们还需要提供一个频率表, 这个频率表规定了 cpu 可以工作的频率值, 当然这些频率值必须要在 cpuinfo_xxx_freq 的范围内. 

4. 有了这些频率信息, CPUFreq 系统就可以根据当前 cpu 的负载轻重状况, 合理地从频率表中选择一个合适的频率供 cpu 使用, 已达到节能的目的. 至于如何选择频率表中的频率, 这个要由不同的 governor 来实现, 目前的内核版本提供了多种 governor 供我们选择. 选择好适当的频率以后, 具体的频率调节工作就交由 scaling_driver 来完成.

5. CPUFreq 系统把一些公共的逻辑和接口代码抽象出来, 这些代码与平台无关, 也与具体的调频策略无关, 内核的文档把它称为 CPUFreq Core ([/Documents/cpufreq/core.txt](https://www.kernel.org/doc/Documentation/cpu-freq/core.txt)). 另外一部分, 与实际的调频策略相关的部分被称作 `cpufreq_policy`, cpufreq_policy 又是由频率信息和具体的 governor 组成, governor 才是具体策略的实现者, 当然 governor 需要我们提供必要的频率信息, governor 的实现最好能做到平台无关, 与平台相关的代码用 cpufreq_driver 表述, 它完成实际的频率调节工作.

6. 最后, 如果其他内核模块需要在频率调节的过程中得到通知消息, 则可以通过 cpufreq notifiers 来完成. 


Cpufreq 对如何在底层控制各种不同 CPU 所支持的变频技术以及如何在上层根据系统负载动态选择合适的运行频率进行了封装和抽象, 并在二者之间定义了清晰的接口, 从而在设计上完成了前文所提到的对 mechanism 与 policy 的分离.

在 cpufreq 模块的底层, 各个 CPU 生产厂商只需根据其变频技术的硬件实现和使用方法提供与其 CPU 相关的变频驱动程序(CPU-specific drivers)，例如 Intel 需要提供支持 Enhanced SpeedStep 技术的 CPU 驱动程序, 而 AMD 则需要提供支持 PowerNow 技术的 CPU 驱动程序.

在 cpufreq 模块的上层, governor 作为选择合适的目标运行频率的决策者, 根据一定的标准在适当的时刻选择出 CPU 适合的运行频率, 并通过 cpufreq 模块定义的接口操作底层与 CPU 相关的变频驱动程序, 将 CPU 设置运行在选定的运行频率上. 目前最新的 Linux 内核中提供了 performance, powersave, userspace, conservative 和 ondemand 五种 governors 供用户选择使用, 它们在选择 CPU 合适的运行频率时使用的是各自不同的标准并分别适用于不同的应用场景. 用户在同一时间只能选择其中一个 governor 使用, 但是可以在系统运行过程中根据应用需求的变化而切换使用另一个 governor.

这种设计带来的好处是使得 governor 和 CPU 相关的变频驱动程序的开发可以相互独立进行, 并在最大限度上实现代码重用, 内核开发人员在编写和试验新的 governor 时不会再陷入到某款特定 CPU 的变频技术的硬件实现细节中去, 而 CPU 生产厂商在向 Linux 内核中添加支持其特定的 CPU 变频技术的代码时只需提供一个相对来说简单了很多的驱动程序, 而不必考虑在各种不同的应用场景中如何选择合适的运行频率这些复杂的问题.

## 2.3 通用扩展调速器( Generic Scaling Governors )

CPUFreq 提供可与所有 scaling drivers 一起使用的 generic scaling governors.

- performance  
    性能优先，直接将 cpu 频率设置为 policy->{min,max} 中的最大值。
- powersave  
    功耗优先，直接将 cpu 频率设置为 policy->{min,max} 中的最小值。
- userspace  
    由用户空间程序通过 scaling_setspeed 修改频率。
- schedutil
- ondemand  
    根据 CPU 的当前使用率，动态的调节 CPU 频率。
- conservative  
    类似 Ondemand，不过频率调节的会平滑一下，不会忽然调整为最大值，又忽然调整为最小值。

```c
 /* include/linux/cpufreq.h */
struct cpufreq_governor {
    char    name[CPUFREQ_NAME_LEN];
    int     initialized;
    int (*governor)(struct cpufreq_policy *policy, unsigned int event);
    ssize_t (*show_setspeed)(struct cpufreq_policy *policy, char *buf);
    int (*store_setspeed)(struct cpufreq_policy *policy, unsigned int freq);
    /* 
     * HW must be able to switch to
     * next freq faster than this value in nano secs or we
     * will fallback to performance governor 
    */
    unsigned int max_transition_latency; 
    struct list_head        governor_list;
    struct module           *owner;
};

/*
 * name, 名称
 * initialized, 记录 governor 是否已经初始化
 * max_transition_latency, 容许的最大频率切换延迟，硬件频率的切换必须小于这个值，才能满足需求。
 * governor_list, 用于将该 governor 挂到一个全局的 governor 链 cpufreq_governor_list 上。
 * show_setspeed 和 store_setspeed, 有些 governor 支持从用户空间修改频率值，此时该 governor 必须提供 show_setspeed 和 store_setspeed 两个回调函数，用于响应用户空间的 scaling_setspeed 请求。
 * governor, cpufreq governor 的主要功能都是通过该回调函数实现，该函数借助不同的 event ，以状态机的形式，实现 governor 的启动、停止等操作
*/
```

```c
/* include/linux/cpufreq.h */

/* Governor Events */
// 启动governor
#define CPUFREQ_GOV_START       1
#define CPUFREQ_GOV_STOP        2
// 通常在 governor 启动后发送，要求 governor 检查并修改频率值，使其在 policy 规定的有效范围内。
#define CPUFREQ_GOV_LIMITS      3
/*
policy 启动新的 governor 之前（通常在 cpufreq policy 刚创建或者 governor 改变时）发送。 governor 接收到这个 event 之后，会进行前期的准备工作。
*/
#define CPUFREQ_GOV_POLICY_INIT 4
// 与 CPUFREQ_GOV_POLICY_INIT 相反
#define CPUFREQ_GOV_POLICY_EXIT 5
```

# 0x03. Frequency Boost Support, 频率提升支持

有些处理器支持在特定条件下暂时提高部分内核的运行频率的机制。

不同的供应商使用不同的名称来指代此功能。对于 Intel 处理器，它被称为 Turbo Boost ， AMD 称之为 Turbo-Core 或（在技术文档中） Core Performance Boost 等等。一般来说，不同的供应商也有不同的实现方式。

频率提升机制可以是基于硬件的或基于软件的。如果它是基于硬件的（例如在 x86 上），则触发加速的决定由硬件做出。如果它是基于软件的（例如在 ARM 上），则 scaling driver 决定是否触发提升以及何时触发。

许多系统都可以在平台固件 (BIOS) 设置中禁用频率提升机制。