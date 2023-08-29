# 0x00. 导读

GDB 基础命令介绍。

# 0x01. 简介

# 0x02. 命令

堆栈
```bash
info frame
frame [number]  # 显示当前栈帧，或者选择不同的栈帧
info locals     # 当前栈帧的局部变量
info args       # 列出对应函数调用的参数值
bt              # 打印堆栈信息

print 表达式    # 简记为 p ，其中“表达式”可以是任何当前正在被测试程序的有效表达式，比如当前正在调试C语言的程序，那么“表达式”可以是任何C语言的有效表达式，包括数字，变量甚至是函数调用。
print a         # 将显示整数 a 的值
print ++a       # 将把 a 中的值加1,并显示出来
print name      # 将显示字符串 name 的值
print gdb_test(22)      # 将以整数22作为参数调用 gdb_test() 函数
print gdb_test(a)       # 将以变量 a 作为参数调用 gdb_test() 函数

display 表达式      # 在单步运行时将非常有用，使用display命令设置一个表达式后，它将在每次单步进行指令后，紧接着输出被设置的表达式及值。如： display a

watch 表达式    # 设置一个监视点，一旦被监视的“表达式”的值改变，gdb将强行终止正在被调试的程序。如： watch a
```

线程
```bash
info threads    # 打印线程信息
thread 线程ID   # 切换线程ID
```

跳转
```bash
until+行号      # 运行至某行，不仅仅用来跳出循环
finish          # 运行程序，直到当前函数完成返回，并打印函数返回时的堆栈地址和返回值及参数值等信息。
call 函数(参数) # 调用程序中可见的函数，并传递“参数”，如：call gdb_test(55)
```

断点
```bash
b fn1 if a＞b   # 条件断点设置
```