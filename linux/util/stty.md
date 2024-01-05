# 0x00. 导读

# 0x01. 简介

通常，我们可以使用 Ctrl + C 来发送 SIGINT(2) 信号实现程序强制关闭

这里有一个很容易引起困惑的地方: Ctrl + C 是如何与 SIGINT(2) 绑定起来的呢？

在某些未正确配置终端的情况下，我们还会发现按下上下左右箭头，终端并不会跳转到前一条命令或是移动光标，而是打印一些奇怪的字符 ^A、^B、^M，这又是什么意思呢？

从本质上来说，我们在终端的输入，实际上都会被转换为 ASCII，其中 0~9、a-z 等可视字符自然是原样传输，而其他一些控制字符，则会被系统转换为对应的信号或是特殊操作被处理。

# 0x02. 

```
$ $ stty -a

speed 38400 baud; rows 30; columns 120; line = 0;
intr = ^C; quit = ^\; erase = ^?; kill = ^U; eof = ^D; eol = <undef>; eol2 = <undef>; swtch = <undef>; start = ^Q;
stop = ^S; susp = ^Z; rprnt = ^R; werase = ^W; lnext = ^V; discard = ^O; min = 1; time = 0;
-parenb -parodd -cmspar cs8 -hupcl -cstopb cread -clocal -crtscts
-ignbrk -brkint -ignpar -parmrk -inpck -istrip -inlcr -igncr icrnl ixon -ixoff -iuclc -ixany -imaxbel -iutf8
opost -olcuc -ocrnl onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0
isig icanon iexten echo echoe echok -echonl -noflsh -xcase -tostop -echoprt echoctl echoke -flusho -extproc
```

`intr = ^C` 的字段，这表示 Ctrl + C 会触发 intr(SIGINT) 信号，使用 `stty intr ^G`，即可改绑按键，再次调用命令，可以发现 Ctrl + C 已经不会停止进程了，取而代之的是 Ctrl + G

```
eof : 输入结束，文件结束，默认为Ctrl+D。比如：用cat >file来创建文件时，按Ctrl+D来结束输入。

erase : 向后删除字符，擦除最后一个输入字符，默认为Ctrl+?。注意默认情况下退格键Backspace不是删除字符。

intr : 中断当前程序，默认为Ctrl+C。

kill : 删除整条命令，删除整行，默认为Ctrl+U。

quit :退出当前程序，默认为Ctrl+\或Ctrl+|。

start : 启动屏幕输出，默认为Ctrl+Q。

stop :停止屏幕输出，默认为Ctrl+S。

susp : terminal stop当前程序，默认为Ctrl+Z。这样当前进程就会变成后台进程了。

werase：删除最后一个单词，默认为Ctrl+W。
```

## 2.1 原生输入

信号可以改绑，自然就可以取消绑定。有一些伪命令行客户端（看起来是个命令行，但是和我们通常的 shell 有不同），如 mongodb、mysql 的客户端内。我们通常希望按下上箭头会跳回上一条命令，但是往往输出的是奇怪的字符。
这是因为我们输入的上箭头被 tty 处理为信号了，程序内部并没有收到我们输入的上箭头的键码

在这种场景下，可以使用 stty raw 来设定使用原生输入，不做处理。这会导致所有信号都不会被监听，如果程序内没有处理 SIGINT，那么在程序自己退出前，无论按多少次 Ctrl + C，程序只会收到 2 这个数字本身（使用 `stty -raw` 可还原）

## 2.2 回显

在开发一些按键监听的功能（比如 AutoHotkey），我们为了确认按键是否按下，往往需要去调试输出按下的按键。这在 GUI 程序内似乎是理所当然的。

但是在终端中，我们按下键立即可以看到我们输入的内容似乎也是理所当然的，这种回显实际上也是由 tty 实现的。

如果使用 `stty -echo`，即可关闭回显。
无论我们按下什么键，都不会有东西显示在终端中。但是如果我们能够确认输入是正确的，按下回车则可以正确执行命令。（使用 `stty echo` 可还原）

## 2.3 

**start = ^Q; stop = ^S;**  

这是两个特殊的控制命令，估计经常有人会碰到，在键盘上不小心输入CTRL+S后，终端没反应了，即没输出，也不响应任何输入。这是因为这个命令会告诉TTY暂停，阻塞所有读写操作，即不转发任何数据，只有按了CTRL+Q后，才会继续。这个功能应该是历史遗留，以前终端和服务器之间没有流量控制功能，所以有可能服务器发送数据过快，导致终端处理不过来，于是需要这样一个命令告诉服务器不要再发了，等终端处理完了后在通知服务器继续。

该命令现在比较常用的一个场景就是用tail -f命令监控日志文件的内容时，可以随时按CTRL+S让屏幕停止刷新，看完后再按CTRL+Q让它继续刷，如果不这样的话，需要先CTRL+C退出，看完后在重新运行tail -f命令。

**-tostop**

如果你在shell中运行程序的时候，后面添加了&，比如./myapp &，这样myapp这个进程就会在后台运行，但如果这个进程继续往tty上写数据呢？这个参数就用来控制是否将输出转发给终端，也即结果会不会在终端显示，这里“-tostop”表示会输出到终端，如果配置为“tostop”的话，将不输出到终端，并且tty会发送信号SIGTTOU给myapp，该信号的默认行为是将暂停myapp的执行。

# 0x03.

在 stty 中相应选项前冠以负号 (-) ，该选项被清除（禁止）；如果无负号，该选项被设置。

```
# 打印出终端的行数和列数
$ stty size
```