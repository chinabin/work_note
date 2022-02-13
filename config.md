## **centos 升级 gcc**    
升级的办法有两个：手动编译安装，或从源安装。大多数情况都不推荐第一种，因为有各种依赖问题需要人工手动解决，并且极其费时费力。推荐第二种：   
红帽其实已经编译好了高版本的gcc，但未更新到base和epel这两个常用的源中，而是放在scl中。第一步便是安装scl：
```
sudo yum install -y centos-release-scl
```
gcc 包含在 Development Tools 这个组中。所以 scl 中的 gcc/g++ 软件包的前缀都是 devtoolset 。安装 gcc 9 版本的命令是
```
sudo yum install devtoolset-9-gcc*
```
注意，多个版本不冲突，可以同时存在 gcc6 gcc 7 gcc8 。   
因为不会覆盖系统默认的gcc，使用这些软件的方法有四种：

1. 使用绝对路径；
2. 添加可执行文件路径到PATH环境变量；
3. 使用官方推荐的加载命令：scl enable devtoolset-x bash, x为要启用的版本;
4. 执行安装软件自带的脚本： source /opt/rh/devtoolset-x/enable，x为要启用的版本。   

推荐使用最后一种方式，可以使用 `gcc -v` 查看版本。如果希望长期使用高版本（因为上述的方法都是只针对当前 terminal 单次有效），可将此命令写入 .bashrc 等配置文件。

最后说一下，scl以及scl-rh源中的软件包都安装在/opt/rh/目录下，包含可执行文件、配置等。所以启用命令的路径是/opt/rh/xxx/enable，安装的服务重启命令则可能是systemctl restart rh-xxx，需要加rh或scl前缀以区别其他源的包。