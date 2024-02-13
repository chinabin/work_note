# 0x00. 导读

# 0x01. 简介

# 0x02. 

```
date [OPTION]... [+FORMAT]
date [-u|--utc|--universal] [MMDDhhmm[[CC]YY][.ss]]
```

- FORMAT 格式符号
    - %F ：年月日全格式，例如 2016-06-21
    - %T ：时间全格式，例如 13:14:42
    - %Y：年
    - %m：月
    - %d：日
    - %H：小时
    - %M  分
    - -%S  秒
    - %s：从 1970 年 1 月 1 日 00:00:00 开始的秒数

- 系统时间设定 date [MMDDhhmm[[CC]YY][.ss]]
    - MM: 月分
    - DD：几号
    - hh: 小时
    - mm: 分钟
    - YY: 两位年份
    - CCYY: 四位年份
    - .ss: 秒钟

```bash
# 设置时间显示格式
[root@centos7 ~]# date +"%F %T"
2017-05-19 20:01:17
[root@centos7 ~]# date +"%Y%m%d"
20170519

# 修改系统时间
[root@centos7 ~]# date 122111112012.11
Fri Dec 21 11:11:11 CST 2012
```