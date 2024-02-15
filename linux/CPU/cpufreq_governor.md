# 0x00. 导读

# 0x01. 简介

# 0x02. 

[Documentation\cpu-freq\governors.txt](https://www.kernel.org/doc/Documentation/cpu-freq/governors.txt)

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

## 2.1 

有两种类型的 cpu: 一种只需要给定调频范围，cpu 会在该范围内自行确定运行频率；另一种需要软件指定具体的运行频率。

- 对第一种 cpu, cpufreq policy 中会指定频率范围 policy->{min, max}, 之后通过 setpolicy 接口，使其生效即可。

- 对第二种 cpu, cpufreq policy 在指定频率范围的同时，会指明使用的 governor. governor 在启动后，会动态的（例如启动一个 timer, 监测系统运行情况，并根据负荷调整频率），或者静态的（直接设置为某一个合适的频率值），设定 cpu 运行频率。

# 0x03. 通用扩展调速器( Generic Scaling Governors )

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