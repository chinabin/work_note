# 0x00. 导读

# 0x01. 简介

用于 强制将 改变的内容 立刻 写入磁盘，并更新块信息。

在 Linux/Unix 系统中，在文件或数据处理过程中一般先放到内存缓冲区中，等到适当的时候再写入磁盘，以提高系统的运行效率。  
sync 命令则可用来强制将内存缓冲区中的数据立即写入磁盘中。  
用户通常不需执行 sync 命令，系统会自动执行 update 或 bdflush 操作，将缓冲区的数据写 入磁盘。只有在 update 或 bdflush 无法执行或用户需要非正常关机时，才需手动执行 sync 命令。

> sync is automatically called before unmounting a volume, rebooting, or shutting down a system.

# 0x02. 命令

可以直接不加参数运行。

```
-d, --data             只同步文件数据，不同步不必要的元数据
-f, --file-system      同步包含这些文件的文件系统
```