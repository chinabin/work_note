# 0x00. 导读

# 0x01. 简介

测试不同 numa 策略的性能差距

# 0x02. 

```bash
$ numademo
usage: numademo [-S] [-f] [-c] [-e] [-t] msize[kmg] {tests}
No tests means run all.
-c output CSV data. -f run even without NUMA API. -S run stupid tests. -e exit on error
-t regression test; do not run all node combinations
valid tests: memset memcpy forward backward stream random2 ptrchase

# 例子
# 执行以下命令（采用4g数据内存复制）可以对比不同NUMA策略的性能
$ numademo 4g memset
```