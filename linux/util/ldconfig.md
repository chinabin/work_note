# 0x00. 导读

# 0x01. 简介

ldconfig 是一个动态链接库管理命令。

主要是在默认搜寻目录 (`/lib` 和 `/usr/lib`) 以及动态库配置文件 `/etc/ld.so.conf` 内所列的目录下,搜索出可共享的动态链接库(格式如 lib*.so* ), 进而创建出动态装入程序 (ld.so) 所需的连接和缓存文件. 缓存文件默认为 `/etc/ld.so.cache`, 此文件保存已排好序的动态链接库名字列表.

ldconfig 通常在系统启动时运行，而当用户安装了一个新的动态链接库时，就需要手工运行这个命令。

ldconfig需要注意的地方：

1. 往 /lib 和 /usr/lib 里面加东西，是不用修改 /etc/ld.so.conf 文件的，但是添加完后需要调用下 ldconfig ，不然添加的 library 会找不到。

2. 如果添加的 library 不在 /lib 和 /usr/lib 里面的话，就一定要修改 /etc/ld.so.conf 文件，往该文件追加 library 所在的路径，然后也需要重新调用下 ldconfig 命令。比如在安装MySQL的时候，其库文件 /usr/local/mysql/lib ，就需要追加到 /etc/ld.so.conf 文件中。命令如下：  
    `echo "/usr/local/mysql/lib" >> /etc/ld.so.conf`

    `ldconfig -v | grep mysql`

3. 如果没有权限操作写 /etc/ld.so.conf 文件的话，这时就需要往 export 里写一个全局变量 LD_LIBRARY_PATH ，就可以了。

4. ldconfig 做的这些东西跟程序运行时有关，与编译时没有任何一点关系。编译的时候还是该加 -L 就得加，不要混淆了

# 0x02. 命令

```bash
-c, --format=FORMAT        
指定缓存文件所使用的格式,共有三种: ld(老格式),new(新格式)和compat(兼容格式,此为默认格式)

-C CACHE                   
指定生成的缓存文件为 CACHE ,系统默认的是 /etc/ld.so.cache ,此文件存放已排好序的可共享的动态链接库的列表

-f CONF                    
指定动态链接库的配置文件为CONF,系统默认为/etc/ld.so.conf.

-n				 
ldconfig仅扫描命令行指定的目录,不扫描默认目录(/lib,/usr/lib),也不扫描配置文件/etc/ld.so.conf所列的目录

-p, --print-cache          
指示ldconfig打印出当前缓存文件所保存的所有共享库的名字

```