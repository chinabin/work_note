# 0x00. 导读

[package_manager](../system/package_manager.md)

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

```bash
$ yum provides ifconfig
$ yum search all ifconfig

# 查看 yum 源信息
$ yum repolist

#显示所有yum仓库中的所有的软件，未来用于根据软件包名字查找
$ yum list | grep tree
```

# 0x03. 源

`/etc/yum.conf` ==> 为所有仓库提供公共配置

```bash
$ cat /etc/yum.conf
[main]
cachedir=/var/cache/yum/$basearch/$releasever
# 是否保存安装缓存
keepcache=0
# 调试等级
debuglevel=2
# 安装日志
logfile=/var/log/yum.log
# 是否进行平台匹配
exactarch=1
obsoletes=1
# 是否检测合法性
gpgcheck=1
# 是否支持插件
plugins=1
# 可以同时安装几个程序包
installonly_limit=5
# 追踪bug时候的url
bugtracker_url=http://bugs.centos.org/set_project.php?project_id=23&ref=http://bugs.centos.org/bug_report_page.php?category=yum
# 版本号从哪里获取
distroverpkg=centos-release
```

`/etc/yum.repos.d/*.repo` ==> 为仓库的指向提供配置

```bash
# 随便举个例子
$ cat /etc/yum.repos.d/CentOS-Base.repo
# 仓库ID号，要唯一
[repositoryID]
# 描述信息
name=Some name for this repository
# 仓库地址在哪里，一般为ftp://、http://、nfs://、file:///等
# baseurl还可以指向多个url，以此查找
baseurl=url://path/to/repository/
# 使用镜像站点中使用，不能和baseurl一起使用
mirrorlist=
# 仓库是否启用，默认为启用
enabled={1|0}
# 对程序包是否校验，来源合法性和完整性
gpgcheck={1|0}
# 校验的key在哪里
gpgkey=URL
# 是否可以基于组进行管理，默认启用
enablegroups={1|0}
# 默认为roundrobin，意为随机挑选；priority，自上而下优先级
failovermethod={roundrobin|priority}
# 开销，默认为1000
cost=
```
  
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

## 3.1 扩展 yum 源

EPEL(Extra Packages for Enterprise Linux) 包含一个叫做 'epel-release' 的包，其中包含了用于软件包签名的 gpg 密钥和软件源的信息。安装这个包到你的 Linux 上之后，你将可以通过使用类似于 yum 的工具来安装软件包和它们的依赖。在默认情况下，EPEL 仓库的稳定版本是开启的。

```
# 扩展 yum 源
yum install -y epel-release
# 刷新仓库缓存
yum clean all && yum makecache
# 查看启用的仓库
yum repolist
```