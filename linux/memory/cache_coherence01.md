# 0x00. 导读

[Yizhou Shan's Home Page Practical Cache Coherence](http://lastweek.io/notes/cache_coherence/)

# 0x01. 简介

课本上经常解释 MESI 是什么，会涉及到的算法，例如 snoop 和 directory. 但是通常没有继续解释细节：
1. 如何解决冲突？
2. 如果没有 single shared bus 该怎么办？

现在处理器都有 片上网络(Network-on-Chip, NoC), 在 die, cores, cache slices (large LLC is usually divided into slices), memory controllers 等部件的内部，都是通过一个 on-chip network(e.g., a ring in old-generation Xeon, or a mesh in Xeon Scalable) 连接起来的。同时，NoC 有自己的 协议 ，所有的 on-chip components 都通过收发 packets 来通讯，不存在中央协调器（central coordinator）。

这和 cache coherence 意味着什么？

cache requests 由 MESI 协议产生，并且对于所请求的 core 应该是原子性的（收到了我的请求，就必须解决完再处理别人的请求），还有就是 NoC 这个资源本身就带有并发属性（你能用，我也能用），那么这对于一个高性能的 cache coherence 实现就带来了巨大的挑战。

举个例子，Intel 使用 MESIF cache coherence protocol, 实现中，当一个 core 对一个 invalid line 产生 read 访问的时候，这个 cache 会需要产生一个 read transaction(事务) 来从其它 cache 或者内存中获取数据，这个 transaction 会包含很多步骤，例如：发送请求、等待回应、发送 ACK 等。如果同时存在多个 read 和 write transaction 就会产生冲突，那么就必须有人在解决冲突，例如通过不同的 cache controller 解决，或者通过单个串行化的点，例如 agent ，解决。

我们有很多方法来实现 cache coherence transactions.

原子 Read-Modify-Write (RMW) 指令会使得 cache coherence 实现太复杂，这些指令包含了 read-and-inc, test-and-set, and lock;-prefixed 等，感觉它们会依靠一些 “lock the bus”, or “locked state” 来达到目的，总之，原子 RWM 指令会给整体的 transaction 设计带来很多的复杂性。

Directory-based cache coherence protocol and implementation 将是多核机器的未来。因为它比基于 snoop 的流量产生更少的一致性流量，因此更具可扩展性。最近的英特尔 UPI 基于目录的方法证实了这一趋势。


# 0x02. 