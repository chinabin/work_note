# 0x00. 导读

# 0x01. 简介

[intel/SDM-Processor-Topology-Enumeration](https://github.com/intel/SDM-Processor-Topology-Enumeration)

我已经下载编译好了。

1. compile
    ```bash
        gcc -g -c -Wall linux_os_util.c
        gcc -g -c -Wall cpuid_topology_display.c
        gcc -g -c -Wall cpuid_topology_file.c
        gcc -g -c -Wall cpuid_topology_parsecachetlb.c
        gcc -g -c -Wall cpuid_topology_parsecpu.c
        gcc -g -c -Wall cpuid_topology_tools.c
        gcc -g  cpuid_topology.c -Wall -o cpu_topology64.out linux_os_util.o cpuid_topology_display.o cpuid_topology_file.o cpuid_topology_parsecachetlb.o  cpuid_topology_parsecpu.o cpuid_topology_tools.o
    ```

2. 使用

    对应不同的展示信息：
    ```
    0 - Display the topology via OS APIs (Not valid with File Load)
    1 - Display the topology via CPUID
    2 - Display CPUID Leaf values one processor
    3 - Display CPUID Leaf values all processors
    4 - Display APIC ID layout
    5 - Display TLB Information
    6 - Display Cache Information
    ```
    ```bash
    $ sudo ./cpu_topology64.out C 0
    $ sudo ./cpu_topology64.out C 1
    $ sudo ./cpu_topology64.out C 2
    $ sudo ./cpu_topology64.out C 3
    $ sudo ./cpu_topology64.out C 4
    $ sudo ./cpu_topology64.out C 5
    $ sudo ./cpu_topology64.out C 6
    ```

# 0x02.