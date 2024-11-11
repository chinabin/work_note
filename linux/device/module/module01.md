# 0x00. 导读

# 0x01. 简介

Linux内核非常庞大，包含很多组件。我们如何把需要的部分包含在内核中？
两种办法：
- 1）把所有需要的功能都编译进内核中。
  
    会导致2个问题：生成的内核会非常大；为现有内核添加、删除功能，将不得不重新编译。

- 2）不包含所有功能，需要的时候，动态地加载代码到内核。
  
    这种机制称为模块（Module）。

模块特点：

- 模块本身不被编译入内核映像，从而控制内核的大小。
- 模块一旦被加载，就和内核中其他部分完全一样。
- 
Linux 中，一个 .ko 文件就是一个模块文件，可以通过 insmod/rmmod/lsmod 命令，动态地加载/卸载/查看模块。

## 1.1 例子

以点亮LED为例，分析操作系统各层间的协作关系:

1. 应用程序调用库提供的 open 函数打开代表 LED 的设备文件；
2. 库根据open函数传入的参数执行“swi”指令，这条指令会引起CPU异常，进而进入内核；
3. 内核的异常处理函数根据这些参数找到相应的驱动程序，返回一个文件句柄给库，进而返回给应用程序；
4. 应用程序得到文件句柄后，使用库提供的write或ioctl函数发出控制指令。
5. 库根据write或ioctl函数传入的参数执行“swi”指令，引起CPU异常，进入内核；
6. 内核的异常处理函数根据这些参数调用驱动程序的相关函数，点亮LED

库给应用程序通过的接口函数：open、read、write、ioctl、mmap等（系统调用），都是设置好相应寄存器后，执行某条指令引发异常进入内核，对于ARM架构CPU，指令为swi，软中断

实际上，内核和驱动并没有界线，因为驱动最终是要通过静态链接和动态加载编进内核中去的；从上分析可知，驱动一般是被动的，根据应用程序的需求来执行。

# 0x02. ko

在 Linux 操作系统中，**.ko** 文件是内核模块文件的扩展名。**内核模块** 是一种可以在运行时加载和卸载的代码，用于扩展 Linux 内核的功能，无需重新编译整个内核。每个 .ko 文件包含模块的代码和元数据，允许用户在不重新启动系统的情况下添加或移除特定功能，如驱动程序或文件系统支持。

## 2.1 ko 代码例子

```c
#include <linux/module.h>
#include <linux/init.h>

static int helloworld_init(void)
{
    printk("helloworld!\n");
    return 0;
}

static void helloworld_exit(void)
{
    printk("helloworld bye\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luckfox");
MODULE_VERSION("V1.0");

```

- 模块加载函数
  
    当通过 `insmod` 或 `modprobe` 命令加载内核模块时，模块的加载函数会自动被内核执行，完成本模块的相关初始化工作。  
    Linux 内核模块加载函数一般以 `__init` 标识声明，模块加载函数以 `module_init(函数名)` 的形式被指定。它返回整型值，若初始化成功，应返回 0。而在初始化失败时，应该返回错误编码。在 Linux 内核里，错误编码是一个接近于0的负值。

- 模块卸载函数
  
  当通过 `rmmod` 命令卸载某模块时，模块的卸载函数会自动被内核执行，完成与模块卸载函数相反的功能。  
  Linux 内核模块加载函数一般以 `__exit` 标识声明，模块卸载函数在模块卸载的时候执行，而不返回任何值，且必须以 `module_exit(函数名)` 的形式来指定。

- 模块许可证声明许可证（LICENSE）
    明描述内核模块的许可权限，如果不声明 LICENSE, 模块被加载时，将收到内核被污染（Kernel Tainted）的警告。在Linux内核模块领域，可接受的 LICENSE 包括“GPL”、“GPL v2”、“GPLand additional rights”、“Dual BSD/GPL”、“Dual MPL/GPL”和“Proprietary”。大多数情况下，内核模块应遵循GPL兼容许可权。Linux内核模块最常见的是以 MODULE_LICENSE（“GPL v2”）语句声明模块采用GPL v2。

- 模块作者等信息声明
    MODULE_AUTHOR、MODULE_DESCRIPTION、MODULE_VERSION、MODULE_DEVICE_TABLE、MODULE_ALIAS 分别声明模块的作者、描述、版本、设备表和别名

mydriver.c 文件 make 编译后会产生 mydriver.ko mydriver.mod.c mydriver.mod.o mydriver.o 等文件，进入所在目录，执行 `insmod mydriver.ko` ，也可以选择用命令 `modprobe` 执行，麻烦点。

然后使用 `lsmod | grep mydriver` 查看已加载的驱动模块列表，以及使用 `modinfo mydriver.ko` 查看模块信息。最后使用 `rmmod mydriver` 卸载模块。

## 2.2 makefile

linux 内核使用的是 `kbuild` 编译系统，在编译可加载模块时，其 makefile 的风格和常用的编译C程序的 makefile 有所不同，尽管如此，makefile的作用总归是给编译器提供编译信息。

