# 0x00. 导读

# 0x01. 简介

# 0x02. 变量

see [cmake-variables](https://cmake.org/cmake/help/v3.12/manual/cmake-variables.7.html)

## 2.1 PROJECT_NAME

```cmake
# set the project name
project(Tutorial)
```

指定了项目名后，后面可能会有多个地方用到这个项目名，如果更改了这个名字，就要改多个地方，比较麻烦，那么可以使用 PROJECT_NAME 来表示项目名。
```cmake
add_executable(${PROJECT_NAME} tutorial.cpp)
```

## 2.2 PROJECT_BINARY_DIR 

表示当前工程的二进制路径，即编译产物会存放到该路径，此时 PROJECT_BINARY_DIR 就是 build 所在路径。可以任务等同于 CMAKE_BINARY_DIR.

## 2.3 PROJECT_SOURCE_DIR

工程顶层目录。

## 2.4 CMAKE_SOURCE_DIR

根源代码目录，工程顶层目录。可以认为等同于 PROJECT_SOURCE_DIR.

## 2.5 CMAKE_CURRENT_SOURCE_DIR

当前处理的 `CMakeLists.txt` 所在的路径

## 2.6 CMAKE_BINARY_DIR

外部构建时就是 build 目录。

## 2.7 CMAKE_CXX_FLAGS

编译选项，用来传递 flags 给编译器，cmake 内置变量，只影响 c++ 编译器。

CMAKE_CXX_FLAGS 是CMake传给C++编译器的编译选项，通过设置这个值就好比 g++ -std=c++11 -g -Wall

CMAKE_CXX_FLAGS_DEBUG 是除了CMAKE_CXX_FLAGS外，在Debug配置下，额外的参数

CMAKE_CXX_FLAGS_RELEASE 同理，是除了CMAKE_CXX_FLAGS外，在Release配置下，额外的参数