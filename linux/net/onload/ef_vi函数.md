# 0x00. 导读

# 0x01. 简介

# 0x02.

## 2.1 step 1:  

```c++
ef_driver_handle driver_handle;
ef_vi vi;
// Protection Domain
ef_pd pd;

ef_driver_open(&driver_handle);
ef_pd_alloc(&pd, driver_handle, eth_name, EF_PD_DEFAULT );
// 从 protection domain 分配 virtual interface
ef_vi_alloc_from_pd(&vi, driver_handle, &pd, driver_handle, -1, -1, -1, NULL, -1, 0);
```

## 2.2 step 2:

```c++
// The next step is to allocate a memory region and register it for packet buffers
// BUF_SIZE 就用 2048 ，建议，好用
const int BUF_SIZE = 2048; /* Hardware always wants 2k buffers */
int bytes = N_BUFS * BUF_SIZE;
void* p;
// 因为 ef_memreg_alloc 的倒数第二个参数需要是页对齐，也就是 4k 对齐，所以 posix_memalign 第二个参数是 4096
// 也可以使用 mmap ，其本身就是以页为单位映射
posix_memalign(&p, 4096, bytes) /* allocate aligned memory */
ef_memreg_alloc(&memreg, driver_handle, &pd, driver_handle, p, bytes); /* Make it available to ef_vi */
```

This is all that is strictly necessary to set up the packet buffers.

However, the packet buffer is 2048 bytes long, whereas the normal MTU size for a transmitted packet is only 1500 bytes. There is some spare memory in each packet buffer. Performance can be improved by using this space to cache some of the packet meta-data, so that it does not have to be recalculated:

- The DMA address of the packet is cached. It is determined by getting the DMA address of the base of the memory chunk, and then incrementing in 2KB chunks.

- The packet ID is also cached.

A structure is used to store the cached meta-data and a few pointers in the buffer. An array is used to track all the buffers:

```c++
#define MEMBER_OFFSET(c_type, mbr_name) \
            ((uint32_t) (uintptr_t)(&((c_type*)0)->mbr_name))
#define CACHE_ALIGN __attribute__((aligned(EF_VI_DMA_ALIGN)))

struct pkt_buf {
    struct pkt_buf* next;
    /* We’re not actually going to use this;
    but chaining multiple buffers together is a common and useful trick. */
    ef_addr dma_buf_addr;
    int id;
    uint8_t dma_buf[1] CACHE_ALIGN;
    /* Not strictly required, but cache aligning the payload is a speed
    boost, so do it. */
};

/* We’re also going to want to keep track of all our buffers, so have an
array of them. Not strictly needed, but convenient. */
struct pkt_buf* pkt_bufs [N_BUFS];
for( i = 0; i < N_BUFS; ++i ) {
    struct pkt_buf* pb = (struct pkt_buf*) ((char*) p + i * BUF_SIZE);
    pb->id = i;
    pb->dma_buf_addr = ef_memreg_dma_addr(&memreg, i * BUF_SIZE);
    pb->dma_buf_addr += MEMBER_OFFSET(struct pkt_buf, dma_buf);
    pkt_bufs[i] = pb;
}
```