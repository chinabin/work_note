# 0x00. 导读

# 0x01. 简介

```bash
# 在源码目录使用，能创建 build 并且生成 makefile
$ cmake -B build

# 主要是为了跨平台，和上面功能一样
cmake --build build
```

# 0x02. 

创建一个 build 目录存放编译产物，可以避免编译产物与代码文件混在一起，这种叫做外部构建(out-of-source)。

还有一种内部构建(in source)，即直接在项目根目录下进行构建系统与编译。


## 2.1 变量

set 命令指定 SRC_LIST 变量来表示多个源文件，用 ${var_name} 获取变量的值

```cmake
set(SRC_LIST a.cpp b.cpp c.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})
```

# 2. configure_file

```cmake
configure_file(input output options)
```
将一个文件(由 input 参数指定)拷贝到指定位置(由 output 参数指定)，并根据 options 修改其内容。

具体来说，会做如下几个替换：
1. 将 input 文件中的 `@var@或者${var}` 替换成 cmake 中指定的值。
2. 将 input 文件中的 `#cmakedefine var` 关键字替换成 `#define var` 或者 `#undef var` ，取决于 cmake 是否定义了 var 。

举例说明：  
`version.h.in`
```c
const char *SR_VERSION = R"SR(
                 build time: ${build_time} by ${build_user}
                 build type: ${CMAKE_BUILD_TYPE}
                 git version: ${build_version}
                 compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}
                 os: ${CMAKE_SYSTEM}
)SR";

```

CMakeLists.txt  
```cmake
# configure version
execute_process(COMMAND git rev-parse --short HEAD
                OUTPUT_VARIABLE build_version
                OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND date "+%Y-%m-%d %H:%M:%S" OUTPUT_VARIABLE build_time
                OUTPUT_STRIP_TRAILING_WHITESPACE)

set(build_user $ENV{USER})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in ${CMAKE_BINARY_DIR}/version.h)
```

执行 `cmake .` 命令自动生成的 version.h 文件内容
```c
const char *SR_VERSION = R"SR(
                 build time: 2023-11-30 10:52:40 by centos
                 build type: Debug
                 git version: 95e5299
                 compiler: GNU 9.3.1
                 os: Linux-3.10.0-1160.49.1.el7.x86_64
)SR";

```

## 2.3 打印编译参数

在 CMakeLists.txt 文件中添加 `SET( CMAKE_VERBOSE_MAKEFILE on )` 然后，make 的时候就能看到打印

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) or make VERBOSE=1