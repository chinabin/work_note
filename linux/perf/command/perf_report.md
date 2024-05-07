# 0x00 导读

用于总结 perf record 输出文件 perf.data 中的信息。可以通过 TUI （文本用户界面）或 STDIO （标准输入输出）方式显示。 TUI 提供了一个交互式操作界面，而 STDIO 将结果显示在终端上，也可以将结果重定向到文件中。

# 0x01. 简介

# 0x02. 命令

perf report [options]

以下是 perf report 的一些常用参数：  
-i 或 --input：指定要读取的输入文件，默认为 perf.data。  
-F 或 --fields：指定要显示的字段，例如：comm, dso, symbol 等。  
--sort：指定排序顺序，例如：dso,symbol 或 symbol,dso。  
--show-total-period：显示每个符号的总周期数。  
-T 或 --threads：显示线程相关数据。  
-m 或 --modules：显示模块（共享库）相关数据。  
-k 或 --vmlinux：指定内核符号表文件（vmlinux）的路径。  
-f 或 --force：强制解析文件，即使它看起来无效或损坏。  
-c 或 --comms：指定要显示的命令（进程）列表。  
--dsos：指定要显示的动态共享对象（DSO）列表。  
-s 或 --symbols：指定要显示的符号（函数）列表。  
--percent-limit：仅显示超过指定百分比的项。  
-P 或 --pretty：指定输出格式，如raw、normal等。  
--stdio：以文本模式显示报告（而非 TUI 模式）。  
--tui：以 TUI 模式显示报告（**默认方式**）。  
--gtk：以 GTK 模式显示报告。  
-g：指定 call graph 的参数，通常会指定三个（逗号分割）：  

    1. 第一个参数是各个节点百分比的计算方法。
        如果指定 graph 则会直接显示该节点占总体样本数的比例。
        如果指定 fractal 会显示一个节点和其他平级节点相比所占的比例。  
    2. 第二个参数是绘制 Call Graph 的门槛值。
        如果一个 Stack Trace 的数量与总体样本数相比低于门槛值，则忽略该 Stack Trace。此数值以百分比表示。如果指定的数值为 0.5，则占比低于 0.5% 的 Stack Trace 会被忽略。  
    3. 第三个参数是 Stack Trace 的走访顺序。
        如果指定 caller 就会显示 Caller-based Call Graph。反之，如果指定 callee 则会显示 Callee-based Call Graph。  

--children: 被调用者的样本数量会加到调用者的统计数字，默认选项。
--no-children：仅显示独立样本，不显示调用子函数的样本。  
--no-demangle：禁用 C++ 符号解析。  
--demangle：指定 C++ 符号解析方式，如：no, normal, smart 等。  
--filter：指定过滤器，如：--filter 'dso(/lib*)'。  
--max-stack：指定栈帧的最大数量。  

-g 选项的预设值会受 --[no-]children 选项影响：
```
--children      graph,0.5,caller
--no-children	graph,0.5,callee
```

## 2.1 

```
a: 汇编，左侧数字为 percent
t: a 的输出结果的左侧，切换 percent period samples 三种输出
```