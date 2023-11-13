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