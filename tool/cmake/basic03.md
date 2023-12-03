# 0x00. 导读

# 0x01. 简介

# 0x02. 指令

INTERFACE|PUBLIC|PRIVATE 指的是影响可以传播的范围。

没有 target 的下面的指令就像是全局的，不推荐使用。

## 2. option 

option 表示提供用户可以选择的选项。命令格式为：
```cmake
option(<variable> "description" [initial value])
```

可以使用 `cmake -D` 参数设定

## 2. add_compile_options

## 2. target_compile_options

添加编译命令选项

## 2. add_compile_definitions

不建议使用

添加编译命令选项

## 2. add_definitions

不建议使用

宏定义

## 2. target_add_definitions

宏定义

## 2. aux_source_directory

查找指定目录下的所有源文件，然后将结果存进指定变量名。其语法如下：

```cmake
aux_source_directory(<dir> <variable>)
```

```cmake
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
```

## 2. target_sources

添加要编译的源文件

## 2. include_directories

不建议使用。
```cmake
include_directories(${PROJECT_SOURCE_DIR}/include)
```

## 2. target_include_directories

头文件搜索目录
```cmake
target_include_directories(mylib
                        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                        )
```

## 2. target_link_directories

## 2. link_libraries

不建议使用

库文件搜索路径

## 2. target_link_libraries

添加需要链接的库
```cmake
list(APPEND EXTRA_LIBS gcov mylib)
target_link_libraries(mymain ${EXTRA_LIBS})
```




## 2. add_executable

```cmake
add_executable(mymain ${SOURCES})
```

## 2. target_compile_features

定义 target 所需的编译特性，让 cmake 解决编译选项。