`obj-m` 表示编译生成可加载模块。相对应的，obj-y 表示直接将模块编译进内核。obj-m 和 obj-y 是 GNU make 提供的扩展语法，并不是 gcc 的通用语法。

```
构建模块：
make -C <path to kernel src> M=$PWD

安装模块：
make -C <path to kernel src> M=$PWD modules_install
```

1. `$KDIR：/lib/modules/$(shell uname -r)/build/`，指定内核源码（其实并不需要内核源码，需要的是目标机的内核头文件）的位置。
2. `M=$(PWD)`：需要编译的模块源文件地址，M= 选项使 makefile 在试图建立模块目标前, 回到你的模块源码目录。
3. `[target]`：modules，事实上，这是个可选选项。

`obj-m := module_name.o`

其中，module_name.o 是需要编译为模块的目标文件名（编译器会自动推倒 module_name.c 文件）。编译完成后，会生成一个名为 module_name.ko 的模块文件。

```makefile
obj-m := bbro_pcie.o
bbro_pcie-objs := bbro_pcie_dev.o bbro_pcie_pci.o
```

模块对应是 bbro_pcie ，依赖于 bbro_pcie_dev.o bbro_pcie_pci.o 两个对象。

注意，有时候报错 `could not insert module xquote.ko: Invalid module format` 或者 dmesg 显示 `compat: disagrees about version of symbol module_layout` 原因是由于当前使用的内核和模块是重新编译过的，导致 /usr/src/linux-headers-5.15.0-56-generic/Module.symvers 里的 module_layout 值和当前使用的内核(及模块) module_layout 值不匹配，所以在编译的时候，指定 KLIB 和 KLIB_BUILD 为当前内核源码的路径去编译，问题即可解决。

```
KDIR = /lib/modules/$(shell uname -r)/build
KLIB = /lib/modules/$(shell uname -r)/build
KLIB_BUILD = /lib/modules/$(shell uname -r)/build
```

/usr/src/kernels/ 为空的解决
```
$ sudo make                    
make  -C /lib/modules/5.14.0-284.11.1.el9_2.x86_64/build M=/home/alfred/tab/u50/driver modules       
make[1]: *** /lib/modules/5.14.0-284.11.1.el9_2.x86_64/build: 没有那个文件或目录。 停止。            
make: *** [Makefile:10：all] 错误 2
       
[alfred@localhost driver]$ ls -hl /lib/modules/5.14.0-284.11.1.el9_2.x86_64/build 
lrwxrwxrwx. 1 root root 45  5月 10  2023 /lib/modules/5.14.0-284.11.1.el9_2.x86_64/build -> /usr/src/kernels/5.14.0-284.11.1.el9_2.x86_64

[alfred@localhost driver]$ ls -hlrt /usr/src/kernels/   
总用量 0   
[alfred@localhost driver]$ sudo dnf update
[alfred@localhost driver]$ sudo dnf install -y kernel
[alfred@localhost driver]$ sudo dnf install -y kernel-devel

重启，很关键！！！！

$ ls -hlrt /usr/src/kernels/
总用量 4.0K
drwxr-xr-x. 25 root root 4.0K 10月 28 09:29 5.14.0-427.40.1.el9_4.x86_64

$ cd /lib/modules/5.14.0-284.11.1.el9_2.x86_64/
$ sudo ln -sf /usr/src/kernels/5.14.0-427.40.1.el9_4.x86_64 build
```

Skipping BTF generation, pahole：未找到命令
```
$ sudo dnf install dwarves -y
```

签名
```
$ grep CONFIG_MODULE_SIG /boot/config-$(uname -r)
CONFIG_MODULE_SIG_FORMAT=y
CONFIG_MODULE_SIG=y
# CONFIG_MODULE_SIG_FORCE is not set
CONFIG_MODULE_SIG_ALL=y
# CONFIG_MODULE_SIG_SHA1 is not set
# CONFIG_MODULE_SIG_SHA224 is not set
# CONFIG_MODULE_SIG_SHA256 is not set
# CONFIG_MODULE_SIG_SHA384 is not set
CONFIG_MODULE_SIG_SHA512=y
CONFIG_MODULE_SIG_HASH="sha512"
CONFIG_MODULE_SIG_KEY="certs/signing_key.pem"


$ cd /usr/src/kernels/uname -r/certs
$ vim x509.genkey
[ req ]
default_bits = 4096
distinguished_name = req_distinguished_name
prompt = no
string_mask = utf8only
x509_extensions = myexts

[ req_distinguished_name ]
CN = Modules

[ myexts ]
basicConstraints=critical,CA:FALSE
keyUsage=digitalSignature
subjectKeyIdentifier=hash
authorityKeyIdentifier=keyid
$ sudo openssl req -new -nodes -utf8 -sha512 -days 36500 -batch -x509 -config x509.genkey -outform DER -out signing_key.x509 -keyout signing_key.pem
```