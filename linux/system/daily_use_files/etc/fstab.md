# 0x00. 导读

# 0x01. 简介

我们将挂载信息填入 `/etc/fstab` 文件后，系统在每次开机时都自动将其挂载。

# 0x02. 

```
#  <fs>          <mountpoint>                   <type>    <opts>      <dump/pass>
/dev/mapper/centos-root /                       xfs     defaults        1 1
/dev/mapper/centos-swap swap                    swap    defaults        0 0

<opts>: 文件系统参数
<dump> 备份命令：dump(一个程序的名字) 会检查 entry 并用数字来决定是否对这个文件系统进行备份。0 不备份，1 备份。
<pass> 是否以fsck检验扇区：启动的过程中，系统默认会以fsck检验我们的 filesystem 是否完整 (clean)。 不过，某些 filesystem 是不需要检验的，例如内存置换空间 (swap) ，或者是特殊文件系统例如 /proc 与 /sys 等等。fsck会检查这个头目下的数字来决定检查文件系统的顺序，允许的数字是0, 1, 和2。0 是不要检验， 1 表示最早检验(一般只有根目录会配置为 1)， 2 也是要检验，不过1会比较早被检验啦！一般来说,根目录配置为1,其他的要检验的filesystem都配置为 2 就好了。
```

opts 常用参数：
- noatime 关闭atime特性，提高性能，这是一个很老的特性，放心关闭，还能减少loadcycle
- defaults 使用默认设置。等于rw,suid,dev,exec,auto,nouser,async，具体含义看下面的解释。
- 自动与手动挂载:
    - auto 在启动或在终端中输入mount -a时自动挂载
    - noauto 设备（分区）只能手动挂载
- 读写权限:
    - ro 挂载为只读权限
    - rw 挂载为读写权限
- 可执行:
    - exec 是一个默认设置项，它使在那个分区中的可执行的二进制文件能够执行
    - noexec 二进制文件不允许执行。千万不要在你的root分区中用这个选项！！！
- I/O同步:
    - sync 所有的I/O将以同步方式进行
    - async 所有的I/O将以非同步方式进行
- 户挂载权限:
    - user 允许任何用户挂载设备。 Implies noexec,nosuid,nodev unless overridden.
    - nouser 只允许root用户挂载。这是默认设置。
- 临时文件执行权限：
    - suid Permit the operation of suid, and sgid bits. They are mostly used to allow users on a computer system to execute binary executables with temporarily elevated privileges in order to perform a specific task.（允许suid和sgid位的操作。它们主要用于允许计算机系统上的用户执行具有临时提升权限的二进制可执行文件，以执行特定任务。）
- nosuid Blocks the operation of suid, and sgid bits.（阻止suid和sgid位的操作。）