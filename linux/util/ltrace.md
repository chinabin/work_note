# 0x00. 导读

# 0x01. 简介

用来跟踪进程调用库函数的情况。

# 0x02. 命令

-c  
计算时间和调用，并在程序退出时打印摘要。  

-C  
Decode low-level symbol names into user-level names. Besides removing any initial underscore prepended by the system, this makes C++ function names readable.  
    用一个 C++ 程序测试就知道了

-e exp  
改变跟踪的事件。  
例如 -e printf 表示仅跟踪 printf 库调用。相比之下，-e !printf 表示跟踪除 printf 之外的每个库调用。

-i  
Print the instruction pointer at the time of the library call.

-l filename  
只打印某个库中的调用。

-L  
不打印库调用。

-n, --indent=NR  
对每个调用级别嵌套以 NR 个空格进行缩进输出。

-o, --output=file  
把输出定向到文件。

-p PID  
附着在值为 PID 的进程号上进行 ltrace 。

-s  
STRLEN 设置打印的字符串最大长度。

-S  
显示系统调用。

-r  
打印相对时间戳。

-t, -tt, -ttt  
打印绝对时间戳。

-T  
输出每个调用过程的时间开销。