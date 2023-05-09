# 0x00 导读

# 0x01 简介

一些问题。

# 0x02 伪共享

两个全局变量 A 和 B 声明在一起，使得它们落在同一个 `cache line` ，两个进程 P1 和 P2 ，P1 在 CPU0 上运行并修改 A ，P2 在 CPU1 上运行并修改 B ，因为缓存一致性协议，这样会造成 `cache 颠簸`。 A 和 B 之间就是 `伪共享关系`，实际并没有共享。

既然 A 和 B 由于在同一行 cache line 导致了伪共享问题，那么解决方案也很显然易见，我们可以让 A 和 B 不落在一个 cache line ，这样就可以解决问题。

不落在同一行 cache line 的方法很简单，使 A 和 B 的内存地址都按照 cache line size 对齐，相当于以空间换时间。浪费一部分内存，换来了性能的提升。

当我们把 A 和 B 都 cache line size 对齐后，我们再思考上面的问题。此时 CPU0 和 CPU1 分别修改 A 和 B 互不影响。 A 和 B 对应的 cache line 状态可以一直维持 Modified 状态。这样 MESI 协议就不会在两个 CPU 间不停的发送消息。降低了带宽压力。

在 Linux kernel 中存在 `__cacheline_aligned_in_smp` 宏定义用于解决 false sharing 问题。
```c
#ifdef CONFIG_SMP
#define __cacheline_aligned_in_smp __cacheline_aligned
#else
#define __cacheline_aligned_in_smp
#endif
```
在单核系统上，宏定义为空。在多核系统下，该宏是 `L1 cach line size` 。针对静态定义的全局变量，如果在多核之间竞争比较严重，为了避免影响其他全局变量，可以采用上面的宏使变量 cache line 对齐，避免 false sharing 问题。


# 0x03 题目

[出处](https://coolshell.cn/articles/20793.html)

```c++
// 第二个循环要比第一个循环少4倍的计算量，其应该也是要快4倍的。
const int LEN = 64*1024*1024;
int *arr = new int[LEN];

for (int i = 0; i < LEN; i += 2) arr[i] *= i;
for (int i = 0; i < LEN; i += 8) arr[i] *= i;
// 其实相差无几。因为CPU会以一个 Cache Line 64Bytes 最小时单位加载，也就是 16 个 32bits 的整型，所以，无论你步长是2还是8，都差不多。
```

```c++
// size = 横轴 * 纵轴
// increment = 横轴
for (int i = 0; i < 10000000; i++) {
    for (int j = 0; j < size; j += increment) {
        memory[j] += j;
    }
}
/*
| count |   1    |   16  |  512  | 1024  |
------------------------------------------
|     1 |  17539 | 16726 | 15143 | 14477 |
|     2 |  15420 | 14648 | 13552 | 13343 |
|     3 |  14716 | 14463 | 15086 | 17509 |
|     4 |  18976 | 18829 | 18961 | 21645 |
|     5 |  23693 | 23436 | 74349 | 29796 |
|     6 |  23264 | 23707 | 27005 | 44103 |
|     7 |  28574 | 28979 | 33169 | 58759 |
|     8 |  33155 | 34405 | 39339 | 65182 |
|     9 |  37088 | 37788 | 49863 |156745 |
|    10 |  41543 | 42103 | 58533 |215278 |
|    11 |  47638 | 50329 | 66620 |335603 |
|    12 |  49759 | 51228 | 75087 |305075 |
|    13 |  53938 | 53924 | 77790 |366879 |
|    14 |  58422 | 59565 | 90501 |466368 |
|    15 |  62161 | 64129 | 90814 |525780 |
|    16 |  67061 | 66663 | 98734 |440558 |
|    17 |  71132 | 69753 |171203 |506631 |
|    18 |  74102 | 73130 |293947 |550920 |
*/
// 从 [9，1024] 以后，时间显著上升。包括 [17，512] 和 [18,512] 也显著上升。
// 这是因为，我机器的 L1 Cache 是 32KB, 8 Way 的，前面说过，8 Way的有64组，每组8个Cache Line，当for-loop步长超过1024个整型，也就是正好 4096 Bytes时，也就是导致内存地址的变化是变化在高位的24bits上，而低位的12bits变化不大，尤其是中间6bits没有变化，导致全部命中同一组set，导致大量的cache 冲突，导致性能下降，时间上升。而 [16, 512]也是一样的，其中的几步开始导致L1 Cache开始冲突失效。
```

```c++
const int row = 1024;
const int col = 512
int matrix[row][col];

//逐行遍历
int sum_row=0;
for(int _r=0; _r<row; _r++) {
    for(int _c=0; _c<col; _c++){
        sum_row += matrix[_r][_c];
    }
}

//逐列遍历
int sum_col=0;
for(int _c=0; _c<col; _c++) {
    for(int _r=0; _r<row; _r++){
        sum_col += matrix[_r][_c];
    }
}
// 执行时间有十几倍的差距。其中的原因，就是逐列遍历对于CPU Cache 的运作方式并不友好，所以，付出巨大的代价。
```

```c++
void fn (int* data) {
    for(int i = 0; i < 10*1024*1024; ++i)
        *data += rand();
}

int p[32];

int *p1 = &p[0];
int *p2 = &p[1]; // int *p2 = &p[30];

thread t1(fn, p1);
thread t2(fn, p2);
// 因为 p[0] 和 p[1] 在同一条 Cache Line 上，而 p[0] 和 p[30] 则不可能在同一条Cache Line 上 ，CPU的缓存最小的更新单位是Cache Line，所以，这导致虽然两个线程在写不同的数据，但是因为这两个数据在同一条Cache Line上，就会导致缓存需要不断进在两个CPU的L1/L2中进行同步，从而导致了5倍的时间差异。
```

```c++
int total_size = 16 * 1024 * 1024; //数组长度
int* test_data = new test_data[total_size]; //数组
int nthread = 6; //线程数（因为我的机器是6核的）
int result[nthread]; //收集结果的数组

void thread_func (int id) {
    result[id] = 0;
    int chunk_size = total_size / nthread + 1;
    int start = id * chunk_size;
    int end = min(start + chunk_size, total_size);

    for ( int i = start; i < end; ++i ) {
        if (test_data[i] % 2 != 0 ) ++result[id];
    }
}
```
在执行过程中，你会发现，6个线程居然跑不过1个线程。因为根据上面的例子你知道 result[] 这个数组中的数据在一个Cache Line中，所以，所有的线程都会对这个 Cache Line 进行写操作，导致所有的线程都在不断地重新同步 result[] 所在的 Cache Line，所以，导致 6 个线程还跑不过一个线程的结果。这叫 False Sharing。
改进如下，使用一个线程内的变量。
```c++
void thread_func (int id) {
    result[id] = 0;
    int chunk_size = total_size / nthread + 1;
    int start = id * chunk_size;
    int end = min(start + chunk_size, total_size);

    int c = 0; //使用临时变量，没有cache line的同步了
    for ( int i = start; i < end; ++i ) {
        if (test_data[i] % 2 != 0 ) ++c;
    }
    result[id] = c;
}
```

# 0x04 SpinLock 的历史进化

[spinlock前世今生](https://zhuanlan.zhihu.com/p/133445693)

通过 spinlock 的实现变化，讲述优化。