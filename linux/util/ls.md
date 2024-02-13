# 0x00. 导读

# 0x01. 简介

# 0x02. 

```bash
# 递归展示子目录
$ ls -R

# 按照大小排序
$ ls -S
```

# 0x03. 权限

注意，有时候会出现 S 和 T ，那表示条件不满足，强行设置。

## 3.1 SUID

```bash
$ ls -hl /usr/bin/passwd    
-rwsr-xr-x. 1 root root 28K Apr  1  2020 /usr/bin/passwd
```

当 s 出现在文件 **拥有者** 的 x 权限上时，就被称为 SETUID BITS 或 SETUID ，其特点如下：

- SUID 权限仅对二进制可执行文件有效
- 如果执行者对于该二进制可执行文件具有 x 的权限，执行者将具有该文件的所有者的权限
- 本权限仅在执行该二进制可执行文件的过程中有效

用例子来理解：  
一个普通账号 tester 需要修改自己的密码，那么就要访问 `/etc/shadow` 这个文件。但是只有 root 才能访问 /etc/shadow 这个文件，那么修改自己的密码是如何做到的？

- tester 用户对于 /usr/bin/passwd 这个程序具有执行权限，因此可以执行 passwd 程序
- passwd 程序的所有者为 root
- tester 用户执行 passwd 程序的过程中会暂时获得 root 权限
- 因此 tester 用户在执行 passwd 程序的过程中可以修改 /etc/shadow 文件

```bash
$ chmod u+s FILE...
$ chmod u-s FILE...
```

## 3.2 SGID

当 s 标志出现在用户组的 x 权限时称为 SGID。SGID 的特点与 SUID 相同。

除二进制程序外，SGID 也可以用在目录上。当一个目录设置了 SGID 权限后，它具有如下功能：

- 用户若对此目录具有 r 和 x 权限，该用户能够进入该目录
- 用户在此目录下的有效用户组将变成该目录的用户组
- 若用户在此目录下拥有 w 权限，则用户所创建的新文件的用户组与该目录的用户组相同

```bash
$ chmod g+s DIR...
$ chmod g-s DIR...
```

## 3.3 SBIT

SBIT 是 the  restricted  deletion  flag  or  sticky  bit 的简称。
SBIT 目前只对目录有效，用来阻止非文件的所有者删除文件。比较常见的例子就是 /tmp 目录：

```bash
$ ls -hld /tmp                                  
drwxrwxrwt. 16 root root 4.0K Jan 24 16:13 /tmp
```

权限信息中最后一位 t 表明该目录被设置了 SBIT 权限。SBIT 对目录的作用是：当用户在该目录下创建新文件或目录时，仅有自己和 root 才有权力删除。

```bash
$ chmod o+t DIR...
$ chmod o-t DIR...
```

## 3.4 ACL

ACL：Access Control List 访问控制列表。除了文件原本的权限位设置，可以自定义用户访问控制。

CentOS 7 默认创建的 xfs 和 ext4 文件系统具有 ACL 功能

CentOS 7 之前版本，系统安装时候创建的文件系统有 ACL 功能，默认手工创建的 ext4 文件系统无 ACL 功 能，需手动增加。

可以使用 getfacl 和 setfacl