# 0x00. 导读

# 0x01. 简介

man x86_energy_perf_policy

# 0x02. 

```bash
# 查看当前策略
# 0 is maximum performance and 15  is  maximum  energy  efficiency.
$ x86_energy_perf_policy -r
cpu0: 0x0000000000000000
cpu1: 0x0000000000000000
cpu2: 0x0000000000000000
cpu3: 0x0000000000000000
cpu4: 0x0000000000000000
cpu5: 0x0000000000000000
cpu6: 0x0000000000000000
cpu7: 0x0000000000000000
cpu8: 0x0000000000000000
cpu9: 0x0000000000000000
cpu10: 0x0000000000000000
cpu11: 0x0000000000000000
cpu12: 0x0000000000000000
cpu13: 0x0000000000000000
cpu14: 0x0000000000000000
cpu15: 0x0000000000000000
cpu16: 0x0000000000000000
cpu17: 0x0000000000000000

# 设置策略:
# performance normal powersave
$ x86_energy_perf_policy profile_name
```