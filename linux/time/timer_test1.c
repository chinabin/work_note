#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/mutex.h>
#include <linux/timer.h>
/*
 * 执行结果
 * sudo ./app

 * [109127.309807] 
 *                 second open.
 *                 jiffies = 27207252.
 *                 jiffies = 27207502.
 *                 jiffies = 27207752.
 *                 jiffies = 27208002.
 *                 jiffies = 27208253.
 *                 jiffies = 27208503.
 *                 jiffies = 27208753.
 *                 jiffies = 27209003.
 *                 jiffies = 27209253.
*/

#define DEV_NAME    "second"

struct second_dev_t
{
    struct cdev cdev;
    struct class * class;
    dev_t  dev_no;
    struct timer_list timer;
}second_dev;


static void sec_timer_isr( unsigned long arg) 
{
    struct second_dev_t * second_devp  = (struct second_dev_t *)arg;

    printk("\r\n1.jiffies = %d.",jiffies);
    second_devp->timer.expires = jiffies + HZ;
    add_timer(&second_devp->timer);

    printk("\r\n2.jiffies = %d.",jiffies);
}


int second_open(struct inode * inode, struct file * filp)
{
    filp->private_data = &second_dev;

    printk("\r\nsecond open.");
    printk("\r\nsecond open.");

    init_timer( &second_dev.timer );
    second_dev.timer.function = &sec_timer_isr;
    second_dev.timer.data = (unsigned long)(&second_dev);
    second_dev.timer.expires = jiffies + HZ;
    add_timer(&second_dev.timer);

    return 0;
}

ssize_t second_read(struct file *filp, char __user * buf, size_t len, loff_t * pos)
{
    struct second_dev_t * second_devp;
    int ret;

    second_devp = filp->private_data;


    return ret;
}

int second_release(struct inode * inode, struct file * filp)
{
    struct second_dev_t * second_devp = filp->private_data; 

    del_timer(&second_devp->timer);
    return 0;
}


struct file_operations second_fops = {
    .owner = THIS_MODULE,
    .open = second_open,
    .read = second_read,
    .release = second_release,
};

static int __init second_init( void )
{
    int ret;
    
    cdev_init(&second_dev.cdev,&second_fops);
    second_dev.cdev.owner=THIS_MODULE;

    if( (ret=alloc_chrdev_region(&second_dev.dev_no,0,1,DEV_NAME))<0 )
    {
        printk("alloc_chrdev_region err.\r\n");    
        return ret;
    }
    ret = cdev_add(&second_dev.cdev,second_dev.dev_no,1);
    if( ret )
    {
        printk("cdev_add err.\r\n");    
        return ret;
    }

    /*
         * $ sudo insmod second.ko    如果使用class_create,insmod时会报错，dmesg查看内核log信息发现找不到class相关函数
     *   insmod: ERROR: could not insert module second.ko: Unknown symbol in module
     *   $ dmesg   
     *    [ 5495.606920] second: Unknown symbol __class_create (err 0)
     *    [ 5495.606943] second: Unknown symbol class_destroy (err 0)
     *    [ 5495.607027] second: Unknown symbol device_create (err 0)
     */    

    second_dev.class = class_create( THIS_MODULE, DEV_NAME );
    device_create(second_dev.class,NULL,second_dev.dev_no,NULL,DEV_NAME);

    return 0;
}


static void __exit second_exit( void )
{
    unregister_chrdev_region(second_dev.dev_no, 1);
    cdev_del(&second_dev.cdev);
    device_destroy(second_dev.class,second_dev.dev_no);
    class_destroy(second_dev.class);
}


module_init(second_init);
module_exit(second_exit);

MODULE_LICENSE("GPL");    // 不加此声明，会报上述Unknown symbol问题