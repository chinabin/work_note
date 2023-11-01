# 0x00. 导读

# 0x01. 简介

request: 一个 request 中包含了一个或多个 bio ，为什么要有 request 这个结构呢？它存在的目的就是为了进行 io 的调度。通过 request 这个辅助结构，我们来给 bio 进行某种调度方法的排序，从而最大化地提高磁盘访问速度。

request_queue: 每个磁盘对应一个 request_queue. 该队列挂的就是 request 请求。

# 0x02. 

块设备使用请求队列来保存等待中的 block I/O 请求，其使用 request_queue 结构来表示，定义在 `<linux/blkdev.h>` 头文件中，此头文件中还包含了非常重要的 request 结构：

```c
struct request {
    struct list_head queuelist;
    struct call_single_data csd;
 
    struct request_queue *q;
 
    unsigned int cmd_flags;
    enum rq_cmd_type_bits cmd_type;
    unsigned long atomic_flags;
 
    int cpu;
 
    /* the following two fields are internal, NEVER access directly */
    unsigned int __data_len;    /* total data len */
    sector_t __sector;      /* sector cursor */
 
    struct bio *bio;
    struct bio *biotail;
 
    struct hlist_node hash; /* merge hash */
    /*
     * The rb_node is only used inside the io scheduler, requests
     * are pruned when moved to the dispatch queue. So let the
     * completion_data share space with the rb_node.
     */
    union {
        struct rb_node rb_node; /* sort/lookup */
        void *completion_data;
    };
 
    /*
     * two pointers are available for the IO schedulers, if they need
     * more they have to dynamically allocate it.
     */
    void *elevator_private;
    void *elevator_private2;
 
    struct gendisk *rq_disk;
    unsigned long start_time;
 
    /* Number of scatter-gather DMA addr+len pairs after
     * physical address coalescing is performed.
     */
    unsigned short nr_phys_segments;
 
    unsigned short ioprio;
 
    int ref_count;
 
    void *special;      /* opaque pointer available for LLD use */
    char *buffer;       /* kaddr of the current segment if available */
 
    int tag;
    int errors;
 
    /*
     * when request is used as a packet command carrier
     */
    unsigned char __cmd[BLK_MAX_CDB];
    unsigned char *cmd;
    unsigned short cmd_len;
 
    unsigned int extra_len; /* length of alignment and padding */
    unsigned int sense_len;
    unsigned int resid_len; /* residual count */
    void *sense;
 
    unsigned long deadline;
    struct list_head timeout_list;
    unsigned int timeout;
    int retries;
 
    /*
     * completion callback.
     */
    rq_end_io_fn *end_io;
    void *end_io_data;
 
    /* for bidi */
    struct request *next_rq;
};
 
struct request_queue
{
    /*
     * Together with queue_head for cacheline sharing
     */
    struct list_head    queue_head;
    struct request      *last_merge;
    struct elevator_queue   *elevator;
 
    /*
     * the queue request freelist, one for reads and one for writes
     */
    struct request_list rq;
 
    request_fn_proc     *request_fn;
    make_request_fn     *make_request_fn;
    prep_rq_fn      *prep_rq_fn;
    unplug_fn       *unplug_fn;
    merge_bvec_fn       *merge_bvec_fn;
    prepare_flush_fn    *prepare_flush_fn;
    softirq_done_fn     *softirq_done_fn;
    rq_timed_out_fn     *rq_timed_out_fn;
    dma_drain_needed_fn *dma_drain_needed;
    lld_busy_fn     *lld_busy_fn;
 
    /*
     * Dispatch queue sorting
     */
    sector_t        end_sector;
    struct request      *boundary_rq;
 
    /*
     * Auto-unplugging state
     */
    struct timer_list   unplug_timer;
    int         unplug_thresh;  /* After this many requests */
    unsigned long       unplug_delay;   /* After this many jiffies */
    struct work_struct  unplug_work;
 
    struct backing_dev_info backing_dev_info;
 
    /*
     * The queue owner gets to use this for whatever they like.
     * ll_rw_blk doesn't touch it.
     */
    void            *queuedata;
 
    /*
     * queue needs bounce pages for pages above this limit
     */
    gfp_t           bounce_gfp;
 
    /*
     * various queue flags, see QUEUE_* below
     */
    unsigned long       queue_flags;
 
    /*
     * protects queue structures from reentrancy. ->__queue_lock should
     * _never_ be used directly, it is queue private. always use
     * ->queue_lock.
     */
    spinlock_t      __queue_lock;
    spinlock_t      *queue_lock;
 
    /*
     * queue kobject
     */
    struct kobject kobj;
 
    /*
     * queue settings
     */
    unsigned long       nr_requests;    /* Max # of requests */
    unsigned int        nr_congestion_on;
    unsigned int        nr_congestion_off;
    unsigned int        nr_batching;
 
    void            *dma_drain_buffer;
    unsigned int        dma_drain_size;
    unsigned int        dma_pad_mask;
    unsigned int        dma_alignment;
 
    struct blk_queue_tag    *queue_tags;
    struct list_head    tag_busy_list;
 
    unsigned int        nr_sorted;
    unsigned int        in_flight[2];
 
    unsigned int        rq_timeout;
    struct timer_list   timeout;
    struct list_head    timeout_list;
 
    struct queue_limits limits;
 
    /*
     * sg stuff
     */
    unsigned int        sg_timeout;
    unsigned int        sg_reserved_size;
    int         node;
#ifdef CONFIG_BLK_DEV_IO_TRACE
    struct blk_trace    *blk_trace;
#endif
    /*
     * reserved for flush operations
     */
    unsigned int        ordered, next_ordered, ordseq;
    int         orderr, ordcolor;
    struct request      pre_flush_rq, bar_rq, post_flush_rq;
    struct request      *orig_bar_rq;
 
    struct mutex        sysfs_lock;
 
#if defined(CONFIG_BLK_DEV_BSG)
    struct bsg_class_device bsg_dev;
#endif
};
```

request_queue 中的很多成员和 I/O 调度器、request、bio 等息息相关。

request_queue 中的 queue_head 成员为请求的双向链表。  
nr_requests 为请求的数量。I/O 请求被文件系统等上层的代码加入到队列中，只要队列不为空，block 设备驱动程序就需要从队列中抓取请求并提交到对应的块设备中。这个队列中的就是单独的请求，以 request 结构来表示。

每个 request 结构又可以由多个 bio 组成，一个 request 中放着顺序排列的 bio（请求在多个连续的磁盘块上）。

实际上在 request_queue 中，只有当请求队列有一定数目的请求时，I/O 调度算法才能发挥作用，否则极端情况下它将退化成 “先来先服务算法”，这就悲催了。通过对 request_queue 进行 plug 操作相当于停用，unplug 相当于恢复。请求少时将 request_queue 停用，当请求达到一定数目，或者 request_queue 里最 “老” 的请求已经等待一段时间了才将 request_queue 恢复，这些见 request_queue 中的 unplug_fn、unplug_timer、unplug_thresh、unplug_delay 等成员。