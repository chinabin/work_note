gcc/g++ 编译我

ldd 查看我依赖哪些库
readelf 看看程序文件本身的组织格式

strace 看我跑起来了在干嘛  
pstack 给我的栈来个快照

/proc/pid/maps 包含了我这个进程映射的内存区域以及访问权限  
/proc/pid/smaps 显示了进程映射的内存消耗
/proc/pid/status 进程状态信息