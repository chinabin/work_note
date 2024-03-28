# 0x00. 导读

# 0x01. 简介

[github mtools](https://github.com/troglobit/mtools)

> msend - send UDP messages to a multicast group
> 
> mreceive - receive UDP multicast messages and display them

# 0x02. 

```
-g GROUP
Specify the IP multicast group address to which packets are sent, or received.

-p PORT
Specify the UDP port number used by the multicast group.

-c NUM
Number of packets to send, default: unlimited.

-i ADDRESS
Specify the IP address of the interface to be used to send the packets.
用来收发数据的 ip ，因为可能存在多个网卡

-I INTERFACE
Specify the interface to send on(转寄，转送). Can be specified as an alternative to -i.

-q
Quiet mode, don't print sending or receiving messages. Errors are still printed.

-n
Interpret the contents of the message text as a number instead of a string of characters. Use mreceive -n on the other end to interpret the message text correctly.
```

## 2.1 msend 

msend continuously sends UDP packets to the multicast group specified by the -g and -p options.

```
msend [-46hnv] [-g GROUP] [-p PORT] [-join] [-t TTL] [-i ADDRESS] [-P PERIOD]
      [-I INTERFACE] [-c NUM] [-text "text"]

-P PERIOD
Specify the interval in milliseconds between two transmitted packets. The default value is 1000 milliseconds.

-text "text"
Specify a message text which is sent as the payload of the packets and is displayed by the mreceive(8) command. The default value is an empty string.

-join
Multicast sender will join join the multicast group. By default, a multicast sender does not join the group.
```

## 2.2 mreceive 

mreceive joins a multicast group specified by the -g and -p options, then receives and displays the multicast packets sent to this group:port combination by the msend command.

```bash
$ ./mreceive -g 组播地址 -p 组播端口 -i localip
```

```
mreceive [-46hnv] [-g group] [-p port] [-i ip] ... [-i ip] [-I INTERFACE]
```