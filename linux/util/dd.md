# 0x00. 导读

# 0x01. 简介

dd 可从标准输入或文件中读取数据，根据指定的格式来转换数据，再输出到文件、设备或标准输出。

# 0x02. 命令

```bash
参数说明:

if=file
    从 file 中读而不是标准输入。 
of=file
    写到 file 里去而不是标准输出。除非指定 conv=notrunc ，否则， dd 将把 file 截为O字节（或由 seek= 选项指定的大小） 
ibs=bytes
    一次读 bytes 字节。默认是512。 
obs=bytes
    一次写 bytes 字节。默认是512。 
bs=bytes
    一次读和写 bytes 字节。这将覆盖 ibs 和 obs 设定的值（并且，设定 bs 不等于同时将 ibs 和 obs 设为同一个值，至少在 只使用 sync, noerror 或 notrunc 转换时是这样的。因为bs规定，每个输入块都应作为单独 的数据块拷贝到输出，而不把较短的块组合到一起）。 
cbs=bytes
    为 block 转换和 unblock 转换指定转换块的大小。 
skip=blocks
    在拷贝之前，跳过输入文件的前 blocks 块，每块大小为 ibs-byte 字节。 
seek=blocks
    在拷贝之前，跳过输出文件的前 blocks 块，每块大小为 obs-byte 字节。 
count=blocks
    只拷贝输入文件的前 blocks 块（每块的大小为 ibs-byte 字节），而不是全部内容，直到文件末尾。 
```