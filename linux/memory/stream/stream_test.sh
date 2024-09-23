#!/bin/bash
# 获取可用 memory
available_memory_size=`free -m | grep Mem | awk '{print ($4)*1024*1024}'`
# 计算最大 array_size
array_size=$((available_memory_size/8/3))
# 编译 stream
# gcc -O3 stream.c -mtune=native -march=native -fopenmp -DSTREAM_ARRAY_SIZE=$array_size -DNTIME=30 -mcmodel=medium -o stream.o
gcc -O3 stream.c -DSTREAM_ARRAY_SIZE=$array_size -DNTIME=30 -mcmodel=medium -o stream.o
# 执行测试
echo 3 > /proc/sys/vm/drop_caches
./stream.o