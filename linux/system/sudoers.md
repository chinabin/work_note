# 0x00. 导读

[Sudo命令的使用技巧](https://www.escapelife.site/posts/d3407f3a.html)

# 0x01. 简介

# 0x02. 

[1] 理解 sudo 命令的工作流程

- sudo 会读取和解析 /etc/sudoers 文件，查找调用命令的用户及其权限。
- 然后提示调用该命令的用户输入密码，或者也可以通过 NOPASSWD 标志来跳过密码验证。
- 之后，sudo 创建一个子进程，调用 setuid() 来切换到目标用户。
- 最好，它会在上述子进程中执行参数给定的 shell 或命令。

[2] 理解 sudo 命令授权配置

- USER/GROUP HOST=(USER[:GROUP]) [NOPASSWD:] COMMANDS
- USER/GROUP: 表示需要被授权的用户或者组；如果是组则需要以 % 开头
- HOST: 表示允许从哪些主机登录的用户运行 sudo 命令；ALL 表示允许从任何终端、机器访问
- (USER[:GROUP]): 表示使用 sudo 可切换的用户或者组，组可以不指定；ALL 表示可以切换到系- 统的所有用户
- NOPASSWD: 如果指定，则该用户或组使用 sudo 时不必输入密码
- COMMANDS: 表示运行指定的命令；ALL 表示允许执行所有命令

```
vim /etc/sudoers

# 允许用户 youuser 执行 sudo 命令(需要输入密码).
youuser ALL=(ALL) ALL
# 允许用户组 youuser 里面的用户执行 sudo 命令(需要输入密码).
%youuser ALL=(ALL) ALL
# 允许用户 youuser 执行 sudo 命令,并且在执行的时候不输入密码.
youuser ALL=(ALL) NOPASSWD: ALL
# 允许用户组 youuser 里面的用户执行 sudo 命令,并且在执行的时候不输入密码.
%youuser ALL=(ALL) NOPASSWD: ALL

```

## 2.1 sudo 环境问题

我们执行 sudo 命令之后会切换用户，如果保留环境变量会有一定的安全问题，系统会默认重置环境变量为安全的环境变量。先前设置的变量都会失效，只有少数配置文件中指定的环境变量能够保存下来。

我们可以看一下 sudo 配置文件 /etc/sudoers 来找找作用原因。我们执行如下命令之后，可以看到如下输入(有可能和我这里的不一样)。
其中 `env_reset` 表示默认会重置环境变量，因此我们自定义的变量会在 sudo 命令执行之后失效，也就不会正确获取变量值了。
而 `env_keep` 则表示用于保留部分环境变量不被重置，需要保留的变量就写入双引号之中，可自行追加需要保留的变量。
最后就是 `secure_path` 变量，其作用就是包含的路径将被当做 sudo 环境的 PATH 变量来使用。如果在 sudo 环境无法找到某些命令，那么可以将这些命令的路径加入该配置项之中。

```bash
$ sudo sed '/^#/d;/^$/d' /etc/sudoers
Defaults    env_reset
Defaults    env_keep =  "COLORS IDSPLAY HOSTNAME HISTSIZE LS_COLORS"
Defaults    env_keep += "MAIL PS1 PS2 QTDIR USERNAME LANG LC_ADDRESS"
Defaults    secure_path="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin"
```

第一种解决方法，就是在使用的时候，使用 -E 参数。加上 -E 选项后，用户可以在 sudo 执行时保留当前用户已存在的环境变量，不会被 sudo 重置。

第二种解决方法，就是修改 sudo 配置文件。可以通过修改 /etc/sudoers 文件的 env_keep 和 secure_path 配置项，来指定 sudo 环境中需要保留的环境变量和路径。当然，我们也可以把配置文件的变量 !env_reset 给去掉，这样就不会有限制了。