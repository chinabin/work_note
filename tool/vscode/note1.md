# 0x00. 导读

# 0x01. 简介

# 0x02. 配置

## settings.json

从高到低分别是全局设置、工作区设置、文件夹设置; 全局设置默认通过 ctrl+shift+P 后输入 settings.json 打开。

左下角 manage -> settings ，目前存放 `/C:/Users/NUC/AppData/Roaming/Code/User/settings.json`

## tasks.json

vscode 就使用 tasks 来支持快捷实现一些方便的功能。

## launch.json

主要是用来对调试提供支持。

## c_cpp_properties.json

C/C++插件提供的，c_cpp_properties.json

该配置文件是给编辑器使用的，用于编辑器正确跳转，高亮，宏开关正常显示等作用，跟编译没关系。

# 0x03. cmake

Ctrl+Shift+p 打开VSCode的指令面板，然后输入 cmake:q ，VSCode 会根据输入自动提示，然后选择 CMake: Quick Start

## 3.1 cmake-variants.yaml

该配置文件指定 CMake Config 的配置，包括 buildType ，以及不同配置的 CMake 环境变量和 settings 的变量

# 4. vscode-server

```
help -> about 查看 commitid
Version: 1.84.2 (user setup)
Commit: 1a5daa3a0231a0fbba4f14db7ec463cf99d7768e
Date: 2023-11-09T10:51:52.184Z
Electron: 25.9.2
ElectronBuildId: 24603566
Chromium: 114.0.5735.289
Node.js: 18.15.0
V8: 11.4.183.29-electron.0
OS: Windows_NT x64 10.0.19045
```

下载对应的 vscode-server:
```
https://update.code.visualstudio.com/commit:commit_id/server-linux-x64/stable

例如 
https://update.code.visualstudio.com/commit:1a5daa3a0231a0fbba4f14db7ec463cf99d7768e/server-linux-x64/stable
```

得到 `vscode-server-linux-x64.tar.gz ` 文件。登录远程机器，在 `~/.vscode-server/bin/` 下面解压，然后将解压文件重命名为之前的 commit id ，然后在这个目录中创建一个名为 0 的文件：
```bash
$ cd ~/.vscode-server/bin/1a5daa3a0231a0fbba4f14db7ec463cf99d7768e
$ touch 0
```

vscode-server 完成远程部署后不包含扩展，可将其它已安装有 vscode 的 Ubuntu 机器的 ~/.vscode 目录下的 extensions 目录放到上文中创建的远程服务器的 ~/.vscode-server 目录下，实现扩展离线安装，或通过下文中将提到的方法直接将本地扩展全部或部分在线安装到远程服务器。