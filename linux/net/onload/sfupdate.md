# 0x00. 导读

# 0x01. 简介

不带命令运行，用来查看网卡信息
```bash
$ sudo sfupdate
Solarflare firmware update utility [v8.2.4]
Copyright 2002-2020 Xilinx, Inc. 
Loading firmware images from /usr/share/sfutils/sfupdate_images

ens81f0 - MAC: 00-0F-53-B8-B9-70
    Firmware version:   v8.5.2
    Controller type:    Solarflare SFC9250 family
    Controller version: v8.5.0.1002
    Boot ROM version:   v5.2.2.1006
    MUM type:           Micro-controller
    MUM version:        v2.1.1.1028
    UEFI ROM version:   v2.9.6.3
    Bundle type:        X2522 25G
    Bundle version:     v8.5.2.1000

The Bundle firmware is up to date

ens81f1 - MAC: 00-0F-53-B8-B9-71
    Firmware version:   v8.5.2
    Controller type:    Solarflare SFC9250 family
    Controller version: v8.5.0.1002
    Boot ROM version:   v5.2.2.1006
    MUM type:           Micro-controller
    MUM version:        v2.1.1.1028
    UEFI ROM version:   v2.9.6.3
    Bundle type:        X2522 25G
    Bundle version:     v8.5.2.1000

The Bundle firmware is up to date
```

# 0x02. 

```
sfupdate [‐‐adapter=eth<N>] [options]
    ethN is the interface name (ifname) of the Solarflare adapter to be upgraded
    The format for the options are: <option>=<parameter>
```

```
Running the command sfupdate with no additional parameters will show the 
current firmware(固件) version for all Solarflare adapters and identifies whether the firmware version within sfupdate is more up to date. To update the firmware for all Solarflare adapters run the command sfupdate ‐‐write
Solarflare recommend the following procedure:
1. Run sfupdate to check that the firmware on all adapters is up to date
2. Run sfupdate ‐‐write to update the firmware on all adapters
```