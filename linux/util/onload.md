# 0x00. 导读

# 0x01. 简介

# 0x02.

# 0x0. 经验之谈

以下报错
```
ERROR: TRY(ef_driver_open(&dh)) failed
ERROR: at /home/zhujun/gitlab/md_api_all/src/McastReceiver.cpp:573
ERROR: rc=-2 errno=2 (No such file or directory)
```

解决：
```
1. 先确认是否有 sf 卡
    sudo sfboot -l
2. 检查是否有 onload
    onload --version
3. lsmod | grep onload 看看是否有程序在使用 onload ，因为需要重新加载 onload 模块
    sudo onload_tool reload
```