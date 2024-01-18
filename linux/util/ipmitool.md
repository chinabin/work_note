# 0x00. 导读

[ipmitool](https://www.zhaowenyu.com/linux-doc/ipmi/ipmitool.html)

# 0x01. 简介

## 1.1 IPMI

IPMI (Intelligent Platform Management Interface) 即 智能平台管理接口 是使硬件管理具备 智能化 的新一代通用接口标准。

用户可以利用 IPMI 监视服务器的物理特征，如温度、电压、电扇工作状态、电源供应以及机箱入侵等。Ipmi 最大的优势在于它是独立于 CPU BIOS 和 OS 的，所以用户无论在开机还是`关机`的状态下，只要接通电源就可以实现对服务器的监控。

ipmi 是一种规范的标准，其中最重要的物理部件就是 BMC (Baseboard Management Controller )，一种嵌入式管理微控制器，它相当于整个平台管理的 “大脑”，通过它， ipmi 可以监控各个传感器的数据并记录各种事件的日志。

## 1.2 ipmitool

ipmitool 是一种可用在 Linux/Unix 系统下的基于命令行方式的 ipmi 平台管理工具。它支持 ipmi 1.5 和 ipmi 2.0 规范（最新的规范为 ipmi 2.0）。利用它可以实现获取传感器的信息、显示系统日志内容、网络远程开关机等功能。

其主要功能包括读取和显示传感器数据 (SDR) ，显示 System Event Log (SEL) 的内容，显示打印 Field Replaceable Unit（FRU）信息，读取和设置 BMC 模块的 LAN 配置，远程控制服务器主机的电源。

ipmitool 可以本地运行，也可以在远程机器上运行，实现对服务器主机的管理和控制。

# 0x02. 命令

`ipmitool [options...] <command>`

## 2.1 选项

```
-a      提示输入远程服务器的密码
-A      当IPMIv1.5会话激活时，指定使用一个认证类型。
-c      使输出格式为 CSV(逗号分隔的变量)格式。但是不是针对所有命令都有效。
-C      为IPMIv2 lanplus连接使用远程服务器身份验证、完整性和加密算法。
-H      远程服务地址，可以为ip地址或者是主机名。Lan和lanplus接口都需要这个操作。
-I      选择使用的IPMI接口。支持的接口都在使用帮助( -h )输出中可见
-L      力量会话特权级别。可以为CALLBACK,USER, OPERATOR, ADMIN。默认为ADMIN。
-p      设置要连接的远程服务UPD端口，默认为 623。
-P      在命令行中指定远程服务密码。如果支持，他将会进程列表中被掩盖。
-t      桥接IPMI的请求到远程目标地址。
-U      远程服务用户名，默认为NULL。
```

ipmitool -I lan -U ADMIN -P ADMIN -H 10.238.30.89