# 0x00. 导读

# 0x01. 简介

# 0x02. 

```bash
$ cat /proc/zoneinfo 
Node 0, zone      DMA
  pages free     3977
        min      16
        low      20
        high     24
        scanned  0
        spanned  4095
        present  3998
        managed  3977
    nr_free_pages 3977
    nr_alloc_batch 4
    nr_inactive_anon 0
    nr_active_anon 0
    nr_inactive_file 0
    nr_active_file 0
    nr_unevictable 0
    nr_mlock     0
    nr_anon_pages 0
    nr_mapped    0
    nr_file_pages 0
    nr_dirty     0
    nr_writeback 0
    nr_slab_reclaimable 0
    nr_slab_unreclaimable 0
    nr_page_table_pages 0
    nr_kernel_stack 0
    nr_unstable  0
    nr_bounce    0
    nr_vmscan_write 0
    nr_vmscan_immediate_reclaim 0
    nr_writeback_temp 0
    nr_isolated_anon 0
    nr_isolated_file 0
    nr_shmem     0
    nr_dirtied   0
    nr_written   0
    numa_hit     0
    numa_miss    0
    numa_foreign 0
    numa_interleave 0
    numa_local   0
    numa_other   0
    workingset_refault 0
    workingset_activate 0
    workingset_nodereclaim 0
    nr_anon_transparent_hugepages 0
    nr_free_cma  0
        protection: (0, 2812, 15866, 15866)
  pagesets
    cpu: 0
              count: 0
              high:  0
              batch: 1
  vm stats threshold: 6
    cpu: 1
              count: 0
              high:  0
              batch: 1
  vm stats threshold: 6
    cpu: 2
              count: 0
              high:  0
              batch: 1
  vm stats threshold: 6
    cpu: 3
              count: 0
              high:  0
              batch: 1
  vm stats threshold: 6
  all_unreclaimable: 1
  start_pfn:         1
  inactive_ratio:    1
Node 0, zone    DMA32
  pages free     148388
        min      2991
        low      3738
        high     4486
        scanned  0
        spanned  1044480
        present  782304
        managed  720692
    nr_free_pages 148388
    nr_alloc_batch 748
    nr_inactive_anon 3260
    nr_active_anon 300219
    nr_inactive_file 131376
    nr_active_file 89739
    nr_unevictable 0
    nr_mlock     0
    nr_anon_pages 211340
    nr_mapped    6522
    nr_file_pages 223664
    nr_dirty     2
    nr_writeback 0
    nr_slab_reclaimable 19598
    nr_slab_unreclaimable 6642
    nr_page_table_pages 3290
    nr_kernel_stack 144
    nr_unstable  0
    nr_bounce    0
    nr_vmscan_write 18647
    nr_vmscan_immediate_reclaim 1226
    nr_writeback_temp 0
    nr_isolated_anon 0
    nr_isolated_file 0
    nr_shmem     2466
    nr_dirtied   2521799
    nr_written   1054122
    numa_hit     7321557579
    numa_miss    0
    numa_foreign 0
    numa_interleave 0
    numa_local   7321557579
    numa_other   0
    workingset_refault 267841
    workingset_activate 16020
    workingset_nodereclaim 0
    nr_anon_transparent_hugepages 175
    nr_free_cma  0
        protection: (0, 0, 13054, 13054)
  pagesets
    cpu: 0
              count: 164
              high:  186
              batch: 31
  vm stats threshold: 36
    cpu: 1
              count: 173
              high:  186
              batch: 31
  vm stats threshold: 36
    cpu: 2
              count: 122
              high:  186
              batch: 31
  vm stats threshold: 36
    cpu: 3
              count: 163
              high:  186
              batch: 31
  vm stats threshold: 36
  all_unreclaimable: 0
  start_pfn:         4096
  inactive_ratio:    4
Node 0, zone   Normal
  pages free     638065
        min      13887
        low      17358
        high     20830
        scanned  0
        spanned  3407872
        present  3407872
        managed  3341870
    nr_free_pages 638065
    nr_alloc_batch 1463
    nr_inactive_anon 10720
    nr_active_anon 1414700
    nr_inactive_file 626885
    nr_active_file 392050
    nr_unevictable 0
    nr_mlock     0
    nr_anon_pages 1002487
    nr_mapped    31621
    nr_file_pages 1030982
    nr_dirty     3
    nr_writeback 0
    nr_slab_reclaimable 91087
    nr_slab_unreclaimable 33442
    nr_page_table_pages 16124
    nr_kernel_stack 825
    nr_unstable  0
    nr_bounce    0
    nr_vmscan_write 43723
    nr_vmscan_immediate_reclaim 2115
    nr_writeback_temp 0
    nr_isolated_anon 0
    nr_isolated_file 0
    nr_shmem     11186
    nr_dirtied   11617790
    nr_written   4817137
    numa_hit     33520166649
    numa_miss    0
    numa_foreign 0
    numa_interleave 25331
    numa_local   33520166649
    numa_other   0
    workingset_refault 1167524
    workingset_activate 139555
    workingset_nodereclaim 0
    nr_anon_transparent_hugepages 803
    nr_free_cma  0
        protection: (0, 0, 0, 0)
  pagesets
    cpu: 0
              count: 173
              high:  186
              batch: 31
  vm stats threshold: 48
    cpu: 1
              count: 165
              high:  186
              batch: 31
  vm stats threshold: 48
    cpu: 2
              count: 145
              high:  186
              batch: 31
  vm stats threshold: 48
    cpu: 3
              count: 131
              high:  186
              batch: 31
  vm stats threshold: 48
  all_unreclaimable: 0
  start_pfn:         1048576
  inactive_ratio:    10
```

pages free: 表示这个内存管理区中空闲页面的数量
min：表示这个内存管理区中处于最低警戒水位的页面数量
low：表示这个内存管理区中处于低水位的页面数量
high：表示这个内存管理区中处于高水位的页面数量
spanned：表示这个内存管理区包含的页面数量
present：表示这个内存管理区里实际管理页面的数量
managed：表示这个内存管理区中被伙伴系统管理的页面数量
protection：表示这个内存管理区预留的内存

pagesets：表示每个CPU内存分配器中每个CPU缓存的页面信息。
node_unreclaimable：表示页面回收失败的次数。
start_pfn：表示内存管理区的起始页帧号