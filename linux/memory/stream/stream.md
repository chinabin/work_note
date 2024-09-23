# 0x00. 导读

[官方源码地址 stream.c](http://www.cs.virginia.edu/stream/FTP/Code/stream.c)

# 0x01. 简介

Stream 是业界主流的内存带宽测试程序，测试行为相对简单可控。该程序对 CPU 的计算能力要求很小，对 CPU 内存带宽压力很大。目前处理器核心数量在不断增大，而内存带宽并没有随之成线性增长，因此内存带宽对提升多核心的处理能力就越发重要。Stream 具有良好的空间局部性，是对 TLB 友好，Cache 友好的一款测试程序，其分为复制（Copy）、尺度变换（Scale）、矢量求和（Add）、复合矢量求和（Triad）四个更基本的测试功能：

- Copy 是复制操作，从一个内存单元中读取一个数，并复制到另一个内存单元，有2次访存操作。
- Scale 是乘法操作，从一个内存单元中读取一个数，与常数scale相乘，得到的结果写入另一个内存单元，有2次访存。
- Add 是加法操作，从两个内存单元中分别读取两个数，将其进行加法操作，得到的结果写入另一个内存单元中，有2次读和1次写共3次访存。
- Triad 是将 Copy、Scale、Add 三种操作组合起来进行测试。具体操作方式是：先从内存中读取一个数，与scale相乘得到一个乘积，然后从另一个内存单元中读取一个数与之前的乘积相加，得到的结果再写入内存。所以，有2次读和1次写共3次访存操作。

测试的内存带宽Add>Triad>Copy>Scale。因为访存次数越多，内隐藏的访存延迟越大，得到的带宽越大。同理，运算的操作越复杂，操作时间就越长，程序运行时间就越长，得到的访存带宽就相应减少。这就是为什么3次访存的操作得到的带宽比2次访存操作得到的要大，而相同访存次数的操作，加法要比乘法得到的结果要好。

结果：Best Rate 越高越好。

如果是测试内存延迟，得用 MLC 工具。

# 0x02. 安装

上传 Stream.c 到待测试机器（然后用 stream_test.sh 脚本就完事了）。编译：
```bash
$ gcc -O3 -fopenmp -DSTREAM_ARRAY_SIZE=2000000 -DNTIMES=10 stream.c -o stream
```

编译参数解释：
```
-O3：指定最高编译优化级别，即3

-fopenmp：启用OpenMP，适应多处理器环境，更能得到内存带宽实际最大值。开启后，程序默认运行线程为CPU线程数

-DSTREAM_ARRAY_SIZE=2000000：表示测试数据集的大小, Byte。

-DNTIMES=10：执行的次数，并从这些结果中选最优值。

其他参数：

-mtune=native -march=native：针对CPU指令的优化，此处由于编译机即运行机器。故采用native的优化方法。更多编译器对CPU的优化参考
-mcmodel=medium ；当单个 Memory Array Size 大于 2GB 时需要设置此参数
-DOFFSET=4096 ；数组的偏移，一般可以不定义
```

注意：
1. DSTREAM_ARRAY_SIZE 过大的情况下编译，stream 运行需要的内存过大，导致产生段错误 (Segmentation fault)，出现该情况下可以增大内存或者减小 DSTREAM_ARRAY_SIZE。
2. stream_test.sh 别搞那些优化选项，不然结果无法解释（比最大理论带宽还大）。 `gcc -O3 stream.c -DSTREAM_ARRAY_SIZE=$array_size -DNTIME=30 -mcmodel=medium -o stream.o` 就行。

# 0x04. 例子

可以先用 dd 命令感受一下：
```bash
$ dd if=/dev/zero of=/dev/null bs=4096 count=1048576
记录了1048576+0 的读入
记录了1048576+0 的写出
4294967296字节（4.3 GB，4.0 GiB）已复制，1.20857 s，3.6 GB/s
```

```bash
$ sudo bash stream_test.sh
-------------------------------------------------------------
STREAM version $Revision: 5.10 $
-------------------------------------------------------------
This system uses 8 bytes per array element.
-------------------------------------------------------------
Array size = 2692349952 (elements), Offset = 0 (elements)
Memory per array = 20541.0 MiB (= 20.1 GiB).
Total memory required = 61623.0 MiB (= 60.2 GiB).
Each kernel will be executed 10 times.
 The *best* time for each kernel (excluding the first iteration)
 will be used to compute the reported bandwidth.
-------------------------------------------------------------
Your clock granularity/precision appears to be 1 microseconds.
Each test below will take on the order of 1980356 microseconds.
   (= 1980356 clock ticks)
Increase the size of the arrays if this shows that
you are not getting at least 20 clock ticks per test.
-------------------------------------------------------------
WARNING -- The above is only a rough guideline.
For best results, please be sure you know the
precision of your system timer.
-------------------------------------------------------------
Function    Best Rate MB/s  Avg time     Min time     Max time
Copy:           14284.3     3.214871     3.015721     3.814341
Scale:           8322.3     5.485085     5.176137     6.091701
Add:            10585.6     7.052315     6.104177     7.726443
Triad:          12919.1     5.578810     5.001622     6.212581
-------------------------------------------------------------
Solution Validates: avg error less than 1.000000e-13 on all three arrays
-------------------------------------------------------------
```


```bash
# 看看插了几根内存条
$ sudo dmidecode | grep -P -A5 "Memory\s+Device" | grep Size | grep -v Range | grep -v "No Module Installed" | wc -l
8

# 每根多大
$ sudo dmidecode | grep -P -A5 "Memory\s+Device" | grep Size | grep -v Range | grep -v "No Module Installed"
        Size: 8 GB
        Size: 8 GB
        Size: 8 GB
        Size: 8 GB
        Size: 8 GB
        Size: 8 GB
        Size: 8 GB
        Size: 8 GB

# 每根的频率，数值就是所谓的 标称频率 的数值
$ sudo dmidecode | grep -A16 "Memory Device" | grep 'Speed'
        Speed: 2133 MT/s
        Speed: 2133 MT/s
        Speed: 2133 MT/s
        Speed: 2133 MT/s
        Speed: 2133 MT/s
        Speed: 2133 MT/s
        Speed: 2133 MT/s
        Speed: 2133 MT/s
```

2133 * 64（位宽）/ 8（bit 转 Byte） = 2133 * 8 = 17064 MB/s

从 SDRAM-DDR 时代，数据总线位宽时钟没有改变，都为 64bit，但是采用双通道技术，可以获得 64X2=128bit 的位宽。ddr5 单个内存条里面就是双通道的，但是单通道位宽是 32bit.