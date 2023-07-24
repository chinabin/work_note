```
# 判断是否出现内核丢包
sudo perf record -g -a -e skb:kfree_skb  
所有的cpu(-a)对调用栈（-g）进行事件（-e：指定性能事件，kfree_skb：因为某种错误报文被丢弃）采样（record：对程序运行过程中的事件进行分析和记录，并写入perf.data）

sudo perf script  
script：读取perf.data(由perf record生成)，生成trace记录，供其他分析工具使用
```