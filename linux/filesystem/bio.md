# 0x00. 导读

# 0x01. 简介

bio, 代表了一个 io 请求。

# 0x02. 

`bio` 结构体是内核中块 I/O 的基本容器，定义在 `<linux/bio.h>` 中。该结构将正在活动的块 I/O 操作表示为 segment list ， segment 是内存中连续的缓冲区块（请注意，不同段不一定是连续的），通过允许以块的形式描述缓冲区， bio 结构为内核提供了从内存中的多个位置执行单个缓冲区的块 I/O 操作的能力。

`bio` 结构中需要一些缓冲内存来保存运往或者来自块层的数据，这些缓冲内存就被称为 `segment` 。段是内存中连续的缓冲区块，但是不同的段是不一定连续的，这样就产生了段列表。

在内核中有两种方式分配内存：

- kmalloc
- vmalloc

bio 的每一段的物理地址是连续的，所以是使用的 kmalloc() 的分配方式。

```c
struct bio {
    struct bio          *bi_next;   /* request queue link */
    struct block_device *bi_bdev;
    unsigned int        bi_flags;   /* status, command, etc */
    int                 bi_error;
    unsigned long       bi_rw;      /* 末尾 bit 表示 READ/WRITE,
                                        * 起始 bit 表示优先级
                                        */
    struct bvec_iter    bi_iter;

    /* 当完成物理地址合并之后剩余的段的数量 */
    unsigned int        bi_phys_segments;

    /*
        * To keep track of the max segment size, we account for the
        * sizes of the first and last mergeable segments in this bio.
        */
    unsigned int        bi_seg_front_size;
    unsigned int        bi_seg_back_size;

    /* 关联 bio 的数量 */
    atomic_t            __bi_remaining;
    bio_end_io_t        *bi_end_io;
    void                *bi_private;
    unsigned short      bi_vcnt;    /* how many bio_vec's, bi_io_vec 所指向的 bio_vec 类型列表个数 */
    unsigned short      bi_idx;     /* current index into bvl_vec, 为当前在 bi_io_vec 数组中的索引 */

    /*
        * Everything starting with bi_max_vecs will be preserved by bio_reset()
        */
    unsigned short      bi_max_vecs;    /* max bvl_vecs we can hold */
    /* 当前 bio 的引用计数，当该数据为 0 时才可以 free */
    atomic_t            __bi_cnt;       /* pin count */
    struct bio_vec      *bi_io_vec;     /* the actual vec list , 表示指定的 block I/O 操作中的单独的段*/
    struct bio_set      *bi_pool;

    /*
        * We can inline a number of vecs at the end of the bio, to avoid
        * double allocations for a small number of bio_vecs. This member
        * MUST obviously be kept at the very end of the bio.
        * 表示跟在 bio 后面的数据集合
        */
    struct bio_vec      bi_inline_vecs[0];
};

```

```c
struct bvec_iter {
    sector_t        bi_sector;      /* device address in 512 byte sectors */
    unsigned int    bi_size;        /* residual I/O count */
    unsigned int    bi_idx;         /* current index into bvl_vec */
    unsigned int    bi_bvec_done;   /* number of bytes completed in current bvec */
};
```

- bi_iter.bi_sector字段指示了整个BIO的起始扇区，且并没有针对该字段的访问方法。
- bi_iter.bi_size字段指示了请求操作的总大小（以字节为单位），也可以通过 bio_sectors(struct bio *bio) 方法获得以扇区为单位的大小。
- bi_rw字段指示了请求的类型（READ 还是 WRITE），也可以通过 int bio_data_dir(struct bio *bio) 方法来获取其值。
- 可以通过struct page* bio_page(struct bio *bio) 方法来返回一个当前struct page的指针。

`bi_inline_vecs` 是数据所在部分，相当于这个结构体描述的都只是元数据部分，实际数据都包含在紧跟其后的 bio_vec 中，那么这个 bio_vec 何许人也？

```c
struct bio_vec {
    struct page     *bv_page;
    unsigned int    bv_len;
    unsigned int    bv_offset;
};
```

`bio_vec` 是组成 `bio` 的最小单位，包含了一块数据所在的页，这块数据所在的页内偏移以及长度，通过这些信息就可以很清晰的描述数据具体位于什么位置。

![Alt text](../../../pic/linux/filesystem/struct_bio.png)

![Alt text](../../../pic/linux/filesystem/struct_bio2.png)


# 0x03. 

一个 bio 结构体的缓冲块是不连续的，而下盘时的读写操作总是在连续的扇区上进行，这被称为向量 IO(vector I/O) ，也称为 scatter-gather I/O 。

分散/聚集 I/O是一种可以在单次系统调用中对多个缓冲区输入输出的方法，可以把多个缓冲区的数据写到单个数据流，也可以把单个数据流读到多个缓冲区中。其命名的原因在于数据会被分散到指定缓冲区向量，或者从指定缓冲区向量中聚集数据。

那么，如果一个bio需要将多个缓冲区写到磁盘不连续的位置，该怎么办呢？

就需要拆分bio了。