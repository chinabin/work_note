# 0x00. 导读

# 0x01. 简介

# 0x02. 命令

```bash
Usage: btt 
[ -a               | --seek-absolute ]
[ -A               | --all-data ]
[ -B <output name> | --dump-blocknos=<output name> ]
[ -d <seconds>     | --range-delta=<seconds> ]
[ -D <dev;...>     | --devices=<dev;...> ]
[ -e <exe,...>     | --exes=<exe,...>  ]
[ -h               | --help ]
[ -i <input name>  | --input-file=<input name> ]
[ -I <output name> | --iostat=<output name> ]
[ -l <output name> | --d2c-latencies=<output name> ]
[ -L <freq>        | --periodic-latencies=<freq> ]
[ -m <output name> | --seeks-per-second=<output name> ]
[ -M <dev map>     | --dev-maps=<dev map>
[ -o <output name> | --output-file=<output name> ]
[ -p <output name> | --per-io-dump=<output name> ]
[ -P <output name> | --per-io-trees=<output name> ]
[ -q <output name> | --q2c-latencies=<output name> ]
[ -Q <output name> | --active-queue-depth=<output name> ]
[ -r               | --no-remaps ]
[ -s <output name> | --seeks=<output name> ]
[ -S <interval>    | --iostat-interval=<interval> ]
[ -t <sec>         | --time-start=<sec> ]
[ -T <sec>         | --time-end=<sec> ]
[ -u <output name> | --unplug-hist=<output name> ]
[ -V               | --version ]
[ -v               | --verbose ]
[ -X               | --easy-parse-avgs ]
[ -z <output name> | --q2d-latencies=<output name> ]
[ -Z               | --do-active

常用的是：
[ -i <input name>  | --input-file=<input name> ]
[ -l <output name> | --d2c-latencies=<output name> ]
[ -q <output name> | --q2c-latencies=<output name> ]
```

# 0x03. 输出

## 一、平均时间

第一部分中得到设备的平均延时，单位是秒

```bash
==================== All Devices ====================

            ALL           MIN           AVG           MAX           N
--------------- ------------- ------------- ------------- -----------

Q2Q               0.000000001   0.000067762   4.902880182     1063492
Q2G               0.000000120   0.000000848   0.045202306     1063497
G2I               0.000000374   0.000019883   4.881094326     1063462
I2D               0.000000170   0.000001103   0.000044844     1063462
D2C               0.000067070   0.068852508   4.961615752     1063493
Q2C               0.000080046   0.068874342   4.961619413     1063493
```
说明：

D2C: time the I/O is “active” in the driver and on the device  
Q2C: Total processing time of the I/O；客户发起请求到收到响应的时间

## 二、设备损耗

第二部分得到各个阶段消耗比例得到定性分析

```bash
==================== Device Overhead ====================

       DEV |       Q2G       G2I       Q2M       I2D       D2C
---------- | --------- --------- --------- --------- ---------
 (259,  0) |   0.0012%   0.0289%   0.0000%   0.0016%  99.9683%
---------- | --------- --------- --------- --------- ---------
   Overall |   0.0012%   0.0289%   0.0000%   0.0016%  99.9683%
```

## 三、设备合并信息

```bash

==================== Device Merge Information ====================

       DEV |       #Q       #D   Ratio |   BLKmin   BLKavg   BLKmax    Total
---------- | -------- -------- ------- | -------- -------- -------- --------
 (259,  0) |  1063497  1063497     1.0 |        8        8      256  8511432
```

Q表示传入的IO请求，D表示合并后发出的请求,此外还能看到平均IO块大小为8。

## 四、磁盘寻道讯息

用于显示连续队列和提交IO之间的扇区距离。NSEEKS表示提交到驱动的IO寻道次数，　MEAS表示IO之间距离，MEDIA为０表示向前和向后寻道次数一样，MODE中数值表示块IO中连续的扇区，这部分比较晦涩。

包含Q2Q和D2D两部分，Q2Q是到达的IO请求之间，D2D是驱动中处理的IO.请求
```bash
==================== Device Q2Q Seek Information ====================

       DEV |          NSEEKS            MEAN          MEDIAN | MODE           
---------- | --------------- --------------- --------------- | ---------------
 (259,  0) |         1063493    1042641801.3               0 | 8(29)
---------- | --------------- --------------- --------------- | ---------------
   Overall |          NSEEKS            MEAN          MEDIAN | MODE           
   Average |         1063493    1042641801.3               0 | 8(29)

==================== Device D2D Seek Information ====================

       DEV |          NSEEKS            MEAN          MEDIAN | MODE           
---------- | --------------- --------------- --------------- | ---------------
 (259,  0) |         1063497    1042638814.0               0 | 8(29)
---------- | --------------- --------------- --------------- | ---------------
   Overall |          NSEEKS            MEAN          MEDIAN | MODE           
   Average |         1063497    1042638814.0               0 | 8(29)
```

## 五、请求队列阻塞信息

队列不是无限的的，必然存在队列阻塞情况，这个就是现实队列阻塞，不能被驱动处理。这里的统计信息就是现实不能被处理的比例，如下图：
```bash
==================== Plug Information ====================

       DEV |    # Plugs # Timer Us  | % Time Q Plugged
---------- | ---------- ----------  | ----------------

       DEV |    IOs/Unp   IOs/Unp(to)
---------- | ----------   ----------
 (259,  0) |        0.0          0.0
```

## 六、队列中IO调度

有时候需要关注请求在IO调度上花费的时间。

```bash
==================== Active Requests At Q Information ====================

       DEV |  Avg Reqs @ Q
---------- | -------------
 (259,  0) |           0.3
```
详细数据

使用–all-data或-A可以显示更详细信息。

可以显示每个设备的在各个阶段的统计数据。

## 七、活动数据文件

活动数据文件的格式容易被画图和分析，如下：
```bash
# Total System
#     Total System : q activity
  0.000000000   0.0
  0.000000000   0.4
  3.255723298   0.4
  3.255723298   0.0
  8.137471916   0.0
  8.137471916   0.4
 10.881109703   0.4
 10.881109703   0.0
 15.761898578   0.0
 15.761898578   0.4
 18.934806695   0.4
 18.934806695   0.0
 ...
 ...
```
文件中数据是划分成对的，每对包含队列信息和完成信息。