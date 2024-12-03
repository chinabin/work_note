# 0x00. 导读

# 0x01. 简介

tmuxp 是基于 python 开发的 tmux 配置工具，用于管理和配置 tmux 会话，允许用户通过简单的配置文件来定义和组织 tmux 会话，以及保存和加载这些会话。

tmuxp 提供了一个简单且可扩展的方式来管理 tmux 会话，使用户能够更轻松地管理多个 tmux 窗口和面板，以及定义会话的布局和运行的命令。通过 tmuxp，用户可以轻松地创建和维护复杂的 tmux 工作环境，并能够快速地恢复到先前保存的会话状态。

# 0x02. 

安装
```bash
$ pip3 install tmuxp
```

冻结
```
# 使用方式
# session_name: 会话(Session)名称
# -h: tmux服务器的地址
# -L: tmux服务器的socket名，与tmux相同
# -S: tmux服务器的socket路径，与tmux相同
# -2: 强迫tmux的终端支持256色
# -8: 与-2类似，但是只支持88色
# --list=False: 列出可用的配置文件
$ tmuxp load [-h] [-L socket-name] [-S socket-path] [-2 | -8] [--list] [config]

# 通过配置文件路径加载
$ tmuxp load .
$ tmuxp load ./mysession.yaml
$ tmuxp load ~/workspaces/myproject.yaml

# 通过自定义会话名称加载
$ tmuxp load mysession

# 一次加载多个会话
$ tmuxp load mysession ./another/project/
```

加载
```
# 使用方式
# session_name: 会话(Session)名称
# -h: tmux服务器的地址
# -L: tmux服务器的socket名，与tmux相同
# -S: tmux服务器的socket路径，与tmux相同
$ tmuxp freeze [-h] [-L socket-name] [-S socket-path] session_name [session_name ...]

# 为tmux布局、窗格路径和窗口/会话名称创建快照
$ tmuxp freeze session-name
```