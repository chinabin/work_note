[一个程序的前世今生（一）——代码如何变成可执行文件](https://www.cnblogs.com/edver/p/13449843.html)
[一个程序的前世今生（二）——可执行文件如何加载进内存](https://www.cnblogs.com/edver/p/13752835.html)
[一个程序的前世今生（三）——动态链接库和静态链接库](https://www.cnblogs.com/edver/p/13769577.html)
[一个程序的前世今生（四）——延迟绑定和GOT与PLT](https://www.cnblogs.com/edver/p/13769582.html)




gcc/g++ 编译我

ldd 查看我依赖哪些库
readelf 看看程序文件本身的组织格式

strace 看我跑起来了在干嘛  
pstack 给我的栈来个快照

/proc/pid/maps 包含了我这个进程映射的内存区域以及访问权限  
/proc/pid/smaps 显示了进程映射的内存消耗
/proc/pid/status 进程状态信息