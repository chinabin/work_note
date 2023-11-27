# 0x00. 导读

# 0x01. 简介

[explainshell](https://explainshell.com/)

# 0x02. 重定向

前提：

1. 0 是标准输入，1 是标准输出，2 是标准错误输出
2. `command > output` 是 `command 1> output` 的缩写
3. 使用 `&[FILE_DESCRIPTOR]` 来引用文件描述符的值

所以，使用 `2>&1` 来重定向 stderr 的输出至 stdout 的地方。

# 0x03. 终端与伪终端

[Linux 伪终端(pty)](https://www.cnblogs.com/sparkdev/p/11605804.html)
[Linux 终端(tty)](https://www.cnblogs.com/sparkdev/p/11460821.html)