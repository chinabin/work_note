# 0x00 导读

传入 Cache 中的地址是虚拟地址或者物理地址可以划分不同种类。

# 0x01 简介

歧义(ambiguity)   
别名(alias)

# 0x02 问题

# 2.1 歧义(ambiguity)

一个虚拟地址映射不同的物理地址。

**具体表现是那些不同的物理地址对应的虚拟地址的 `index` 和 `tag` 相同。**

例如进程 A 在虚拟地址 0x1234 存了数据，进程切换之后，进程 B 去取 0x1234 就会得到错误的数据。

# 2.2 别名(alias)

不同的虚拟地址映射相同的物理地址。

**具体表现是那些不同的虚拟地址的 `index` 相同。**

例如虚拟地址 0x1234 在 cache 中存在两份数据，当发生改动并且才用 write-back 策略，则另一份数据就是错的了。

# 0x03 虚拟高速缓存(VIVT Virtual Index Virtual Tag)

虚拟地址做 `index` ，虚拟地址做 `tag` ，传入 Cache 的地址是虚拟地址。

VIVT 存在 歧义 和 别名 问题，对于歧义，解决办法是：
- 切换进程的时候，可以选择 flush 所有的 cache。使得内存中的数据是最新且有效的，并且使得 Cache 里面的数据无效。

对于别名，解决办法是：
- 不使用 cache 。

# 0x04 物理高速缓存(PIPT Physical Index Physical Tag)

物理地址做 `index` ，物理地址做 `tag` ，传入 Cache 的地址是 MMU 转换后的物理地址。

天然就不存在 歧义 和 别名 问题。

并且为了加快 MMU 转换地址，还加了一个 TLB 的硬件。

# 0x05 物理标记的虚拟高速缓存(VIPT Virtual Index Physical Tag)

## 5.1

虚拟地址做 `index` ，物理地址做 `tag` ，传入的还是虚拟地址。

使用虚拟地址对应的 `index` 位查找 cache ，与此同时(硬件上同时进行)将虚拟地址发到 MMU 转换成物理地址。当 MMU 转换完成，同时 cache 控制器也查找完成，此时比较 cacheline 对应的 tag 和物理地址 tag 域，以此判断是否命中 cache 。

因为使用物理地址做 tag ，所以不存在歧义问题。存在 别名 问题。

## 5.2 何时出现别名

因为 Linux 映射的最小单位是 页 ，大小是 4KB 。所以虚拟地址和物理地址的低 12 位是相等的，也就是说，经过转换后的地址低 12 位一样，那么就是 VI = PI。

假如是直接映射缓存， cache size 等于 4KB ，cache line size 等于 64 字节。可以算出来 index 是 6(6-11) 位， offset 是 6(0-5) 位，所以查找 cache 用到了 12 位数据，就能保证不相等，就不会有别名情况。


假如是直接映射缓存， cache size 等于 8KB ，cache line size 等于 256 字节。可以算出来 index 是 5(8-12) 位， offset 是 8(0-7) 位，13 大于 12 ，有一位无法保证不同，所以可能会出现别名（当 **index + offset > 12 bit** 时，也就是 **单个 way 的 cache size 小于等于 4KB** ）。

市面上很多产品都满足这个要求：
- intel 的sunny cove L1D： 48KB， 12 way-associative ，每块64Bytes
- AMD的ZEN3 L1D: 32KB, 8 way-associative ，每块64Bytes

# 0x06 总结

牢记 `别名` 和 `歧义` 的具体表现，就能明白 `PIPT` 、 `VIVT` 、 `VIPT` 为什么会有/没有这些问题了。

现在使用的方式是 PIPT 或者 VIPT。