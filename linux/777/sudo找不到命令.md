以命令 A 举例
```bash
$ sudo env | grep PATH
PATH=/sbin:/bin:/usr/sbin:/usr/bin

$ type cyclictest
cyclictest is /usr/local/bin/cyclictest

$ whereis cyclictest
cyclictest: /usr/local/bin/cyclictest

# 建立软链接完事
$ sudo ln -sf /usr/local/bin/cyclictest /usr/bin/cyclictest
```

但是通常来说，我们都认为这几个目录这样安排：

- /bin 放置系统的关键程序，比如 ls cat ，对于“关键”的定义，不同的发行版会有不同的理解；
- /usr/bin 放置发行版管理的程序，比如 Ubuntu 自带 md5sum ，这个 binary 就会在这个目录下；
- /usr/local/bin 放置用户自己的程序，比如你编译了一个 gcc，那么 gcc 这个可执行 binary 应该在这个目录下；
- 除此之外，还有对应的三个目录 /sbin /usr/sbin /usr/local/sbin ，放置系统管理的程序，比如 deluser chroot service ；


编译软件的时候，./configure --prefix=/usr/local 然后 make && make install 。那么 /usr/local 就会作为 prefix，库文件就放在 /usr/local/lib 下面，配置文件就放在 /usr/local/etc 下面，可执行文件（binary）就放在 /usr/local/bin 下面。