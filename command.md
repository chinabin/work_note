## **grep**   
`grep [option] pattern file`   
可以使用 --exclude 指定忽略不搜索特定的文件名，或者使用 --exclude 指定只搜索特定的文件名。 --exclude-dir 选项来排除一些目录。   
```
grep -no errorID:[^0].* -r . --include=QMT_2022-01-25_13-00.txt --exclude-dir=fut
```
`-A<行数 x>`：除了显示符合范本样式的那一列之外，并显示该行之后的 x 行内容。   
`-B<行数 x>`：除了显示符合样式的那一行之外，并显示该行之前的 x 行内容。   
`-C<行数 x>`：除了显示符合样式的那一行之外，并显示该行之前后的 x 行内容。   
```
grep -C3 "aaa" file_* test_file_*
```
`-c`：统计匹配的行数   
```
grep -c "order*" demo.txt
```
`-o`：仅显示匹配到的字符串，而不是完整的整行内容。   
`-s`：不显示不存在或者没有匹配文本的错误信息  
```
grep lalala file1 file_1
如果 file1 文件不存在就会出现一条 grep: file1: No such file or directory ，加上 -s 就不会有这个信息了。
```
`-w`: 匹配整个单词，元字符不再被解释为特殊含义。   
`-i`: 忽略字符大小写的差别   
`-n --line-number`: 显示行号   
`-r -R --recursive`: 递归搜索   
```
grep "__main__" -r ./
```
`-v`: 反转查找   
`-h`: 查询多文件时不显示文件名   
`-l --file-with-matches`: 只列出符合匹配的文件名，不列出具体的匹配行   
`-L --files-without-match`: 列出不符合匹配的文件名   
`-x`: 匹配整行，即只有当文件中有整行内容与模式匹配时，grep命令才输出该行结果   
   
    
```
^    # 锚定行的开始 如：'^grep'匹配所有以grep开头的行。    
$    # 锚定行的结束 如：'grep$' 匹配所有以grep结尾的行。
.    # 匹配一个非换行符的字符 如：'gr.p'匹配gr后接一个任意字符，然后是p。    
*    # 匹配零个或多个先前字符 如：'*grep'匹配所有一个或多个空格后紧跟grep的行。    
.*   # 一起用代表任意字符。   
[]   # 匹配一个指定范围内的字符，如'[Gg]rep'匹配Grep和grep。    
[^]  # 匹配一个不在指定范围内的字符，如：'[^A-FH-Z]rep'匹配不包含A-R和T-Z的一个字母开头，紧跟rep的行。    
\(..\)  # 标记匹配字符，如'\(love\)'，love被标记为1。    
\<      # 锚定单词的开始，如:'\<grep'匹配包含以grep开头的单词的行。    
\>      # 锚定单词的结束，如'grep\>'匹配包含以grep结尾的单词的行。    
x\{m\}  # 重复字符x，m次，如：'0\{5\}'匹配包含5个o的行。    
x\{m,\}   # 重复字符x,至少m次，如：'o\{5,\}'匹配至少有5个o的行。    
x\{m,n\}  # 重复字符x，至少m次，不多于n次，如：'o\{5,10\}'匹配5--10个o的行。   
\w    # 匹配文字和数字字符，也就是[A-Za-z0-9]，如：'G\w*p'匹配以G后跟零个或多个文字或数字字符，然后是p。   
\W    # \w的反置形式，匹配一个或多个非单词字符，如点号句号等。   
\b    # 单词锁定符，如: '\bgrep\b'只匹配grep。  

```


--------
## **echo**   
```
> echo {0..9}
0 1 2 3 4 5 6 7 8 9
```
-e 解释转义字符
```
> echo "a\tb\tc"
a\tb\tc
> echo -e "a\tb\tc"
a       b       c
```

--------   
    
