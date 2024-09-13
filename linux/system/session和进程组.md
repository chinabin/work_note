# 0x00. 导读

# 0x01. 简介

[进程ID及进程间的关系](https://mazhen.tech/p/%E8%BF%9B%E7%A8%8Bid%E5%8F%8A%E8%BF%9B%E7%A8%8B%E9%97%B4%E7%9A%84%E5%85%B3%E7%B3%BB/)

# 0x02. session

session 就是一组进程的集合，session id 就是这个 session 中 leader 的进程 ID。

## 2.1 session的特点

session 的主要特点是当 session 的 leader 退出后， session 中的所有其它进程将会收到 `SIGHUP` 信号，其默认行为是终止进程，即 session 的 leader 退出后， session 中的其它进程也会退出。

如果 session 和 tty 关联的话，它们之间只能一一对应，一个 tty 只能属于一个 session ，一个 session 只能打开一个 tty 。当然 session 也可以不和任何 tty 关联。

## 2.2 session 的创建

session 可以在任何时候创建， session 中的第一个进程即为这个 session 的 leader ， leader 是不能变的。常见的创建 session 的场景是：

- 用户登录后，启动 shell 时将会创建新的 session ，shell 会作为 session 的 leader ，随后 shell 里面运行的进程都将属于这个 session ，当 shell 退出后，所有该用户运行的进程将退出。  
这类 session 一般都会和一个特定的 tty 关联， session 的 leader 会成为 tty 的控制进程，当 session 的 **前端进程组** 发生变化时，控制进程负责更新 tty 上关联的前端进程组，当 tty 要关闭的时候，控制进程所在 session 的所有进程都会收到 SIGHUP 信号。

- 启动 deamon 进程，这类进程需要和父进程划清界限，所以需要启动一个新的 session 。这类 session 一般不会和任何 tty 关联。

# 0x03. 进程组

进程组（process group）也是一组进程的集合，进程组 id 就是这个进程组中 leader 的进程 ID。

## 3.1 进程组的特点

进程组的主要特点是可以以进程组为单位通过函数 killpg 发送信号

## 3.2 进程组的创建

进程组主要用在 shell 里面， shell 负责进程组的管理，包括创建、销毁等。（这里 shell 就是 session 的 leader ）

- 对大部分进程来说，它自己就是进程组的 leader ，并且进程组里面就只有它自己一个进程

- shell 里面执行类似 `ls|more` 这样的以管道连接起来的命令时，两个进程就属于同一个进程组，ls 是进程组的 leader 。

- shell 里面启动一个进程后，一般都会将该进程放到一个单独的进程组，然后该进程 fork 的所有进程都会属于该进程组。

## 3.3 后台进程组

shell 中启动一个进程时，默认情况下，该进程就是一个前端进程组的 leader ，可以收到用户的输入，并且可以将输出打印到终端，只有当该进程组退出后， shell 才可以再响应用户的输入。

但我们也可以将该进程组运行在后台，这样 shell 就可以继续相应用户的输入，常见的方法如下：

- 启动程序时，在后面加 `&` ，如 `sleep 1000 &` ，进程将会进入后台继续运行

- 程序启动后，可以按 `CTRL+Z` 让它进入后台，和后面加 `&` 不同的是，进程会被暂停执行

对于后台运行的进程组，在 shell 里面体现为 **job** 的概念，即一个后台进程组就是一个  job ， job 有如下限制：

- 默认情况下，只要后台进程组的任何一个进程读 tty ，将会使整个进程组的所有进程暂停

- 默认情况下，只要后台进程组的任何一个进程写 tty ，将有可能会使整个进程组的所有进程暂停（依赖于tty的配置）

所有后台运行的进程组可以通过 jobs 命令查看到，也可以通过 fg 命令将后台进程组切换到前端，这样就可以继续接收用户的输入了。

自然而然，就有了前端进程组的概念，而前端进程组 (Foreground process group) ，记录了当前前端的进程组是哪一个。

# 0x04. session 和进程组的关系

shell 作为 session leader 的情况，假设我们在 shell 里面执行了这些命令：
```bash
$ sleep 1000 &
[1] 1646
$ cat | wc -l &
[2] 1648
$ jobs
[1]-  Running                 sleep 1000 &
[2]+  Stopped                 cat | wc -l
```

下面这张图标明了这种情况下它们之间的关系：
```
+--------------------------------------------------------------+
|                                                              |
|      pg1             pg2             pg3            pg4      |
|    +------+       +-------+        +-----+        +------+   |
|    | bash |       | sleep |        | cat |        | jobs |   |
|    +------+       +-------+        +-----+        +------+   |
| session leader                     | wc  |                   |
|                                    +-----+                   |
|                                                              |
+--------------------------------------------------------------+
                            session
```

pg = process group(进程组)

-  bash 是 session 的 leader ，sleep、cat、wc 和 jobs这四个进程都由 bash fork 而来，所以他们也属于这个 session 

-  bash 也是自己所在进程组的 leader 

-  bash 会为自己启动的每个进程都创建一个新的进程组，所以这里 sleep 和 jobs 进程属于自己单独的进程组

- 对于用管道符号 `|` 连接起来的命令， bash 会将它们放到一个进程组中

# 0x05. nohup

nohup 干了这么几件事：

- 将 stdin 重定向到 `/dev/null` ，于是程序读标准输入将会返回 EOF

- 将 stdout 和 stderr 重定向到 nohup.out 或者用户通过参数指定的文件，程序所有输出到 stdout 和 stderr 的内容将会写入该文件（有时在文件中看不到输出，有可能是程序没有调用flush）

- 屏蔽掉 SIGHUP 信号

- 调用 exec 启动指定的命令（ nohup 进程将会被新进程取代，但进程ID不变）

从上面 nohup 干的事可以看出，通过 nohup 启动的程序有这些特点：

-  nohup 程序不负责将进程放到后台，这也是为什么我们经常在 nohup 命令后面要加上符号 `&` 的原因

- 由于 stdin 、 stdout 和 stderr 都被重定向了， nohup 启动的程序不会读写 tty

- 由于 stdin 重定向到了 `/dev/null` ，程序读 stdin 的时候会收到 EOF 返回值

-  nohup 启动的进程本质上还是属于当前 session 的一个进程组，所以在当前 shell 里面可以通过 jobs 看到 nohup 启动的程序

- 当 session  leader 退出后，该进程会收到 SIGHUP 信号，但由于 nohup 帮我们忽略了该信号，所以该进程不会退出

- 由于 session leader 已经退出，而 nohup 启动的进程属于该 session ，于是出现了一种情况，那就是通过 nohup 启动的这个进程组所在的 session 没有 leader ，这是一种特殊的情况，内核会帮我们处理这种特殊情况

通过 nohup ，我们最后达到了就算 session leader （一般是 shell ）退出后，进程还可以照常运行的目的。

# 0x06. deamon

通过 nohup ，就可以实现让进程在后台一直执行的功能，为什么我们还要写 deamon 进程呢？

从上面的 nohup 的介绍中可以看出来，虽然进程是在后台执行，但进程跟当前 session 还是有着千丝万缕的关系，至少其父进程还是被 session 管着的，所以我们还是需要一个跟任何 session 都没有关系的进程来实现 deamon 的功能。实现 deamon 进程的大概步骤如下：

- 调用 fork 生成一个新进程，然后原来的进程退出，这样新进程就变成了孤儿进程，于是被 init 进程接收，这样新进程就和调用进程没有父子关系了。

- 调用 setsid ，创建新的 session ，新进程将成为新 session 的 leader ，同时该新 session 不和任何 tty 关联。

- 切换当前工作目录到其它地方，一般是切换到根目录，这样就取消了对原工作目录的引用，如果原工作目录是某个挂载点下面的目录，这样就不会影响该挂载点的卸载。

- 关闭一些从父进程继承过来而自己不需要的 fd ，避免不小心读写这些 fd。

- 重定向 stdin、stdout 和 stderr ，避免读写它们出现错误。