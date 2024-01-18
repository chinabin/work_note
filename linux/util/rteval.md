# 0x00. 导读

和 hwlatdetect 一样，看个乐子

[rteval](https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/rteval)

# 0x01. 简介

用来测试系统在负载下的实时性能。

> Rteval is a python program written to evaluate the performance of a
realtime Linux kernel on a particular hardware platform. 

>The program unpacks source code for two loads: hackbench and a Linux kernel
compile, then loops running hackbench and a parallel build of the
Linux kernel. While the loads are running, the cyclictest program is
run to measure realtime performance under load. When the specified run
duration is met, the loads are stopped and cyclictest outputs measured
timer latency values in histogram format, which is analyzed by
rteval. Rteval then writes an XML file to disk with information about
the system (gotten through DMI tables), the raw histogram data
collected during the run and the statistical analysis of the run.

# 0x02. 

```bash
$ rteval --summarize rteval-<date>-n.tar.bz2
```