## **xargs**   
Unix 命令都带有参数，有些命令可以接受"标准输入"（stdin）作为参数。例如   
`ps -ef | grep Bif`    
管道命令将左侧命令（ps -ef）的标准输出转换为标准输入，提供给右侧命令（grep Bif）作为参数。   
但是，大多数命令都不接受标准输入作为参数，只能直接在命令行输入参数，这导致无法用管道命令传递参数。例如   
`echo "hello" | echo`   
会失败，因为 echo 不接受管道传参。   
xargs命令的作用，是将标准输入转为命令行参数。   
`echo "hello" | xargs echo`   
xargs 命令格式是 `xargs [-options] [command]` ，真正执行的命令，紧跟在xargs后面，接受xargs传来的参数。   
xargs后面的命令默认是echo。   
```
> xargs
# 等同于
>xargs echo
```
输入 xargs 按下回车以后，命令行就会等待用户输入，作为标准输入。你可以输入任意内容，然后按下 Ctrl d ，表示输入结束，这时 echo 命令就会把前面的输入打印出来。
```
> xargs
hello (Ctrl + d)
hello
```
默认情况下，xargs 将换行符和空格作为分隔符，把标准输入分解成一个个命令行参数。`-d` 参数可以更改分隔符。
```
> echo -e "a\tb\tc" | xargs -d "\t" echo
a b c
```
`-p` 参数打印出要执行的命令，询问用户是否要执行。输入 y 才会真的执行。   
```
> echo 'one two three' | xargs -p touch
touch one two three ?...
```
`-t` 参数则是打印出最终要执行的命令，然后直接执行，不需要用户确认。   
如果标准输入包含多行，`-L`参数指定多少行作为一个命令行参数。
```
> xargs find -name
"*.txt"   
"*.md"
find: paths must precede expression: `*.md'
> xargs -L 1 find -name
"*.txt"
./foo.txt
./hello.txt
"*.md"
./README.md
```
`-L` 参数虽然解决了多行的问题，但是有时用户会在同一行输入多项。`-n` 参数指定每次将多少项，作为命令行参数。
```
> echo {0..9} | xargs -n 2 echo
0 1
2 3
4 5
6 7
8 9
```
如果 xargs 要将命令行参数传给多个命令，可以使用 `-I` 参数。`-I` 指定每一项命令行参数的替代字符串。
```
> cat foo.txt
one
two
three
> cat foo.txt | xargs -I file sh -c 'echo file; mkdir file'
one 
two
three
```
使用 `-I file` 表示 `file` 是命令行参数的替代字符串。

--------   
    
## **find**   
`find . -print`    
使用正则表达式的时候注意单引号的添加。     
根据名字查找文件
```
find . -name '*.o' -print
find . -name '[a-zA-Z]*.o' -print
```
根据文件类型查找文件
```
find . -type f -print | xargs ls -l
f 普通文件
d 目录
l 软链接
```
根据时间查找文件
```
mtime: 上一次修改时间，写入
atime: 上一次访问时间，读取，执行
ctime: 上一次 改变 时间，写入，修改所有者，修改权限，设置软链接
find . -mtime 7 -print
```

--------   
    
## **ssh**  
1. SSH（Secure Shell 的缩写）是一种网络协议，用于加密两台计算机之间的通信，并且支持各种身份验证机制。OpenSSH 是 ssh 协议的开源实现。SSH 的软件架构是服务器-客户端模式（Server - Client）。在这个架构中，SSH 软件分成两个部分：向服务器发出请求的部分，称为客户端（client），OpenSSH 的实现为 ssh；接收客户端发出的请求的部分，称为服务器（server），OpenSSH 的实现为 sshd。另外，OpenSSH 还提供一些辅助工具软件（比如 ssh-keygen 、ssh-agent）和专门的客户端工具（比如 scp 和 sftp）。

2. 
    ```
    -N 表示只连接远程主机，不打开远程 shell 执行命令，因为默认是连接上去要执行命令的。不加会报错，Cannot fork into background without a command to execute.
    -T 表示不为这个连接分配TTY。
    -f 参数表示 SSH 连接成功后，转后台运行。
    -p 参数指定 SSH 客户端连接的服务器端口。
    ```

3. SSH 客户端的全局配置文件是 `/etc/ssh/ssh_config`，用户个人的配置文件在 `~/.ssh/config`，优先级高于全局配置文件。

4. ssh 服务端，sshd 的配置文件在 `/etc/ssh` 目录，主配置文件是 `sshd_config`。[sshd config 解释](https://wangdoc.com/ssh/server.html)。
    ```
    # 启动
    $ sudo systemctl start sshd.service

    # 停止
    $ sudo systemctl stop sshd.service

    # 重启
    $ sudo systemctl restart sshd.service
    ```

5. **动态转发**
    ```
    在 A 机器上执行

    ssh -D local-port user@host -N

    SSH会建立一个socket，去监听 local-port 端口。一旦有数据传向那个端口，就自动把它转移到 SSH 连接上面，发往远程主机。可以想象，如果 local-port 原来是一个不加密端口，现在将变成一个加密端口。
    ```
    注意，这种转发采用了 SOCKS5 协议。访问外部网站时，需要把 HTTP 请求转成 SOCKS5 协议，才能把本地端口的请求转发出去。举个例子:
    ```
    > ssh -D 2121 tab@123.123.123.123 -N
    > curl -x socks5://localhost:2121 http://www.example.com
    curl 的 -x 参数指定代理服务器，即通过 SOCKS5 协议的本地 2121 端口，访问http://www.example.com。
    ```

6. **本地转发**
    本地转发（local forwarding）指的是，SSH 服务器作为中介的跳板机，建立本地计算机与特定目标之间的加密连接。本地转发指定一个本地端口（local-port），所有发向那个端口的请求，都会转发到 SSH 跳板机（tunnel-host），然后 SSH 跳板机作为中介，将收到的请求发到目标服务器（target-host）的目标端口（target-port）。
    ```
    > ssh -L local-port:target-host:target-port tunnel-host
    -L 参数表示本地转发，local-port 是本地端口，target-host 是你想要访问的目标服务器，target-port 是目标服务器的端口，tunnel-host 是 SSH 跳板机。
    ```
    举个例子，你的机器是 A ，你能够访问 B ， B 能够访问 C ，但 A 不能访问 C 。于是在 A 的机器上执行命令: `ssh -L 1234:ip-C:2222 user@ip-B` 就搞定，每次你访问本地的 1234 端口就相当于你访问了 C 的 2222 端口。   
    把我本地的请求，转发出去。白话：访问本机器（执行命令的机器）的 local-port 端口，就等同于，通过 ssh 去到 tunnel-host 机器上之后，访问 target-host 机器的 target-port 端口。

7. **远程转发**
    ```
    > ssh -R remote-port:target-host:target-port remotehost
    -R 参数表示远程端口转发，remote-port 是远程计算机的端口，target-host 和 target-port 是目标服务器及其端口，remotehost 是远程计算机。
    ```
    举个例子，你的机器是 A ，还有两台机器 B 和 C ，B 可以访问 A ，B 也可以访问 C 。但是 A 不能访问 B 也不能访问 C ，A 想访问 C 。在机器 **B** 上执行: `ssh -R 1234:ip-C:8000 user@ip-A` 搞定，每次有请求到达 A 的 1234 端口，就相当于请求到达 C 的 8000 端口。   
    把远程的请求，转发进来。白话：访问 A 机器的 1234 端口，就等同于，那个请求通过 ssh 去到 B 机器，然后通过 ssh 去到 C 机器，访问 C 机器的 8000 端口。注意，这个命令是在 B 机器上执行的。

--------   
    
## **free**   
默认等于 `free -k`   
`man free`
```
              total        used        free      shared  buff/cache   available
Mem:       16266140     1976840     9208308       32636     5080992    13918424
Swap:       2113532      462304     1651228
```
第一行是内存情况：   
total: 表示所有内存   
used: 表示已使用   
free: 表示没有使用   
~~shared:~~   
buff/cache: 表示缓存部分，例如高速缓冲区部分   
available: 是从另外的角度来看空闲内存，因为 buff/cache 虽然是缓存了数据的内存，但是当系统缺内存的时候，它们是可以拿来牺牲的，所以理论上来说 available = free + buff/cache      

第二行是交换空间：   
swap space 是磁盘上的一块区域，可以是一个分区，也可以是一个文件。所以具体的实现可以是 swap 分区也可以是 swap 文件。当系统物理内存吃紧时，Linux 会将内存中不常访问的数据保存到 swap 上，这样系统就有更多的物理内存为各个进程服务，而当系统需要访问 swap 上存储的内容时，再将 swap 上的数据加载到内存中，这就是常说的换出和换入。交换空间可以在一定程度上缓解内存不足的情况，但是它需要读写磁盘数据，所以性能不是很高。