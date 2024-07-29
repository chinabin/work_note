# 0x00. 导读

显示基于NUMA的进程和操作内存统计信息

# 0x01. 简介

# 0x02. 

```
-c：紧凑的显示信息，并将内存四舍五入到MB单位，如果节点较多，可以使用这个参数
-m：显示每个节点中，系统范围内使用内存的情况，可以与其它参数组合使用：
-n：以原格式显示，但单位为MB
-p：可以指定pid或指定某Node
-s: 进行排序，查看的更直观：
-z: 忽略所有为0的行和列
```

```bash
# 判定是否需要对分配策略进行调整
$ numastat      
                           node0
numa_hit             43480318208
numa_miss                      0
numa_foreign                   0
interleave_hit             25362
local_node           43480318208
other_node                     0

# 当发现 numa_miss 数值比较高时，说明需要对分配策略进行调整。例如将指定进程关联绑定到指定的 CPU 上，从而提高内存命中率。

# 不管是页面数还是内存大小了
# numa_hit: 我在自己节点上成功拿到的内存
# numa_miss: 我去别的节点拿到的内存。每个 numa_miss 事件在另一个节点上都有对应的 numa_foreign 事件
# numa_foreign: 别人在我的节点拿的内存，与对方node的numa_miss相对应

# interleave_hit: 成功分配给此节点的交集策略页面数量。
# local_node: 该节点的进程成功在本节点上分配内存访问的大小
# other_node: 该节点进程在其它节点上分配的内存访问的大小

# 高 numa_hit 值和低 numa_miss 值（相对于彼此）代表优化的性能。
# 注意：miss 和 foreign 的值越高，就考虑 CPU 绑定
```

# 0x03. 经验

```bash
$ watch -d -n1 numastat
```