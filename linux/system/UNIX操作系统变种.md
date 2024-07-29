# 0x00. 导读

[wiki 出处](https://upload.wikimedia.org/wikipedia/commons/7/77/Unix_history-simple.svg)

简而言之：POSIX 很好用，systemV 基本被淘汰了。

TODO: POSIX

# 0x01. 简介

```
1970 年 Unix 出生。
1973 年，Unix 被丹尼斯·里奇用 C 语言（内核和 I/O 除外）重新编写。
1983 年，Richard Mathew Stallman 发起 GNU 计划。
1985 年，为了避免 GNU 所开发的自由软件被其他人所利用而成为专利软件， 所以他与律师草拟了有名的通用公共许可证 (General Public License, GPL)
1986 年，MINIX 出生。
1991 年，Linus Torvalds 写了第一版 Linux。
1993 年，Slackware 发布，认为是第一个取得广泛成功的Linux发行版。Debian Linux 发布， Debian 是最有影响力的Linux发行版之一。
1994 年 3 月 14 日，Linux 1.0 诞生。
```

![Alt text](../../pic/linux/system/unix_history.gif)

# 0x02.

1970 年 Unix 出生。

## 2.1 Unix 历史

1973 年，Unix 被丹尼斯·里奇用 C 语言（内核和 I/O 除外）重新编写。高级语言编写的操作系统具有更佳的兼容性，也能更容易地移植到不同的计算机平台。

由于 Unix 的高度可移植性、强大的效能、简洁的系统设计、开放的源代码等特点，吸引了许多其他组织和团体对其进行了扩展: 

- 最著名当属加州伯克利分校。他们推出了 `Berkeley Software Distributions(BSD)` 。
    
    BSD 系统的主要特性包括： vi 文本编辑器 (ex 可视模式) 、C shell 和 TCP/IP 的早期实现版等，这些特性至今仍被沿用。
    
- 此外，很多商业公司开始了 Unix 操作系统的衍生开发
    
    例如 AT&T 自家的 `System V`、IBM 的 `AIX` 以及 HP 与 DEC 等公司，都有推出自家的主机搭配自己的类 Unix 操作系统。但是，每家公司推出的硬件架构不同，加上当时没有所谓的协议的概念，导致开发出来的 Unix 操作系统以及内含的相关软件并没有办法在其他的硬件架构下工作！1979 年，AT&T 推出 System V 第七版 Unix 后，这个情况就有点改善了。 这一版最重要的特色是可以支持 X86 架构的个人计算机系统，也就是说 System V 可以在个人计算机上面安装与运作了。

由于商业的考虑，以及在当时现实环境下的思考， AT&T 在 1979 年发行的 System V 第七版 Unix 中，特别提到了 **不可对学生提供源代码** 的严格限制！同时，也造成 Unix 业界之间的紧张气氛，并且也引爆了很多的商业纠纷。

## 2.2 MINIX

由于 1979 年的版权声明的影响，在大学中不能使用Unix源代码用于教学，促使 Andrew Tanenbaum （谭宁邦）教授自己动手开始编写用于教育用途的 Minix 这个类 Unix 的核心程序。在撰写的过程中，为了避免版权纠纷，谭宁邦完全不看 Unix 核心原代码！并且强调他编写的 Minix 系统与 Unix 系统兼容！到 1986 年 Minix 开发工作终于完成。Minix 并不是完全免费的，无法在网络上提供下载，必须要通过磁盘购买才行，所以 Minix 的传递速度并没有很快速！此外，购买时，随磁盘还会附上 Minix 的源代码！

Minix 除启动部分使用汇编语言编写外，其余部分都采用 C 语言编写。共约 12000 行代码，并作为示例放置在他的著作《操作系统：设计与实现》一书的附录当中。值得一提的是，2015 年之后发布的英特尔芯片都在内部都运行着 MINIX 3，作为 Intel 管理引擎(Intel Management Engine)的组件。

## 2.3 GNU 计划

到了1985年，为了避免GNU所开发的自由软件被其他人所利用而成为专利软件， 所以他与律师草拟了有名的通用公共许可证(General Public License, GPL)， 并且称呼为 copyleft (相对于专利软件的 copyright！)。

GNU 项目开发出许多高质量的免费软件，其中包括有名的 Emacs 编辑系统、Bash Shell 程序、Gcc 系列编译程序、Gdb 调试程序等等。不过，对于 GNU 的最初构想『建立一个自由的 Unix 操作系统』来说，有这些优秀的程序是仍无法满足的，因为，当时并没有『自由的 Unix 核心』存在。所以这些软件仍只能在那些有专利的 Unix 平台上工作，一直到 Linux 的出现。

# 0x03. Linux

1988 年，Linus Torvalds（林纳斯·托瓦兹）进入了赫尔辛基大学，并选读了计算机科学系。在读期间，接触到了 Unix 系统。当时整个赫尔辛基只有一部最新的 Unix 系统，同时仅提供16个终端（terminal）。早期的计算机仅有主机具有运算功能，terminal 仅负责提供 Input/Output 而已。在这种情况下，实在很难满足托瓦兹的需求，不久之后，他就知道有一个类似 Unix 的系统，并且与 Unix 完全兼容，还可以在 Intel 386 机器上面运行的操作系统——Minix。托瓦兹贷款购买了一台 Intel 386 个人计算机后，便安装了 Minix 操作系统，并通过 Minix 操作系统附带的源代码，学习到了很多内核程序设计的理念。

由 Andrew Tanenbaum（谭宁邦）教授只愿意将 Minix 系统用于教学，不愿意强化系统功能，导致大部分工程师不满足于已有的功能，也使托瓦兹萌生自己编写操作系统的想法。而 GNU 计划产出的 Bash 工作环境和 Gcc 编译程序等自由软件，让托瓦兹能够顺利的编写内核程序。最终，在 1991 年，他写出了能够在 386 计算机上运行的内核，并将其上传到网络上，提供下载。为了让更多 Unix 系统上的软件运行在 Linux 上，托瓦兹参考 POSIX 标准规范修改 Linux，提高了与 Unix 系统的兼容性。

## 3.1 Linux 命名规则

Linux 内核版本号命名的规则：r.x.y

r：表示目前发布的内核主版本。

x：x是偶数表示稳定版本，主要用于企业等生产环境; 若是奇数表示开发中版本，主要用于开发和测试新功能。

y: 修订版本号，表示修改的次数。

## 3.2 Linux 发行版本

Linux 发行版中会为用户预先集成好 Linux 操作系统及各种应用软件。通常来讲，一个Linux发行版包括Linux内核，以及将整个软件安装到电脑上的一套安装工具，还有各种GNU软件，和其他的一些自由软件，在一些Linux发行版中可能会包含一些专有软件。发行版为许多不同的目的而制作，包括对不同电脑硬件结构的支持，对普通用户或开发者使用方式的调整，针对实时应用或嵌入式系统的开发等等。目前，超过三百个发行版被积极的开发，最普遍被使用的发行版有大约十多个。较为知名的有 Debian、Ubuntu、Fedora、RHEL Red Hat Enterprise Linux、Arch Linux和openSUSE等。

一个典型的发行版包括：Linux内核，GNU函式库和各种系统工具，命令行Shell，图形界面底层的X窗口系统和上层的桌面环境等。桌面环境有如KDE或GNOME等，并包含数千种从办公包，编译器，文本编辑器，小游戏，儿童教育软件，到科学工具的应用软件。

完整详细的发行版历史可以看 [wiki:Linux发行版](https://zh.wikipedia.org/wiki/Linux%E5%8F%91%E8%A1%8C%E7%89%88)


## 3.4 软件包管理系统

**软件包管理系统** 是在电脑中自动安装、配制、卸载和升级软件包的工具组合，在各种系统软件和应用软件的安装管理中均有广泛应用。

RPM（Red Hat Package Manager）和 Deb（Debian Package）是两种最常见的Linux软件包管理格式。它们用于在Linux系统上方便地安装、升级和卸载软件包。RPM 和 Deb 包都是二进制软件包格式，它们包含了应用程序的二进制代码、配置文件、文档和依赖关系等信息。RPM包和Deb包的结构和内容有所不同，但它们都包含了软件包的元数据信息，如软件包名称、版本号、作者、许可证、依赖关系等。这些元数据信息可以帮助系统管理工具（如yum、dnf、apt-get等）确定软件包的安装、升级和卸载方式。

在Linux发行版中，几乎每一个发行版都有自己的软件包管理系统。常见的有：

- 管理 deb 软件包的管理工具叫 dpkg ，它的前端 APT（用于Debian、Ubuntu）。
- 管理 RPM 软件包的管理工具也叫 RPM ，它的前端 dnf（用于Fedora、Red Hat Enterprise Linux 8、CentOS 8）、前端 yum（用于Red Hat Enterprise Linux、CentOS）、前端 ZYpp（用于openSUSE）、前端 urpmi（用于Mandriva Linux、Mageia）等。

使用软件包管理系统将大大简化在Linux发行版中安装软件的过程。

**按照打包方式划分：**

- Debian 系

    Debian GNU / Linux 是一种强调使用自由软件的发行版，支持多种硬件平台。Debian 及其派生发行版使用 `deb` 软件包格式，并使用 `dpkg` 及其前端作为软件包管理器。

    例如 Ubuntu ，就是基于 Debian 。

- Red Hat 系

    Red Hat Linux 和 SUSE Linux 是最早使用 `RPM` 格式软件包的发行版，如今 RPM 格式已广泛运用于众多的发行版。这两种发行版后来都分为商业版本和社区支持版本。Red Hat Linux 的社区支持但受红帽公司赞助的版本现称为 `Fedora`，商业版本则称为 `Red Hat Enterprise Linux`。SUSE Linux则分成了 `SUSE Linux Enterprise` 和 `openSUSE`。

    例如 Centos ，由社群支持的包，旨在 100% 地与 Red Hat Linux 企业版兼容，但不包含 Red Hat 的商业软件。

- Slackware 系

    Slackware 走了一条同其他的发行版本（Red Hat、Debian、Gentoo、SuSE、 Mandriva、Ubuntu等）不同的道路，它力图成为 UNIX风格 的 Linux 发行版本。它的方针是只吸收稳定版本的应用程序，并且缺少其他Linux版本中那些为发行版本定制的配置工具。

- Arch 系

    起于 Arch Linux，通常不采跨版本升级而是通过滚动更新来提供大多数软件的最新稳定版本。使用 `pacman` 作为包管理器，具有依赖处理和构建软件包的功能。除此之外，用户仓库 AUR 由世界各地的 Arch Linux 用户驱动，提供了海量的非官方软件以供选择。

    例如 Arch Linux 、 Manjaro

- 其它

    例如 Gentoo 这个包采用自己独特的 `Portage` 包管理系统，吸引了许多狂热爱好者以及专业人士，由于能自己编译及调整源码依赖等选项，而获得至高的自定义性及优化的软件，在源码包也有相当多新旧版本的选择，是个强调能自由选择的发行版。

    例如 Linux From Scratch: 这是一份文档，介绍如何自己动手，如何白手起家编译打造自己独一无二的Linux系统。

# 0x04. Centos(Community Enterprise Operating System)

2019 年，CentOS 团队在发布 CentOS Linux 8 时，同时发布了一个新的 CentOS 版本: CentOS Stream 8。

CentOS Linux 8 和 CentOS Stream 8 的关系如下:

- CentOS Linux 作为 RedHat Enterprise Linux (RHEL) 的下游版本。
- CentOS Stream 作为 RHEL 的滚动预览版本，将会是 Fedora 的下游版本，RHEL 的上游版本。

这是什么意思呢？总体而言，现在的关系类似于这样（注意，下游比上游稳定）:

Fedora -> CentOS Stream -> RedHat Enterprise Linux -> CentOS Linux

Fedora 和 CentOS Stream 将具有滚动更新，共同作为 RHEL 的测试场地，然后将稳定的功能推向 RHEL。而 CentOS Linux 则依旧作为 RHEL 的 重构建 版本，拥有着和 RHEL 类似的稳定性。这意味着 CentOS 将不再是 RHEL 的克隆版，而是 RHEL 的预览版。

## 4.1 转折

2020 年 12 月 8 日，CentOS 的官方博客发布了一篇文章: [CentOS Project shifts focus to CentOS Stream](https://blog.centos.org/2020/12/future-is-centos-stream/) (CentOS 项目将会把项目重心转移到 CentOS Stream)，“未来是属于 CentOS Stream 的 (future-is-centos-stream)”。因为 Centos 项目被 Red Hat 收购，而 Red Hat 被 IBM 收购。

总而言之就是一句：天下免费的午餐要没了，要稳定快滚去买 RHEL，要么就用 CentOS Stream 当小白鼠。

CentOS Linux 停止支持的决定，让 CentOS 偏离了其原始的目标：**RHEL 的重构建版本**。长期以来， CentOS 一直是 Linux 发行版的可靠选择，特别是对服务器来说，因为它实际上是 Red Hat Enterprise Linux 的免费版本。

在此事件之后，出现了众多的 RHEL 重构建项目，比如 AlmaLinux。

而 CentOS 的创始人，Gregory Kurtzer，也因此启动了新项目，**Rocky Linux**，来实现 CentOS 的最初目标。这个名称中的 Rocky 起源于 CentOS 的联合创始人 Rocky McGaugh。

在项目宣布两天之后，Rocky Linux(2020 年首次发布 8.2 ) 的 GitHub Repo 就获得了 2000+ Star，可见很多人对 RedHat 的举动抱有怨念。

Rocky 8.4 对应内核版本 4.18.0

## 4.2 Rocky Linux

区别：

- 开发团队
    
    Rocky Linux的开发由CloudLinux公司负责，而CentOS则完全由社区志愿者组成，这意味着Rocky Linux可以更快地获得更新和支持，而CentOS则依赖于社区的贡献。

- 商业模式

    Rocky Linux是一个免费的发行版，而CentOS之前是一个免费的发行版，但在2020年宣布将转向商业化模式，这意味着从2024年开始，用户需要购买许可证才能继续使用CentOS。

- 软件包管理

    Rocky Linux使用了新的YUMI工具来安装和管理软件包，而CentOS仍然使用传统的YUM工具，这使得Rocky Linux在软件包管理和升级方面更加灵活和现代化。

- 兼容性

    Rocky Linux和CentOS都致力于与RHEL保持高度兼容性，但由于开发团队的不同，它们在某些特定情况下可能会存在一些差异，对于特定的应用程序或系统配置，用户可能需要进行额外的测试和调整。

相同：

- 源代码基础

    两者都是基于Red Hat Enterprise Linux（RHEL）的源代码构建的，因此它们共享了大量的软件包和功能，这使得用户可以无缝地从一个发行版迁移到另一个发行版。

- 稳定性和安全性

    由于它们都基于RHEL，Rocky Linux和CentOS都提供了高度稳定和安全的操作系统环境，它们经过了广泛的测试和验证，适用于各种服务器应用场景。

- 社区支持

    虽然开发团队不同，但Rocky Linux和CentOS都拥有庞大的社区支持，用户可以通过社区论坛、邮件列表和其他渠道获取帮助和解决问题。

升级路径:  
不支持从7直接升级到9，需要先升级到8，再升级到9
```
CentOS 7 --> Rocky Linux 8.9 --> Rocky Linux 9.3
```

注意事项
- Rocky Linux 9 不再支持 network.service 管理网络接口，需要使用 NetworkManager，并且保证 NetworkManager 服务开机自启动，否则升级后重启连不上服务器
- 原地升级有风险，请备份好数据，做好快照
- 重装系统可能是更好的选择，如果非得要原地升级，请在测试环境做好充足的验证