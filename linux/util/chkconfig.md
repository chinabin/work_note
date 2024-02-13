# 0x00. 导读

# 0x01. 简介

`systemctl`: 是一个systemd工具，主要负责控制systemd系统和服务管理器。

`service`: 可以启动、停止、重新启动和关闭系统服务，还可以显示所有系统服务的当前状态。

`chkconfig`: 是管理系统服务(service)的命令行工具。所谓系统服务(service)，就是随系统启动而启动，随系统关闭而关闭的程序。

注意 chkconfig 不是立即自动禁止或激活一个服务，它只是简单的改变了符号连接。

# 0x02. 

systemctl 是系统服务管理器指令，它实际上将 service 和 chkconfig 这两个命令组合到一起。

systemctl 是 RHEL 7 的服务管理工具中主要的工具，它融合之前 service 和 chkconfig 的功能于一体。可以使用它永久性或只在当前会话中启用/禁用服务。

所以 systemctl 是 service 和 chkconfig 的集合和代替。