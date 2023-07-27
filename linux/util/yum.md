# 0x00. 导读

# 0x01. 简介

基于 RPM 包管理，能够从指定的服务器自动下载 RPM 包并且安装，可以自动处理依赖性关系，并且一次安装所有依赖的软件包，无须繁琐地一次次下载、安装。

yum 提供了查找、安装、删除某一个、一组甚至全部软件包的命令，而且命令简洁而又好记。

# 0x02. 命令

```
yum [options] [command] [package ...]
options：可选，选项包括-h（帮助），-y（当安装过程提示选择全部为 "yes"），-q（不显示安装的过程）等等。
command：要进行的操作。
package：安装的包名。
```

- install：安装rpm软件包；
- update：更新rpm软件包；
- check-update：检查是否有可用的更新rpm软件包；
- remove：删除指定的rpm软件包；
- list：查询所有已安装和可安装的软件包；
- search：从 yum 源服务器上查找与关键字相关的所有软件包；
- info：显示指定的rpm软件包的描述信息和概要信息；
- clean：清理yum过期的缓存；
- resolvedep：显示rpm软件包的依赖关系；
- deplist：显示rpm软件包的所有依赖关系；
- provides：查询某个程序所在安装包。

# 0x03. 源
  
```bash
1. 备份系统自带 yum 源配置文件 
mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup

2. 下载阿里云的 yum 源配置文
wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo

或者网易 163 源
http://mirrors.163.com/.help/CentOS7-Base-163.repo

还有中科大的源，复杂点，有需要的时候自己搜索吧

3. 生成缓存
yum makecache
```