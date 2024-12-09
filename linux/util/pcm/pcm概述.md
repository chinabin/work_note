# 0x00. 导读

Intel Performance Counter Monitor (Intel PCM), 是 Intel 公司开发的用于监控 Intel 处理器的性能和功耗的 API 以及基于这个 API 的一系列工具。

[pcm](https://github.com/intel/pcm)

# 0x01. 简介

# 0x02. 

基础的处理器监控工具（每个 cycle 的指令，核心频率-包括 Intel Turbo Boost 技术和 intel_pstate ，内存，Intel 快速路径内部连接带宽(Intel Quick Patch Interconnct bandwidth)，本地和远程内存带宽，缓存未命中，core 和 CPU 封装(CPU package)睡眠 C-state 定位(residency)，core 和 CPU 封装的热余量(thermal headroom)，缓存使用，CPU 和内存能耗(energy consumption)。

通过 [pcm-exporter :Intel Performance Counter Monitor (Intel PCM) Prometheus exporter](https://cloud-atlas.readthedocs.io/zh-cn/latest/performance/intel_pcm/pcm-exporter.html#pcm-exporter) 可以将 pcm 的监控指标输出到 [Intel PCM Grafana](https://cloud-atlas.readthedocs.io/zh-cn/latest/performance/intel_pcm/pcm-grafana.html#pcm-grafana)


```bash
$ git clone --recursive https://github.com/intel/pcm
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . # or cmake --build . --parallel, 更快

# 程序会生成在 build/bin, 我在 centos 7 上编译了一份
```

```
pcm: CPU、cache
pcm-memory:  监控内存带宽(每个通道和每个DRAM DIMM rank)
pcm-latency: L1 cahce
pcm-pcie: 监视每个socket的PCIe带宽
pcm-numa: 检查 NUMA架构 本地和远程内存访问
pcm-power: 能耗
pcm-core 和 pmu-query: 查询和监视任意处理器核心事件
pcm-bw-histogram: 收集内存带宽利用率直方图
```

