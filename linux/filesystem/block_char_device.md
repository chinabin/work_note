# 0x00. 导读

设备：块设备、字符设备

# 0x01. 简介

**内核管理块设备要比管理字符设备复杂。**  

因为字符设备仅仅需要控制一个位置，即当前位置，而块设备访问的位置必须能够在介质的不同区间前后移动。所以内核不必提供一个专门的子系统来管理字符设备，但是对于块设备的管理却必须要有一个专门提供服务的子系统。  

不仅仅是因为块设备的复杂性远远高于字符设备，更重要的原因是块设备对执行性能要求很
高；对硬盘每多一分利用都会对系统的整体性能带来提升，其效果要远远比键盘吞吐速度成
倍的提高大得多。

# 0x02. 设备分类

设备可以分为两类：

- 块设备：能够随机访问固定大小数据片(chunks)的硬件设备。如硬盘。
- 字符设备：按照字符流有序访问的硬件设备。如键盘、串口。

比如键盘，系统只能以有序的流方式读取键盘输入 h-e-l-l-o ，如果随机读取 e-l-o-h-o ，则这些输入的字符就没有意义了。

块设备访问有两种方式，一是直接访问裸分区（如 /dev/sda ），另一种是访问里面文件系统（如 ext4/f2fs ）。


字符设备与块设备区别：
- 访问数据是否有序
- 字符设备是驱动中自己实现 file_operations 
- 块设备是 file_operations 被文件系统层实现了，并不需要在块驱动中再去实现了，虽然已经被抽象但本质上还是存在 file_operations 这一实现的。

`cat /proc/devices` 可以查看系统上的设备。（输出中，左侧为设备编号，右侧为设备名字）

`ls -l` 的输出结果中， `b` 表示 块设备， `c` 表示字符设备。例如 `ls -l /dev/`

# 0x03. block device

块设备中最小的可寻址单元是扇区 (sector) 。扇区大小一般是2的整数倍，而最常见的是 512 字节。**扇区的大小是设备的物理属性，扇区是所有块设备的基本单元，块设备无法对比它还小的单元进行寻址和操作。**

虽然物理磁盘寻址是按照扇区级进行的，但是内核执行的所有磁盘操作都是按照块 (block) 进行的。由于扇区是设备的最小可寻址单元，所以块不能比扇区还小，只能数倍于扇区大小。具体要求是必须是扇区大小的 2 的整数倍，并且要小于页面大小（估计是为了用页缓存方便）。所以通常块大小是 512 字节、1KB 或 4KB。

**扇区是块设备的最小寻址单元；块是文件系统的最小寻址单元。**
