# 0x00. 导读

# 0x01. 简介

首先在驱动程序分配一页大小的内存，然后用户进程通过 mmap() 将用户空间中大小也为一页的内存映射到内核空间这页内存上。映射完成后，驱动程序往这段内存写10个字节数据，用户进程将这些数据显示出来。

# 0x02. 驱动程序

```c
 #include <linux/miscdevice.h>
 #include <linux/delay.h>
 #include <linux/kernel.h>
 #include <linux/module.h>
 #include <linux/init.h>
 #include <linux/mm.h>
 #include <linux/fs.h>
 #include <linux/types.h>
 #include <linux/delay.h>
 #include <linux/moduleparam.h>
 #include <linux/slab.h>
 #include <linux/errno.h>
 #include <linux/ioctl.h>
 #include <linux/cdev.h>
 #include <linux/string.h>
 #include <linux/list.h>
 #include <linux/pci.h>
 #include <linux/gpio.h>
 
 
 #define DEVICE_NAME "mymap"
 
 
 static unsigned char array[10]={0,1,2,3,4,5,6,7,8,9};
 static unsigned char *buffer;
 
 
 static int my_open(struct inode *inode, struct file *file)
 {
     return 0;
 }
 
 
 static int my_map(struct file *filp, struct vm_area_struct *vma)
 {    
     unsigned long page;
     unsigned char i;
     unsigned long start = (unsigned long)vma->vm_start;
     //unsigned long end =  (unsigned long)vma->vm_end;
     unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);
 
     //得到物理地址
     page = virt_to_phys(buffer);    
     // 进程的地址空间是由若干个vma（struct vm_area_struct）来描述的。当进程执行mmap时，内核会为进程创建一个新的vma，提交给文件或者设备驱动完成mmap。
     //将用户空间的一个 vma 虚拟内存区映射到以 page 开始的一段连续物理页面上
     if(remap_pfn_range(vma         // 虚拟内存区域，即设备地址将要映射到这里 
            , start                 // 虚拟空间的起始地址 
            , page >> PAGE_SHIFT    // 与物理内存对应的页帧号，物理地址右移12位
            , size                  // 映射区域大小，一般是页大小的整数倍
            , PAGE_SHARED))         // 保护属性
         return -1;
 
     //往该内存写10字节数据
     for(i=0;i<10;i++)
         buffer[i] = array[i];
     
     return 0;
 }
 
 
 static struct file_operations dev_fops = {
     .owner    = THIS_MODULE,
     .open    = my_open,
     .mmap   = my_map,
 };
 
 static struct miscdevice misc = {
     .minor = MISC_DYNAMIC_MINOR,
     .name = DEVICE_NAME,
     .fops = &dev_fops,
 };
 
 
 static int __init dev_init(void)
 {
     int ret;    
 
     //注册混杂设备
     ret = misc_register(&misc);
     //内存分配
     buffer = (unsigned char *)kmalloc(PAGE_SIZE,GFP_KERNEL);
     //将该段内存设置为保留
     SetPageReserved(virt_to_page(buffer));
 
     return ret;
 }
 
 
 static void __exit dev_exit(void)
 {
     //注销设备
     misc_deregister(&misc);
     //清除保留
     ClearPageReserved(virt_to_page(buffer));
     //释放内存
     kfree(buffer);
 }
 
 
 module_init(dev_init);
 module_exit(dev_exit);
 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("LKN@SCUT");


```

# 0x03. 应用程序

```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h> 

#define PAGE_SIZE 4096


int main(int argc , char *argv[])
{
    int fd;
    int i;
    unsigned char *p_map;
    
    //打开设备
    fd = open("/dev/mymap",O_RDWR);
    if(fd < 0)
    {
        printf("open fail\n");
        exit(1);
    }

    //内存映射
    p_map = (unsigned char *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0);
    if(p_map == MAP_FAILED)
    {
        printf("mmap fail\n");
        goto here;
    }

    //打印映射后的内存中的前10个字节内容
    for(i=0;i<10;i++)
        printf("%d\n",p_map[i]);
    

here:
    munmap(p_map, PAGE_SIZE);
    return 0;
}


```