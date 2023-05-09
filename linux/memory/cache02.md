# 0x00 导读

```bash
# 查看 CPU cache 信息
$ sudo dmidecode -t cache

dmidecode 3.1
Getting SMBIOS data from sysfs.
SMBIOS 3.1.1 present.

Handle 0x0069, DMI type 7, 27 bytes
Cache Information
        Socket Designation: L1-Cache
        Configuration: Enabled, Not Socketed, Level 1
        Operational Mode: Write Back
        Location: Internal
        Installed Size: 768 kB
        Maximum Size: 768 kB
        Supported SRAM Types:
                Synchronous
        Installed SRAM Type: Synchronous
        Speed: Unknown
        Error Correction Type: Single-bit ECC
        System Type: Unified
        Associativity: 8-way Set-associative

Handle 0x006A, DMI type 7, 27 bytes
Cache Information
        Socket Designation: L2-Cache
        Configuration: Enabled, Not Socketed, Level 2
        Operational Mode: Varies With Memory Address
        Location: Internal
        Installed Size: 12288 kB
        Maximum Size: 12288 kB
        Supported SRAM Types:
                Synchronous
        Installed SRAM Type: Synchronous
        Speed: Unknown
        Error Correction Type: Single-bit ECC
        System Type: Unified
        Associativity: 16-way Set-associative

Handle 0x006B, DMI type 7, 27 bytes
Cache Information
        Socket Designation: L3-Cache
        Configuration: Enabled, Not Socketed, Level 3
        Operational Mode: Varies With Memory Address
        Location: Internal
        Installed Size: 25344 kB
        Maximum Size: 25344 kB
        Supported SRAM Types:
                Synchronous
        Installed SRAM Type: Synchronous
        Speed: Unknown
        Error Correction Type: Single-bit ECC
        System Type: Unified
        Associativity: Fully Associative

Handle 0x006D, DMI type 7, 27 bytes
Cache Information
        Socket Designation: L1-Cache
        Configuration: Enabled, Not Socketed, Level 1
        Operational Mode: Write Back
        Location: Internal
        Installed Size: 768 kB
        Maximum Size: 768 kB
        Supported SRAM Types:
                Synchronous
        Installed SRAM Type: Synchronous
        Speed: Unknown
        Error Correction Type: Single-bit ECC
        System Type: Unified
        Associativity: 8-way Set-associative

Handle 0x006E, DMI type 7, 27 bytes
Cache Information
        Socket Designation: L2-Cache
        Configuration: Enabled, Not Socketed, Level 2
        Operational Mode: Varies With Memory Address
        Location: Internal
        Installed Size: 12288 kB
        Maximum Size: 12288 kB
        Supported SRAM Types:
                Synchronous
        Installed SRAM Type: Synchronous
        Speed: Unknown
        Error Correction Type: Single-bit ECC
        System Type: Unified
        Associativity: 16-way Set-associative

Handle 0x006F, DMI type 7, 27 bytes
Cache Information
        Socket Designation: L3-Cache
        Configuration: Enabled, Not Socketed, Level 3
        Operational Mode: Varies With Memory Address
        Location: Internal
        Installed Size: 25344 kB
        Maximum Size: 25344 kB
        Supported SRAM Types:
                Synchronous
        Installed SRAM Type: Synchronous
        Speed: Unknown
        Error Correction Type: Single-bit ECC
        System Type: Unified
        Associativity: Fully Associative
```

# 0x01 cache 策略

[Cache_(computing)](https://en.wikipedia.org/wiki/Cache_(computing))

## 1.1 Cache 更新策略(Cache update policy)

指当发生 `cache hit` 时，数据已经写入 cache 了，何时更新内存数据。 


- `write through` (写直通)
    - write is done synchronously both to the cache and to the backing store. 
- `write back` (写回)
    - initially, writing is done only to the cache. The write to the backing store is postponed until the modified content is about to be replaced by another cache block.  
        每个 cache line 中会有一个 bit 位记录数据是否被修改过，称之为 dirty bit。我们会将 dirty bit 置位。主存中的数据只会在 cache line 被替换或者显式的 clean 操作时更新。  
        因此，主存中的数据可能是未修改的数据，而修改的数据躺在 cache 中。 cache 和主存的数据可能不一致。

## 1.2 Cache 分配策略( Cache allocation policy )

指当发生 `cache miss` 时，是否需要将数据加载到 cache 。

- `no write allocate policy`( 也叫 `write around` )   
写指令直接更新主存数据。
- `write allocate policy`( 也叫 `fetch on write` )   
首先从主存中加载数据到 cache line 中（相当于先做个读分配动作，因为 cache line 中没有想要写的地址的，所以先加载，再写），然后会更新 cache line 中的数据。


## 1.3 wiki 流程图

一般是 `write-back + write allocate` ，也就是说，当 write miss 的时候用 write allocate ，当 write hit 的时候用 write-back 。   
`write-through + no-write allocate` 。也就是说，当 write miss 的时候用 no-write allocate ，当 write hit 的时候用 write-through 。   

下图源自 [wiki Cache_](https://en.wikipedia.org/wiki/Cache_(computing))

图1 `Write-back_with_write-allocation`

![write-back](../../pic/linux/memory/Write-back_with_write-allocation.png)

图2 `Write-through_with_no-write-allocation`

![write-through](../../pic/linux/memory/Write-through_with_no-write-allocation.png)


## 1.4 例子

有一个 64 Bytes 大小直接映射缓存， cache line 大小是 8 Bytes ，采用 `write-back + write allocate`。

当 CPU 从地址 0x2a 读取一个字节， cache 中的数据将会如何变化呢？假设当前 cache 状态如下图所示( tag 旁边 valid 一栏的数字 1 代表合法。 0 代表非法。后面 Dirty 的 1 代表dirty， 0 代表没有写过数据，即非 dirty )。

![9](../../pic/cache9.png)

根据 index 找到对应的 cache line ，对应的 tag 部分 valid bit 是合法的，但是 tag 的值不相等，因此 hit miss。

此时我们需要从地址 0x28 （因为 cache 和 memory 之间通信的最小单位是 cache line 。 0x2a 二进制是 101010 ， index 是 101 ，所以从主存中取出地址为 101000~101111 的 8 字节写入 cache line ）加载 8 字节数据到该 cache line 中。

但是，我们发现当前 cache line 的 dirty bit 置位。因此， cache line 里面的数据不能被简单的丢弃，由于采用 `write-back` 机制，所以我们需要将 cache 中的数据 0x11223344 写到地址 0x0128(0001 0010 1000) 地址（旧数据 tag 是 0x4 ，也就是 100 ， index 是 101 ，加上最后三位 offset 应该是 000 ，因为要将全部 8 字节数据都写回主存，所以旧数据的地址是 0001 0010 1000 ）。

![10](../../pic/cache10.png)

当写回操作完成，我们将主存中 0x28 地址开始的 8 个字节加载到该 cache line 中( 更新 entry )，并清除 dirty bit 。然后按照 cache hit  流程将找到 0x52 返回给 CPU 。
