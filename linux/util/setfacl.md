# 0x00. 导读

# 0x01. 简介

ACL: Access Control List 访问控制列表。除了文件原本的权限位设置，可以自定义用户访问控制。

# 0x02. 

```bash
# -m 参数，表示 modify 修改的意思

# 给 ops 增加 读写执行 权限
$ setfacl -m u:ops:rwx /opt/share

# 禁止escape用户访问file1
$ setfacl -m u:ops:000 /opt/share
```