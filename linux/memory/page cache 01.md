# 0x00、导读

# 0x01、简介

[vmtouch](../perf/command/vmtouch.md)

2. 
```bash
# page-type 获得步骤
$ wget https://github.com/torvalds/linux/archive/refs/tags/v5.13.tar.gz
$ tar -xzf ./v5.13.tar.gz
$ cd linux-5.13/tools/vm
$ make
$ cp page-types /usr/bin
```
[pagemap.txt](https://www.kernel.org/doc/html/v5.0/admin-guide/mm/pagemap.html)

3. `mincore()` 报告在一个虚拟地址范围中哪些分页当前驻留在 RAM 中。

x. 
- procfs files: /proc/PID/smaps, /proc/pid/pagemap, /proc/kpageflags, /proc/kpagecgroup and sysfs file: /sys/kernel/mm/page_idle;
- system calls: mincore(), mmap(), fsync(), msync(), posix_fadvise(), madvise() and others;
- different open and advise flags O_SYNC, FADV_DONTNEED, POSIX_FADV_RANDOM, MADV_DONTNEED, etc.

测试文件:
```bash
$ dd if=/dev/random of=/var/tmp/file1.db count=128 bs=1M
```

清理缓存:
```bash
$ sync; echo 3 | sudo tee /proc/sys/vm/drop_caches
```

# 0x02、理论

Page Cache 本质上是 VFS(Virtual File System) 的一部分，其主要目的是为了改善 IO 读写操作的延迟。

linux 对文件的访问除了显示使用 O_DIRECT 标志，所有的访问都会经过 page cache 。

回写缓存算法是页缓存的核心构建块。[0x01 cache 策略](./cache02.md)


![write-back](../../pic/linux/memory/page-cache.png)