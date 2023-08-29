# 0x00. 导读

编译器将人类可读的源代码转换为由计算机直接执行的一系列指令。一个常见的问题是 调试器和错误处理程序如何报告源代码中处理器当前所在的位置？并且随着编译器优化代码，将指令映射回源代码也存在一些复杂性。

调试信息是由编译器在编译机器代码时一起生成的，**它是可执行文件和源代码之间关系的一种描述。调试信息根据预定义的格式被编码到机器代码中。**

在过去的年代，对应于各种架构，有很多格式被发明了出来。最经典和广泛使用的格式（stabs，COFF，PE-COFF，OMF，IEEE-695）就是 `DWARF` ，在 Linux 平台和类 Unix(Unix-y) 平台上，`DWARF` 被用来描述 `ELF` 格式可执行文件的调试信息，可以说，它无处不在。

# 0x01. 简介

DWARF 很复杂，它建立于对其他格式的多年研究经验之上,这些格式可以运用于各种架构。

DWARF 必然是复杂的，因为它需要解决一个很难办的问题--向调试器展示任何高级语言代码的调试信息,为各种架构和 ABIs(application binary interface) 提供支持。

# 0x02. 查看 dwarf

查看的前提是 程序编译的时候需要指定 `-gdwarf-4` 。

Q: DWARF 信息在 ELF 文件内部何处？  

.debug_ 开头的 section 就是 DWARF 调试 sections:  

```bash
$ objdump -h a.out      # or readelf -S a.out
...
 25 .debug_aranges 00000030  0000000000000000  0000000000000000  000030bd  2**0
                  CONTENTS, READONLY, DEBUGGING
 26 .debug_info   00002b98  0000000000000000  0000000000000000  000030ed  2**0
                  CONTENTS, READONLY, DEBUGGING
 27 .debug_abbrev 00000665  0000000000000000  0000000000000000  00005c85  2**0
                  CONTENTS, READONLY, DEBUGGING
 28 .debug_line   000003cb  0000000000000000  0000000000000000  000062ea  2**0
                  CONTENTS, READONLY, DEBUGGING
 29 .debug_str    00001962  0000000000000000  0000000000000000  000066b5  2**0
                  CONTENTS, READONLY, DEBUGGING
```

如果想查看各个调试 section 的内容，可以用 `readelf -w* a.out` ，* 是调试 section 的第一个字母，例如 -wi 是查看 .debug_info 的内容，-wl 是查看 .debug_line 的内容。

# 0x04.总结

调试信息理论上是一个很简单的概念。它的实现细节可能错综复杂，重要的是现在我们知道了调试器如何找出可执行文件中的相关信息。有了这些调试信息，调试器架起了一道用户和可执行文件之间的桥梁，用户从源代码和数据结构的角度思考，而可执行文件是一堆机器指令加上内存或寄存器中的数据。