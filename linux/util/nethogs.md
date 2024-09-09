# 0x00. 导读

# 0x01. 简介

要安装 NetHogs, 你必须要启用你所用 Linux 下的 EPEL 源. 然后运行下面的 yum 命令下载安装 NetHogs 包.
```bash
$ yum install nethogs
```

# 0x02.

```
  -d : 延迟更新刷新率（以秒为单位）。 默认值为1。
  -v : 视图模式（0 = KB / s，1 =总KB，2 =总B，3 =总MB）。 默认值为0。
  -c : 更新次数。 默认为0（无限制）。
  -s : 按发送列排序输出。
  -a : 监控所有设备，甚至环回/停止。
  device : 要监控的设备。 默认是所有接口启动和运行，不包括环回

当nethogs运行时，按：
  q：退出
  s：按SENT流量排序
  r：按RECEIVE流量排序
  m：在总（KB，B，MB）和 KB/s 模式之间切换
```