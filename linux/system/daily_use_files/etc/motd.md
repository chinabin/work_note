motd，全称 Message Of The Day ，是 Linux 中发送问候消息的功能。其消息内容通常存储在 `/etc/motd` 中，在用户登录时显示于终端提示符的上方。

/etc/motd 文件

```

        \\   ^__^
         \\  (oo)\\_______
            (__)\\       )\\\/\\
                ||----w |
                ||     ||
连我机器干哈？
```


## 高端玩法，CentOS7 适用

```
1. sudo chmod 777 /usr/local/bin/dynmotd
2. sudo yum install -y figlet
3. 按照下面的步骤操作

figlet: 将英文字母转换为ASCII字符画
jp2a: 将图片转换为ASCII字符画
asciitable: 输出好看的ASCII表格
```

```
#!/bin/bash

# Installation:
#
# 1.   vim /etc/ssh/sshd_config
#      PrintMotd no
#
# 2.   vim /etc/pam.d/login
#      # session optional pam_motd.so
#
# 3.   vim /etc/profile
#      /usr/local/bin/dynmotd # Place at the bottom
#
# 4.   Then of course drop this file at
#      /usr/local/bin/dynmotd
#

ADMIN='Equim'

USER=`whoami`
FROM=`who am i | grep -Po "(?<=\().*(?=\))"`
HOSTNAME=`uname -n`
IP=`hostname -i`
DISK=`df -Pm | grep simfs | \
    awk '{printf "| %6'\''d ] %-6'\''d ] %-7'\''d", $3, $4, $2 }'`
CPU=`cat /proc/cpuinfo | grep name | cut -f2 -d:`
CPU_MODEL=`echo "${CPU}" | uniq | awk '{$1=$1};1'`
LOGICAL_CPU_COUNT=`echo "${CPU}" | wc -l`

MEMORY=`free -m | grep "Mem" | \
    awk '{printf "| %6'\''d ] %-6'\''d ] %-7'\''d", $3, $7, $2 }'`
SWAP=`free -m | grep "Swap" | \
    awk '{printf "| %6'\''d ] %-6'\''d ] %-7'\''d", $3, $4, $2 }'`
PSA=`ps -Afl | wc -l`
FORMATEDTIME=`date +"%y-%m-%d %T"`

# Time of day
HOUR=$(date +"%H")
if [ $HOUR -lt 12  -a $HOUR -ge 0 ]; then
    TIME="morning"
elif [ $HOUR -lt 17 -a $HOUR -ge 12 ]; then
    TIME="afternoon"
else
    TIME="evening"
fi

# System uptime
uptime=`cat /proc/uptime | cut -f1 -d.`
upDays=$((uptime/60/60/24))
upHours=$((uptime/60/60%24))
upMins=$((uptime/60%60))
upSecs=$((uptime%60))

# System load
LOAD1=`cat /proc/loadavg | awk {'print $1'}`
LOAD5=`cat /proc/loadavg | awk {'print $2'}`
LOAD15=`cat /proc/loadavg | awk {'print $3'}`

# aisatsu
WIDTH=$(tput cols)
AISATSU="------ GOOD $TIME $USER ------"

echo -ne "\e[1;35m"
figlet $HOSTNAME -c -w $WIDTH
echo
echo -ne "\e[1;32m"
printf "%*s\n" $(((${#AISATSU}+$WIDTH)/2)) "$AISATSU"
echo -ne "\e[0m"

COLOR_COLUMN="\e[1m-"
COLOR_VALUE="\e[36m"
RESET_COLORS="\e[0m"
echo -e "
===========================================================================
 $COLOR_COLUMN- Hostname$RESET_COLORS............: $COLOR_VALUE $HOSTNAME $RESET_COLORS
 $COLOR_COLUMN- IP Address$RESET_COLORS..........: $COLOR_VALUE $IP $RESET_COLORS
 $COLOR_COLUMN- Release$RESET_COLORS.............: $COLOR_VALUE `cat /etc/redhat-release` $RESET_COLORS
 $COLOR_COLUMN- CPU model$RESET_COLORS...........: $COLOR_VALUE $CPU_MODEL $RESET_COLORS
 $COLOR_COLUMN- Logical CPU count$RESET_COLORS...: $COLOR_VALUE $LOGICAL_CPU_COUNT $RESET_COLORS
 $COLOR_COLUMN- Users$RESET_COLORS...............: $COLOR_VALUE Currently `users | wc -w` user(s) logged on $RESET_COLORS
 $COLOR_COLUMN- Admin$RESET_COLORS...............: $COLOR_VALUE $ADMIN $RESET_COLORS
=========================================================================== $RESET_COLORS
 $COLOR_COLUMN- System time$RESET_COLORS.........: $COLOR_VALUE $FORMATEDTIME $RESET_COLORS
 $COLOR_COLUMN- System uptime$RESET_COLORS.......: $COLOR_VALUE $upDays days $upHours hours $upMins minutes $upSecs seconds $RESET_COLORS
 $COLOR_COLUMN- Current user$RESET_COLORS........: $COLOR_VALUE $USER (from $FROM) $RESET_COLORS
 $COLOR_COLUMN- Load average$RESET_COLORS........: $COLOR_VALUE $LOAD1, $LOAD5, $LOAD15 $RESET_COLORS
 $COLOR_COLUMN- Memory usage$RESET_COLORS........: $COLOR_VALUE $MEMORY $RESET_COLORS
 $COLOR_COLUMN- Swap usage$RESET_COLORS..........: $COLOR_VALUE $SWAP $RESET_COLORS
 $COLOR_COLUMN- Disk space$RESET_COLORS..........: $COLOR_VALUE $DISK $RESET_COLORS
 $COLOR_COLUMN- Processes$RESET_COLORS...........: $COLOR_VALUE $PSA running $RESET_COLORS
============================================================================
"
```