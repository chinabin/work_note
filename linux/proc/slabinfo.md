# 0x00. 导读

# 0x01. 简介

# 0x02. 命令

```bash
$ sudo cat /proc/slabinfo
slabinfo - version: 2.1
# name            <active_objs> <num_objs> <objsize> <objperslab> <pagesperslab> : tunables <limit> <batchcount> <sharedfactor> : slabdata <active_slabs> <num_slabs> <sharedavail>
nf_conntrack_3         0      0    320   25    2 : tunables    0    0    0 : slabdata      0      0      0
nf_conntrack_1      1058   1175    320   25    2 : tunables    0    0    0 : slabdata     47     47      0
nfs4_layout_stateid      0      0    296   27    2 : tunables    0    0    0 : slabdata      0      0      0
nfsd4_delegations      0      0    240   17    1 : tunables    0    0    0 : slabdata      0      0      0
nfsd4_files            0      0    288   28    2 : tunables    0    0    0 : slabdata      0      0      0
nfsd4_lockowners       0      0    400   20    2 : tunables    0    0    0 : slabdata      0      0      0
nfsd4_openowners       0      0    440   18    2 : tunables    0    0    0 : slabdata      0      0      0
rpc_inode_cache       25     25    640   25    4 : tunables    0    0    0 : slabdata      1      1      0
xfs_dqtrx              0      0    528   31    4 : tunables    0    0    0 : slabdata      0      0      0
xfs_dquot              0      0    496   16    2 : tunables    0    0    0 : slabdata      0      0      0
xfs_icr              240    240    168   24    1 : tunables    0    0    0 : slabdata     10     10      0
xfs_ili           108470 109273    176   23    1 : tunables    0    0    0 : slabdata   4751   4751      0
xfs_inode         238486 239853    960   17    4 : tunables    0    0    0 : slabdata  14109  14109      0
xfs_efd_item         266    380    424   19    2 : tunables    0    0    0 : slabdata     20     20      0
xfs_da_state          68     68    480   17    2 : tunables    0    0    0 : slabdata      4      4      0
xfs_btree_cur       1800   1890    216   18    1 : tunables    0    0    0 : slabdata    105    105      0
xfs_log_ticket      1673   1782    184   22    1 : tunables    0    0    0 : slabdata     81     81      0
bio-3                782    900    320   25    2 : tunables    0    0    0 : slabdata     36     36      0
kcopyd_job             0      0   3312    9    8 : tunables    0    0    0 : slabdata      0      0      0
dm_uevent              0      0   2608   12    8 : tunables    0    0    0 : slabdata      0      0      0
dm_rq_target_io        0      0    136   30    1 : tunables    0    0    0 : slabdata      0      0      0
fuse_request           0      0    416   19    2 : tunables    0    0    0 : slabdata      0      0      0
fuse_inode             0      0    768   21    4 : tunables    0    0    0 : slabdata      0      0      0
ip6_dst_cache         72     72    448   18    2 : tunables    0    0    0 : slabdata      4      4      0
RAWv6                104    104   1216   26    8 : tunables    0    0    0 : slabdata      4      4      0
UDPLITEv6              0      0   1216   26    8 : tunables    0    0    0 : slabdata      0      0      0
UDPv6                104    104   1216   26    8 : tunables    0    0    0 : slabdata      4      4      0
tw_sock_TCPv6         16     16    256   16    1 : tunables    0    0    0 : slabdata      1      1      0
TCPv6                 60     60   2112   15    8 : tunables    0    0    0 : slabdata      4      4      0
cfq_queue              0      0    232   17    1 : tunables    0    0    0 : slabdata      0      0      0
bsg_cmd                0      0    312   26    2 : tunables    0    0    0 : slabdata      0      0      0
mqueue_inode_cache     18     18    896   18    4 : tunables    0    0    0 : slabdata      1      1      0
hugetlbfs_inode_cache     78     78    608   26    4 : tunables    0    0    0 : slabdata      3      3      0
configfs_dir_cache      0      0     88   46    1 : tunables    0    0    0 : slabdata      0      0      0
dquot                  0      0    256   16    1 : tunables    0    0    0 : slabdata      0      0      0
kioctx               112    112    576   28    4 : tunables    0    0    0 : slabdata      4      4      0
userfaultfd_ctx_cache      0      0    192   21    1 : tunables    0    0    0 : slabdata      0      0      0
dio                   75     75    640   25    4 : tunables    0    0    0 : slabdata      3      3      0
pid_namespace          0      0   2200   14    8 : tunables    0    0    0 : slabdata      0      0      0
posix_timers_cache     64     64    248   16    1 : tunables    0    0    0 : slabdata      4      4      0
UDP-Lite               0      0   1088   30    8 : tunables    0    0    0 : slabdata      0      0      0
flow_cache             0      0    144   28    1 : tunables    0    0    0 : slabdata      0      0      0
UDP                  150    150   1088   30    8 : tunables    0    0    0 : slabdata      5      5      0
tw_sock_TCP          112    112    256   16    1 : tunables    0    0    0 : slabdata      7      7      0
TCP                  225    272   1984   16    8 : tunables    0    0    0 : slabdata     17     17      0
dax_cache             21     21    768   21    4 : tunables    0    0    0 : slabdata      1      1      0
blkdev_integrity       0      0    112   36    1 : tunables    0    0    0 : slabdata      0      0      0
blkdev_queue          26     26   2496   13    8 : tunables    0    0    0 : slabdata      2      2      0
blkdev_ioc           351    351    104   39    1 : tunables    0    0    0 : slabdata      9      9      0
user_namespace         0      0    488   16    2 : tunables    0    0    0 : slabdata      0      0      0
dmaengine-unmap-128    919    960   1088   30    8 : tunables    0    0    0 : slabdata     32     32      0
sock_inode_cache    1113   1225    640   25    4 : tunables    0    0    0 : slabdata     49     49      0
fsnotify_mark_connector   9010   9010     24  170    1 : tunables    0    0    0 : slabdata     53     53      0
net_namespace         18     18   5248    6    8 : tunables    0    0    0 : slabdata      3      3      0
shmem_inode_cache   1671   1752    680   24    4 : tunables    0    0    0 : slabdata     73     73      0
Acpi-State          5406   5406     80   51    1 : tunables    0    0    0 : slabdata    106    106      0
task_delay_info     1878   2160    112   36    1 : tunables    0    0    0 : slabdata     60     60      0
taskstats             96     96    328   24    2 : tunables    0    0    0 : slabdata      4      4      0
proc_inode_cache    5089   7512    672   24    4 : tunables    0    0    0 : slabdata    313    313      0
sigqueue             100    100    160   25    1 : tunables    0    0    0 : slabdata      4      4      0
bdev_cache            76     76    832   19    4 : tunables    0    0    0 : slabdata      4      4      0
kernfs_node_cache  21216  21216    120   34    1 : tunables    0    0    0 : slabdata    624    624      0
mnt_cache          39005  46851    384   21    2 : tunables    0    0    0 : slabdata   2231   2231      0
inode_cache        15984  15984    592   27    4 : tunables    0    0    0 : slabdata    592    592      0
dentry            559531 561918    192   21    1 : tunables    0    0    0 : slabdata  26758  26758      0
iint_cache             0      0    128   32    1 : tunables    0    0    0 : slabdata      0      0      0
avc_xperms_node     6177   6497     56   73    1 : tunables    0    0    0 : slabdata     89     89      0
avc_node            2408   2408     72   56    1 : tunables    0    0    0 : slabdata     43     43      0
selinux_inode_security 194004 194004     40  102    1 : tunables    0    0    0 : slabdata   1902   1902      0
buffer_head       494247 494247    104   39    1 : tunables    0    0    0 : slabdata  12673  12673      0
vm_area_struct     41399  41886    216   18    1 : tunables    0    0    0 : slabdata   2327   2327      0
mm_struct            338    420   1600   20    8 : tunables    0    0    0 : slabdata     21     21      0
fs_cache             768    768     64   64    1 : tunables    0    0    0 : slabdata     12     12      0
files_cache          540    725    640   25    4 : tunables    0    0    0 : slabdata     29     29      0
signal_cache         463    644   1152   28    8 : tunables    0    0    0 : slabdata     23     23      0
sighand_cache        384    420   2112   15    8 : tunables    0    0    0 : slabdata     28     28      0
task_struct          978   1001   4224    7    8 : tunables    0    0    0 : slabdata    143    143      0
cred_jar            1134   1554    192   21    1 : tunables    0    0    0 : slabdata     74     74      0
anon_vma           20500  22083     80   51    1 : tunables    0    0    0 : slabdata    433    433      0
pid                 2016   2016    128   32    1 : tunables    0    0    0 : slabdata     63     63      0
shared_policy_node   3315   3315     48   85    1 : tunables    0    0    0 : slabdata     39     39      0
numa_policy          124    124    264   31    2 : tunables    0    0    0 : slabdata      4      4      0
radix_tree_node    71898  72380    584   28    4 : tunables    0    0    0 : slabdata   2585   2585      0
idr_layer_cache      345    345   2112   15    8 : tunables    0    0    0 : slabdata     23     23      0
dma-kmalloc-8192       0      0   8192    4    8 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-4096       0      0   4096    8    8 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-2048       0      0   2048   16    8 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-1024       0      0   1024   16    4 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-512        0      0    512   16    2 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-256        0      0    256   16    1 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-128        0      0    128   32    1 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-64         0      0     64   64    1 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-32         0      0     32  128    1 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-16         0      0     16  256    1 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-8          0      0      8  512    1 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-192        0      0    192   21    1 : tunables    0    0    0 : slabdata      0      0      0
dma-kmalloc-96         0      0     96   42    1 : tunables    0    0    0 : slabdata      0      0      0
kmalloc-8192          82     96   8192    4    8 : tunables    0    0    0 : slabdata     24     24      0
kmalloc-4096         867    904   4096    8    8 : tunables    0    0    0 : slabdata    113    113      0
kmalloc-2048         532    608   2048   16    8 : tunables    0    0    0 : slabdata     38     38      0
kmalloc-1024       17218  19792   1024   16    4 : tunables    0    0    0 : slabdata   1237   1237      0
kmalloc-512        18840  28736    512   16    2 : tunables    0    0    0 : slabdata   1796   1796      0
kmalloc-256        16241  19648    256   16    1 : tunables    0    0    0 : slabdata   1228   1228      0
kmalloc-192         6697   7476    192   21    1 : tunables    0    0    0 : slabdata    356    356      0
kmalloc-128         6570   8224    128   32    1 : tunables    0    0    0 : slabdata    257    257      0
kmalloc-96         25491  25620     96   42    1 : tunables    0    0    0 : slabdata    610    610      0
kmalloc-64        478555 525312     64   64    1 : tunables    0    0    0 : slabdata   8208   8208      0
kmalloc-32         34560  34560     32  128    1 : tunables    0    0    0 : slabdata    270    270      0
kmalloc-16        206479 207104     16  256    1 : tunables    0    0    0 : slabdata    809    809      0
kmalloc-8           9728   9728      8  512    1 : tunables    0    0    0 : slabdata     19     19      0
kmem_cache_node      320    320     64   64    1 : tunables    0    0    0 : slabdata      5      5      0
kmem_cache           144    144    256   16    1 : tunables    0    0    0 : slabdata      9      9      0
```

第一列是 object name, 每种对象占用总内存量 = num_objs(第三列) * objsize(第四列)

统计Slab占用超过100M的对象：
```bash
$ cat /proc/slabinfo | awk '{if($3*$4/1024/1024 > 100){print $1,$3*$4/1024/1024} }'
```
