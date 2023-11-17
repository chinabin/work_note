# 0x00. 导读

# 0x01. 简介

用来设置定时任务。

系统任务调度在 `/etc/crontab/` ，用户任务调度在 `/var/spool/cron/`

在 [crontab.guru](https://crontab.guru/) 中输入 crontab 语句，网站将会告诉我们这段语句的含义，以及执行的具体时间，可以用来检验编写的 crontab 语句是否正确

在 [crontab-generator.org](https://crontab-generator.org/) 中可以设定我们想要的运行间隔，然后网页会自动生成对应的 crontab 语句。

crond 服务：
```
/sbin/service crond start    # 启动服务
/sbin/service crond stop     # 关闭服务
/sbin/service crond restart  # 重启服务
/sbin/service crond reload   # 重新载入配置

service crond status

查看crontab服务是否已设置为开机启动，执行命令：
ntsysv

加入开机自动启动：
chkconfig –level 35 crond on
```

新创建的cron job，不会马上执行，至少要过2分钟才执行。如果重启cron则马上执行。

当crontab失效时，可以尝试/etc/init.d/crond restart解决问题。或者查看日志看某个job有没有执行/报错tail -f /var/log/cron。

# 0x02. 详解

如果我们使用 crontab 来定时执行脚本，无法执行，但是如果直接通过命令（如：./test.sh)又可以正常执行，这主要是因为无法读取环境变量的原因。因为我们手动执行某个任务时，是在当前shell环境下进行的，程序当然能找到环境变量，而系统自动执行任务调度时，是不会加载任何环境变量的，因此，就需要在crontab文件中指定任务运行所需的所有环境变量，这样，系统执行任务调度时就没有问题了。

解决方法：

1、所有命令需要写成绝对路径形式，如: /usr/local/bin/docker。

2、通过source命令引入环境变量
```
!/bin/sh
source /etc/profile
. ~/.bash_profile
export RUN_CONF=/home/d139/conf/platform/cbp/cbp_jboss.conf
```

```
    分钟(0-59) 小时(0-24) 日(1-31) 月(1-12）星期（0-7） <要执行的命令>
    f1          f2          f3      f4      f5
1. 星号（*）代表所有可能的值。当 f1 为 * 时表示每分钟都要执行 program，f2 为 * 时表示每小时都要执行程序，其余类推

2. 当 f1 为 a-b 时表示从第 a 分钟到第 b 分钟这段时间内要执行，f2 为 a-b 时表示从第 a 到第 b 小时都要执行，其余类推

3. 正斜线（/）指定时间的间隔频率，例如“0-23/2”表示每两小时执行一次。同时正斜线可以和星号一起使用，例如*/10，如果用在minute字段，表示每十分钟执行一次。当 f1 为 */n 时表示每 n 分钟个时间间隔执行一次，f2 为 */n 表示每 n 小时个时间间隔执行一次，其余类推

4. 当 f1 为 a, b, c,... 时表示第 a, b, c,... 分钟要执行，f2 为 a, b, c,... 时表示第 a, b, c...个小时要执行，其余类推
```
星期天为 0 或者 7

```bash
# 每分钟执行一次命令
* * * * * echo "hello world" >> ~/hello.txt

# 每 5 分钟执行一次命令
*/5 * * * * <要执行的命令>

# 每 15 分钟执行一次命令
*/15 * * * * <要执行的命令>

# 每个小时的27分执行一次命令
# 该定时任务每小时执行一次，每次在27分执行。
27 * * * * <要执行的命令>

# 每个小时的0分、5分、10分分别执行一次
0,5,10 * * * * <要执行的命令>

# 每两小时执行一次命令
0 */2 * * * <要执行的命令>

# 每个工作日（周一到周五）执行一次命令
0 0 * * 1-5 <要执行的命令>

# 每个月10号的16:15执行一次命令
15 16 10 * * <要执行的命令>
```

## 选项

```
-u user：用来设定某个用户的crontab服务；
-e：编辑某个用户的crontab文件内容。如果不指定用户，则表示编辑当前用户的crontab文件。
-l：显示某个用户的crontab文件内容，如果不指定用户，则表示显示当前用户的crontab文件内容。
-r：从/var/spool/cron目录中删除某个用户的crontab文件，如果不指定用户，则默认删除当前用户的crontab文件。
-i：在删除用户的crontab文件时给确认提示。
```

```
# 对 crontab 文件备份
$ crontab -l > $HOME/mycron
```

## 使用标签设置任务

```
@reboot	在系统启动的时候执行一次
@yearly	每年执行一次
@annually	和@yearly相同
@monthly	每个月执行一次
@weekly	每周周星一次
@daily	每天执行一次
@midnight	和@daily相同
@hourly	没消失执行一次

@reboot <要执行的命令>
该定时任务在系统每次启动后执行
```