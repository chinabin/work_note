# 0x00. 导读

# 0x01. 简介

Linux rpm 命令用于管理套件。

rpm （英文全拼：redhat package manager） 原本是 Red Hat Linux 发行版专门用来管理 Linux 各项套件的程序，由于它遵循 GPL 规则且功能强大方便，因而广受欢迎。逐渐受到其他发行版的采用。RPM 套件管理方式的出现，让 Linux 易于安装，升级，间接提升了 Linux 的适用度。

# 0x02. 命令

选项：
```bash
-e或--erase	卸载软件包
-h或--hash 　套件安装时列出标记。
-i 　显示套件的相关信息。
-v 　显示指令执行过程。
-vv 　详细显示指令执行过程，便于排错。
-U<套件档>或--upgrade<套件档> 升级指定的套件档。
-q 　使用询问模式，当遇到任何问题时，rpm指令会先询问用户。
```

```bash
# 安装软件
$ rpm -vhi https://nmap.org/dist/nmap-7.94-1.x86_64.rpm

# 查询包安装位置
$ rpm -ql ls

# 卸载
$ rpm -e package.rpm

# 升级
$ rpm -U file.rpm

# 显示系统已安装过的全部RPM软件包：
$ rpm -qa
```

RPM 包默认安装位置
```
/etc/	配置文件安装目录
/usr/bin/	可执行的命令安装目录
/usr/lib/	程序所使用的函数库保存位置
/usr/share/doc/	基本的软件使用手册保存位置
/usr/share/man/	帮助文件保存位置
```

RPM 包还可以手动(--relocate)指定安装路径（**不推荐**）。

