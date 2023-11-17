# 0x00. 导读

# 0x01. 简介

Provides information about vmalloced/vmaped areas. One line per area, containing the virtual address range of the area, size in bytes, caller information of the creator, and optional information depending on the kind of area:

```
pages=nr	number of pages
phys=addr	if a physical address was specified
ioremap	I/O mapping (ioremap() and friends)
vmalloc	vmalloc() area
vmap	vmap()ed pages
user	VM_USERMAP area
vpages	buffer for pages pointers was vmalloced (huge area)
N<node>=nr	(Only on NUMA kernels) Number of pages allocated on memory node <node>
```

# 0x02. 