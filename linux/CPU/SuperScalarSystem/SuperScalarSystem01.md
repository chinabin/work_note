# 0x00. 导读

# 0x01. 简介

一个时钟周期能够执行多于一条指令的，就是超标量处理器，一个周期能执行 N 条指令的就称为 N 路超标量处理器。

# 0x02. 概述

在超标量处理器中，一条指令的执行从前到后大致可分为以下这些阶段：取指令（Fetch），解码（Decode），寄存器重命名（Register Renaming），分发（Dispatch），发射（Issue），读取寄存器（Register File Read），执行（Execute），写回（Write Back），提交（Commit）。

- 取指令（Fetch）：这个部分负责从 I-Cache（指令缓存）中取指令，这个阶段通常会尝试同时提取多条指令，以提高吞吐量。主要由两大部件组成：I-Cache 和分支预测器。I-Cahe 负责存储最近常用的指令，分支预测器决定下一条指令的 PC 值。

- 解码（Decode）：解码阶段将提取的指令解析为其操作类型和操作数，确定指令需要的资源和下一步的操作。对于 RISC 指令集来说，解码部分相对简单，而对于 CISC 的指令集，例如 x86，解码部分则相对复杂。

- 寄存器重命名（Register Renaming）：逻辑寄存器会被映射到物理寄存器，以消除伪依赖（如WAR和WAW），确保后续指令可以安全并行执行。

- 分发（Dispatch）：在这个阶段，寄存器被重命名之后的指令，会按照程序中规定的顺序写到发射队列（Issue Queue），重排序缓冲（ROB，re-order buffer）和 Store Buffer 等部件中，准备发射。

- 发射（Issue）：此时指令已在发射队列中，仲裁（Select）电路会从这个部件中挑选出合适的指令，送到执行单元中去执行。发射队列中还有唤醒（Wake Up）电路，它可以将发射队列中对应的源操作数置为有效状态（源操作数已准备好被读取），仲裁电路和唤醒电路相互配合，是超标量处理中的关键路径。这个阶段是指令从顺序执行（In order）到乱序执行（Out of order）的临界点，指令在这个阶段之后就按照乱序的方式执行，直到流水线的提交（Commit）阶段，才会重新变为顺序执行的状态。

- 读取寄存器（Register File Read）：被选中执行的指令需要从物理寄存器堆（PRF，Physical Register File）中读取操作数。除了从 PRF 中得到指令的源操作数，还可以从旁路网络中得到操作数（为了解决 RAW 数据相关），事实上很大一部分指令都是通过旁路网络获得操作数的。由于多端口寄存器堆的访问速度一般都不会很快，所以在现实的处理器中，这个阶段都会单独使用一个流水段。

- 执行（Execute）：指令得到了他所需要的操作数之后，就送到对应的执行单元（FU）中去执行。在超标量处理器中，执行阶段通常有很多个不同类型的执行单元。例如负责普通运算的单元，负责分支指令运算的单元，负责 load 和 store 内存访问指令的单元，多媒体指令执行的单元，以及 SIMD 指令单元等。

- 写回（Write Back）：这个阶段会将运算单元计算的结果写回到物理寄存器堆（PRF）中。这个阶段还有一个重要的功能，就是通过旁路网络将计算结果送到需要的地方，一般都是送到执行单位的输入端。

- 提交（Commit）：这个阶段起主要作用的部件是重排序缓冲（ROB，re-order buffer），它会将乱序执行的指令变回到程序中规定的顺序。因为在指令在流水线的分发阶段，会按照程序中规定的顺序写到 ROB 中，所以当指令在提交时（从物理寄存器写到逻辑寄存器中）， ROB 会根据分发阶段所记录下来指令顺序来提交，这样就保存了指令顺序是正确的。在指令提交之前，所有指令的结果都是可以撤销的，只有当指令提交后，也就是说一条指令一旦从 ROB 中离开（称之为退休，retire），才会对机器状态产生真正的影响。这个阶段也会对指令产生的异常（Exception）进行处理。指令在流水线的很多阶段都可能发生异常，但是所有的异常都必须等到指令到达流水线的提交阶段才能进行处理，这样可以保证异常的处理顺序按照程序中规定的顺序进行，并且是能够实现精确异常。