# 0x00. 导读

# 0x01. 简介

[Solarflare® Server Adapter User Guide](https://www.xilinx.com/content/dam/xilinx/publications/solarflare/drivers-software/linux/SF-103837-CD-28_Solarflare_Server_Adapter_User_Guide.pdf)

# 0x02. 

The general usage for sfboot is as follows (as root):
```
sfboot [‐‐adapter=eth<N>] [options] [parameters]
```
When the ‐‐adapter option is not specified, the sfboot command applies to all 
adapters present in the target host.

```
‐i, ‐‐adapter =<ethX>
‐c, ‐‐clear Resets all adapter configuration options to their default 
values
‐r, ‐‐repair Restore firmware configuration settings to default values.
```

```bash
# 列出 adapter
$ sudo sfboot -l
Solarflare boot configuration utility [v8.2.4]
Copyright 2002-2020 Xilinx, Inc. 
Adapter list:
  enp23s0f0
  enp23s0f1
  enp101s0f0
  enp101s0f1

# 查看当前模式
$ sfboot --adapter=p1p1 -v
```

```bash
# firmware‐variant=full‐feature|ultra‐low‐latency|capture‐packed‐stream|auto
# Default value = auto ‐ means the driver will select a variant that meets its needs. 
# 默认会是  ultra‐low‐latency
```