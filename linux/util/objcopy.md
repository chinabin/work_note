# 0x00. 导读

[11](https://tangxinfa.github.io/article/linux-4e0b8c038bd5526553bb8c038bd54fe1606f76847a0b5e8f5d296e83.html)

# 0x01. 简介

将目标文件的一部分或者全部内容拷贝到另外一个目标文件中，或者实现目标文件的格式转换。 

```
Debug 等同于 -O0 -g 编译选项, 不优化代码, 附加调试信息
Release 等同于 -O3 编译选项, 最大化优化代码, 但不包含调试信息
RelWithDebugInfo 等同于 -O2 -g 编译选项, 中等程度优化代码, 并且包含调试信息.

可以自定义使用-O3 -g 的编译选项,来生成最接近Release的代码
```

```
1. objcopy –only-keep-debug foo foo.dbg 把调试信息(即debug section)dump到单独的文件里.
2. objcopy –strip-debug foo 这里只剔除调试信息,符号信息仍保留(用户依然可以用nm查看到各种函数符号,我推荐用–strip-all,连同符号信息一起删除,生成的可执行文件更小,不影响调试)
3. objcopy –add-gnu-debuglink=foo.dbg foo 为foo添加.gnu-debuglink section, 这个会让二进制增加近100字节的样子.

这样分离了Debug Section后,就可以单独分发release版本的foo,调试时只需讲foo.dbg放在和foo同一目录（或者在 foo 所在目录新建 .debug 目录，将调试信息放进去就好）下即可直接gdb foo进行调试.

如果不希望Release的最终二进制文件包含.gnu-debuglink段,那么可以直接省略第三步。
```

# 0x02.

```bash
# 将 foo 程序的调试信息存到 foo.dbg 中
$ objcopy --only-keep-debug  foo foo.dbg

# 从 foo 程序中删除调试信息
$ objcopy --strip-debug foo
```

# 0x03. CMake 实操

PRE_BUILD - 命令将会在其他依赖项执行前执行  
PRE_LINK - 命令将会在其他依赖项执行完后执行  
POST_BUILD - 命令将会在目标构建完后执行

```
cmake_minimum_required(VERSION 3.5) 
 
# set the project name 
project(test_EXE) 
 
#SET(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-g -O3 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(test_EXE
test.cpp 
)

#定义命令用于从release中剔除符号表和调试信息
add_custom_command(TARGET test_EXE
    POST_BUILD
    COMMAND objcopy --only-keep-debug ${EXECUTABLE_OUTPUT_PATH}/test_EXE  ${EXECUTABLE_OUTPUT_PATH}/test_EXE.debug
    COMMAND objcopy --strip-all ${EXECUTABLE_OUTPUT_PATH}/test_EXE
    COMMAND objcopy --add-gnu-debuglink=${EXECUTABLE_OUTPUT_PATH}/test_EXE.debug ${EXECUTABLE_OUTPUT_PATH}/test_EXE
    COMMENT "**** test cmake command: run obj copy"
)

```