# 0x00. 导读

参考 1: [Linux cpufreq framework(2)_cpufreq driver](http://www.wowotech.net/pm_subsystem/cpufreq_driver.html)
# 0x01. 简介

# 0x02. 

```c
struct cpufreq_driver {  
	struct module           *owner;  
	char                    name[CPUFREQ_NAME_LEN];  
	u8                      flags;  
       
	bool                    have_governor_per_policy;  
  
	/* needed by all drivers */  
	int     (*init)         (struct cpufreq_policy *policy);  
	int     (*verify)       (struct cpufreq_policy *policy);  
  
	/* define one out of two */  
	int     (*setpolicy)    (struct cpufreq_policy *policy);  
	int		(*target_index)	(struct cpufreq_policy *policy, unsigned int index);
  
	/* should be defined, if possible */  
	unsigned int    (*get)  (unsigned int cpu);  
  
	/* optional */  
	unsigned int (*getavg)  (struct cpufreq_policy *policy, unsigned int cpu);  
	int     (*bios_limit)   (int cpu, unsigned int *limit);  
  
	int     (*exit)         (struct cpufreq_policy *policy);  
	int     (*suspend)      (struct cpufreq_policy *policy);  
	int     (*resume)       (struct cpufreq_policy *policy);  
	struct freq_attr        **attr;  
};  
```

**setpolicy 回调函数**：对于可以自动调频的CPU，driver 需要提供该接口，通过该接口，将调频范围告知 CPU 。

target 回调函数，不建议使用了

**target_index 回调函数**：对于不可以自动调频的 CPU ，该接口用于指定 CPU 的运行频率。 index 表示 frequency table 中的 index 。

验证了 [Documentation/cpu-freq/intel-pstate.txt](https://www.kernel.org/doc/Documentation/cpu-freq/intel-pstate.txt)
> Understanding the cpufreq core governors and policies are important before
> discussing more details about the Intel P-State driver. Based on what callbacks
> a cpufreq driver provides to the cpufreq core, it can support two types of
> drivers:
> 
> - with target_index() callback: In this mode, the drivers using cpufreq core
> simply provide the minimum and maximum frequency limits and an additional
> interface target_index() to set the current frequency. The cpufreq subsystem
> has a number of scaling governors ("performance", "powersave", "ondemand",
> etc.). Depending on which governor is in use, cpufreq core will call for
> transitions to a specific frequency using target_index() callback.
> 
> - setpolicy() callback: In this mode, drivers do not provide target_index()
> callback, so cpufreq core can't request a transition to a specific frequency.
> The driver provides minimum and maximum frequency limits and callbacks to set a
> policy. The policy in cpufreq sysfs is referred to as the "scaling governor".
> The cpufreq core can request the driver to operate in any of the two policies:
> "performance" and "powersave". The driver decides which frequency to use based
> on the above policy selection considering minimum and maximum frequency limits.
> 
> The Intel P-State driver falls under the latter category, which implements the
> setpolicy() callback. This driver decides what P-State to use based on the
> requested policy from the cpufreq core. If the processor is capable of
> selecting its next P-State internally, then the driver will offload this
> responsibility to the processor (aka HWP: Hardware P-States). If not, the
> driver implements algorithms to select the next P-State.

也就是说：
- target_index
	OS 选择工作频率，处理器再根据频率计算电压。
- setpolicy
	OS 只给出负载，由硬件设置电压和频率。 Intel P-State 就是这种。


## 2.1 

cpufreq driver 主要完成平台相关的 CPU 频率/电压的控制，它在 cpufreq framework 中是非常简单的一个模块，编写步骤包括：

1. 平台相关的初始化动作，包括CPU core的clock/regulator获取、初始化等。

2. 生成 frequency table，即 CPU core 所支持的频率/电压列表。并在初始化时将该 table 保存在policy中。

3. 定义一个 struct cpufreq_driver 变量，填充必要的字段，并根据平台的特性，实现其中的回调函数。

4. 调用 cpufreq_register_driver 将 driver 注册到 cpufreq framework 中。

5. cpufreq core 会在 CPU 设备添加时，调用 driver 的 init 接口。driver 需要在该接口中初始化 struct cpufreq_policy 变量。

6. 系统运行过程中，cpufreq core 会根据实际情况，调用 driver 的 setpolicy 或者 target/target_index 等接口，设置 CPU 的调频策略或者频率值。

7. 系统 suspend 的时候，会将 CPU 的频率设置为指定的值，或者调用 driver 的 suspend 回调函数；系统 resume 时，调用 driver 的 resume 回调函数。