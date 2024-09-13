# 0x00. 导读

cache 策略: Cache update policy and Cache allocation policy

# 0x01. 简介

# 0x02. cache 策略

[Cache wiki](https://en.wikipedia.org/wiki/Cache_(computing))

## 2.1 Cache 更新策略( Cache update policy )

当执行一条 store 指令时，例如写入 D-Cache 了，这时候会造成 D-Cache 和下级存储器对于同一个地址有着不同的数据，这就是 不一致（non-consistent）。

- `write through` (写直通)  
    数据写到 Cache 的时候也写到下级存储器。
    下级存储器的访问时间相对是较长的，每次执行 store 指令都向这样的慢速存储器写入数据，处理器的执行效率肯定不会很高了。
- `write back` (写回)  
    数据只写到 Cache 。下级存储器的更新时间推迟，例如直到这个 Cache 要被替换新的内容进来的时候。
    这样做能获得比较好的性能，但是会造成 Cache, 例如 D-Cache 和下级存储器中有很多地址中的数据不一致，给存储器的一致性管理带来一定的负担。

当然，这是假设在执行 store 指令的时候，要写入的地址总是在 Cache 中存在，而实际上，有可能地址是不在 Cache 中的，这就发生了 写缺失（write miss）。这个时候该如何做？这就涉及 Cache 分配策略了。

## 2.2 Cache 分配策略( Cache allocation policy )

指当发生 `cache miss` 时，是否需要将数据加载到 cache 。

- `no write allocate policy`( `write around` )   
    写指令直接更新主存数据。
- `write allocate policy`( `fetch on write` )   
    首先从主存中加载数据到 cache line 中（相当于先做个读分配动作，因为 cache line 中没有想要写的地址的，所以先加载，再写），然后会更新 cache line 中的数据。


## 2.3 wiki 流程图

一般是 `write-back + write allocate` ，也就是说，当 write miss 的时候用 write allocate ，当 write hit 的时候用 write-back 。   
`write-through + no-write allocate` 。也就是说，当 write miss 的时候用 no-write allocate ，当 write hit 的时候用 write-through 。   

下图源自 [wiki Cache_](https://en.wikipedia.org/wiki/Cache_(computing))

图1 `Write-back_with_write-allocation`

![write-back](../../pic/linux/memory/Write-back_with_write-allocation.png)

图2 `Write-through_with_no-write-allocation`

![write-through](../../pic/linux/memory/Write-through_with_no-write-allocation.png)


## 2.4 例子

有一个 64 Bytes 大小直接映射缓存， cache line 大小是 8 Bytes ，采用 `write-back + write allocate`。

当 CPU 从地址 0x2a 读取一个字节， cache 中的数据将会如何变化呢？假设当前 cache 状态如下图所示( tag 旁边 valid 一栏的数字 1 代表合法。 0 代表非法。后面 Dirty 的 1 代表dirty， 0 代表没有写过数据，即非 dirty )。

![9](../../pic/cache9.png)

根据 index 找到对应的 cache line ，对应的 tag 部分 valid bit 是合法的，但是 tag 的值不相等，因此 hit miss。

此时我们需要从地址 0x28 （因为 cache 和 memory 之间通信的最小单位是 cache line 。 0x2a 二进制是 101010 ， index 是 101 ，所以从主存中取出地址为 101000~101111 的 8 字节写入 cache line ）加载 8 字节数据到该 cache line 中。

但是，我们发现当前 cache line 的 dirty bit 置位。因此， cache line 里面的数据不能被简单的丢弃，由于采用 `write-back` 机制，所以我们需要将 cache 中的数据 0x11223344 写到地址 0x0128(0001 0010 1000) 地址（旧数据 tag 是 0x4 ，也就是 100 ， index 是 101 ，加上最后三位 offset 应该是 000 ，因为要将全部 8 字节数据都写回主存，所以旧数据的地址是 0001 0010 1000 ）。

![10](../../pic/cache10.png)

当写回操作完成，我们将主存中 0x28 地址开始的 8 个字节加载到该 cache line 中( 更新 entry )，并清除 dirty bit 。然后按照 cache hit  流程将找到 0x52 返回给 CPU 。

# 0x03. Cache 的替换策略

简单来说，不管是读取还是写入 Cache 时发生了缺失，都需要从对应的 Cache 中找出一个 line 来存放从下级存储器中读出的数据，如果此时 Cache 中所有的 line 都被占用了呢，那么就需要替换掉一个，如何从这些有效的 Cache line 中找到一个并替换，这就是 Cache replacement 策略。

常用的有 近期最少使用法（LRU）、随机替换（Random Replacement）等。