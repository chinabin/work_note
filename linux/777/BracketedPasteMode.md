# 0x00. 导读

# 0x01. 简介

# 0x02. 

在 ssh 终端黏贴的时候，数据首尾会出现多余的符号，例如
```
00~this should not paste like this01~
```

解决起来很简单，输入命令
```
$ printf "\e[?2004l"
```

还可以将下面的命令拷贝到 `.bashrc`
```
# Disable bracketed paste
if [ "$PS1" ]; then
  printf "\e[?2004l"
fi
```

这个称为 `bracketed paste mode`, 是一些 shell 为了让你知道什么是你输入的，什么是你拷贝黏贴的。