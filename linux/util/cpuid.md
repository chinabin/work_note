# 0x00. 导读

# 0x01. 简介

执行此命令来获取CPU的信息，包含厂商、型号和支持的功能特性。

CPUID 指令的操作码是 0F A2。CPUID没有显式的操作数，其隐式使用EAX或EAX与ECX中的值作为参数，以确定要获取的信息。在英特尔指令集手册中，CPUID的输入参数（EAX），被称为“叶”（Leaf），AMD称其为“功能码”（Function Number），执行CPUID后，返回值将被写入EAX，EBX，ECX和EDX寄存器内。此外，在使用一些特殊的输入参数时，必须同时于ECX内写入第二个输入参数，否则无法得到有意义的返回值。第二个参数也被称为“子叶”（Sub Leaf）或“子功能码”（Sub Function）。

由于可供获取的信息很多，有点像一本供查询的书。因此，在调用此命令时，必须指定一个参数来表明需要获取的是什么类型的信息。这个参数被称为 `Leaf` ，可以理解成页的类型码。这个Leaf值被要求存放在EAX寄存器中。以下是不同Leaf的内容：

- Leaf=0: 得到最大的Function Parameter，即允许的Leaf值，以及生产厂商ID字符串。
- Leaf=1: 得到处理器信息(Processor Info)和特性比特(Feature Bits)。这其中包含处理器类型、家族Family和型号Model
- Leaf=2: 得到CPU多级缓存大小信息(Cache and TLB Descriptor information)
- Leaf=3: 得到处理器的序列码
- Leaf=4和Bh: 得到CPU的核心、线程和Cache的拓补信息
- Leaf=6：得到温度和功耗管理的特性能力，比如是否支持数字温度传感器
- Leaf=7: ECX此时也是参数，可以取0，1，2值。这个ECX参数被称为SubLeaf（子页）。这个Leaf得到一些扩展特性(Extended Feature)，比如是否支持解决分支预测幽灵漏洞的特性(IBRS_IBPB / spec_ctrl)
- Leaf=Dh：XSAVE features and state-components，用于存放是否支持在上下文切换时保存扩展信息，比如AVX矢量状态YMM0至YMM15。
- Leaf=12h，且ECX=0: SGX Leaf Functions
- Leaf=14h, 且ECX=0: 处理器跟踪调试特性。
- Leaf=19h：锁AES的密钥的特性( AES Key Locker features)
- Leaf=24h, ECX=0: AVX10 Features
- Leaf=800000xxh: 这些Leaf应该是AMD厂商定义的，内容基本和前面Leaf类似，比如80000000h这个值与Leaf=0是一致的，只是只返回了最大允许的Leaf，而没有生产厂商ID字符串。

[wiki: CPUID](https://en.wikipedia.org/wiki/CPUID)

# 0x02. 

显示所有 CPU 的信息:
```
$ cpuid
```

仅显示当前 CPU 的信息:
```
$ cpuid -1  # 数字 1
```

显示原始十六进制信息，不进行解码:
```
$ cpuid -r
```

## 0x03. 超纲

`cpuid -1 -l 5 -s 5`  

属于扩展指令

[MWAIT — Monitor Wait](https://www.felixcloutier.com/x86/mwait)