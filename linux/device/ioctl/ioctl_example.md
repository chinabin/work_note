1. 声明宏定义及添加头文件

    ```c
    #include <sys/ioctl.h>
    ​​
    typedef enum {
        IO_NONE = 0,
        IO_WRITE = 1,
        IO_READ = 2,
        IOCTL_WRITE_READ = 3,
        IO_MAX
    } IOCTL_NUMBER;   /* IO命令编号枚举 */
    ​
    #define IOCTL_MAGIC       '~'                                        /* 定义一个魔数,随便定义个 */
    #define IOCTL_NONE        _IO(IOCTL_MAGIC, IO_NONE)                  /* 无操作命令 */
    #define IOCTL_WRITE       _IOW(IOCTL_MAGIC, IO_WRITE, unsigned char[128])  /* 写数据命令 */
    #define IOCTL_READ        _IOR(IOCTL_MAGIC, IO_READ, unsigned char[128])   /* 读数据命令 */
    #define IOCTL_WRITE_READ  _IOWR(IOCTL_MAGIC, IOCTL_WRITE_READ, unsigned char[128])  /* 读写数据命令 */
    ```

2. 函数实现

    ```c
    /**************************************************************************************************
    **  函数名称:  hello_ioctl
    **  功能描述:  应用层调用ioctl函数时同步调用此函数
    **  输入参数:   *file:文件结构体
    **           cmd:IO操作的指令
    **           arg:传递参数的指针地址（用户空间 的）
    **  输出参数:  无
    **  返回参数:  成功返回0,失败返回-1
    **************************************************************************************************/
    long hello_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
    {
        void __user *ptr; /* 数据地址指针 */
        int arg_len;      /* 数据长度 */
        long ret;
        
        ptr =  (void __user*) arg;        /* 强转用户空间数据地址 */
        
        if (_IOC_TYPE(cmd) != IOCTL_MAGIC) {      /* 检查魔数 */
            PRINT("[KERNEL]:%s, MAGIC is not match !\n", __FUNCTION__);
            return -1; 
        }
        
        if (_IOC_NR(cmd) > IO_MAX) {      /* 检查编号 */
            PRINT("[KERNEL]:%s, number over IO_MAX !\n", __FUNCTION__);
            return -1;
        }
    ​
        arg_len = _IOC_SIZE(cmd);
        
        switch (cmd) {
            case IOCTL_NONE:          /* 无操作命令 */
                PRINT("[KERNEL]:%s ------ IOCTL_NONE\n", __FUNCTION__);
                ret = 0;
                break;
            case IOCTL_WRITE:             /* 写数据命令 */
                PRINT("[KERNEL]:%s ------ IOCTL_WRITE\n", __FUNCTION__);
                ret = ioctl_write(ptr, arg_len);
                break;
            case IOCTL_READ:              /* 读数据命令 */
                PRINT("[KERNEL]:%s ------ IOCTL_READ\n", __FUNCTION__);
                ret = ioctl_read(ptr, arg_len);
                break;
            case IOCTL_WRITE_READ:        /* 读写数据命令 */
                PRINT("[KERNEL]:%s ------ IOCTL_WRITE_READ\n", __FUNCTION__);
                ret = ioctl_write_read(ptr, arg_len);
                break;
    ​
            default:
                ret = -1;
                break;
        }
    ​
        return ret;
    }
    ```

    - 参数 arg 为应用层传递过来的参数数据指针，属于用户空间地址，需要使用 copy_to_user 或 copy_from_user 等函数进行读写。  
    - 根据命令进行分类，分发到不同函数中进行数据处理。

3. 将函数指针赋值给 file_operations 结构体。

    ```c
    static struct file_operations hello_fops = {
        .owner = THIS_MODULE,
        .open = hello_open,
        .release = hello_release,
        .read = hello_read,
        .write = hello_write,
        .unlocked_ioctl = hello_ioctl,
    };          
    ```

4. 写命令处理函数

    ```c
    /**************************************************************************************************
    **  函数名称:  ioctl_write
    **  功能描述:  写命令执行函数
    **  输入参数:   *ptr:用户空间数据地址
    **           len:数据长度
    **  输出参数:  无
    **  返回参数:  成功返回0,失败返回-1
    **************************************************************************************************/
    static long ioctl_write(void __user *ptr, int len)
    {
        if (0 == access_ok(VERIFY_WRITE, ptr, len)) {    /* 检查用户空间内存块是否可用 */
            return -1;
        }
        
        memset(test_buff, 0, sizeof(test_buff));
        
        if (0 != copy_from_user(test_buff, ptr, sizeof(test_buff))) {
            PRINT("[KERNEL]:ERROR: %s write error!\n\n", __FUNCTION__);
            return -1;
        }
        
        PRINT("[KERNEL]:%s ------ test_buff = %s\n", __FUNCTION__, test_buff);
    ​
        return 0;
    }
    ```

5. 读命令处理函数

    ```c
    /**************************************************************************************************
    **  函数名称:  ioctl_read
    **  功能描述:  读命令执行函数
    **  输入参数:   *ptr:用户空间数据地址
    **           len:数据长度
    **  输出参数:  无
    **  返回参数:  成功返回0,失败返回-1
    **************************************************************************************************/
    static long ioctl_read(void __user *ptr, int len)
    {
        if (0 == access_ok(VERIFY_READ, ptr, len)) {    /* 检查用户空间内存块是否可用 */
            return -1;
        }
        
        strcat(test_buff, " from kernel!");
    ​
        PRINT("[KERNEL]:%s test_buff = %s\n", __FUNCTION__, test_buff);
        
        if (0 != copy_to_user(ptr, test_buff, sizeof(test_buff))) {
            PRINT("[KERNEL]:ERROR: %s write error!\n\n", __FUNCTION__);
            return -1;
        }
    ​
        return 0;
    }
    ```

6. 读写命令处理函数

    ```c
    /**************************************************************************************************
    **  函数名称:  ioctl_write_read
    **  功能描述:  读写命令执行函数
    **  输入参数:   *ptr:用户空间数据地址
    **           len:数据长度
    **  输出参数:  无
    **  返回参数:  成功返回0,失败返回-1
    **************************************************************************************************/
    static long ioctl_write_read(void __user *ptr, int len)
    {
        if (0 == access_ok(VERIFY_WRITE, ptr, len)) {    /* 检查用户空间内存块是否可用 */
            return -1;
        }
    ​
        /* 读取应用层写入的数据 */
        memset(test_buff, 0, sizeof(test_buff));
        
        if (0 != copy_from_user(test_buff, ptr, sizeof(test_buff))) {
            PRINT("[KERNEL]:ERROR: %s write error!\n\n", __FUNCTION__);
            return -1;
        }
        
        PRINT("[KERNEL]:%s ------ test_buff = %s\n", __FUNCTION__, test_buff);
    ​
        
        /* 向应用层传输数据 */
        memset(test_buff, 0, sizeof(test_buff));
        sprintf(test_buff, "%s", "It is changed by kernel driver!");
        
        PRINT("[KERNEL]:%s test_buff = %s\n", __FUNCTION__, test_buff);
        
        if (0 != copy_to_user(ptr, test_buff, sizeof(test_buff))) {
            PRINT("[KERNEL]:ERROR: %s write error!\n\n", __FUNCTION__);
            return -1;
        }
    ​
        return 0;
    }
    ```

7. 应用编码内容

    ```c
    #include <stdio.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <string.h>
    #include <sys/ioctl.h>

    /*************************************************************************************************/
    //                           局部宏定义
    /*************************************************************************************************/
    #define IOCTL_MAGIC      '~'                                        /* 定义一个魔数,随便定义个 */
    #define IOCTL_NONE       _IO(IOCTL_MAGIC, 0)                        /* 无操作命令 */
    #define IOCTL_WRITE      _IOW(IOCTL_MAGIC, 1, unsigned char[128])   /* 写数据命令 */
    #define IOCTL_READ       _IOR(IOCTL_MAGIC, 2, unsigned char[128])   /* 读数据命令 */
    #define IOCTL_WRITE_READ _IOWR(IOCTL_MAGIC, 3, unsigned char[128])  /* 读写数据命令 */

    #define DELAY50         usleep(50 * 1000);     /* 延迟50ms */

    int main(int argc,char **argv)
    {
        int ret;   /* 读写返回值 */
        int handle;                                /* 文件标识符 */
        char str[128];                             /* 测试用数组 */

        handle = open("/dev/hello", O_RDWR);       /* 打开驱动设备 */
        
        if (handle == -1) {
            printf("open /dev/hello fail!\n");
            return 0;
        }
        
        DELAY50
        
        ret = ioctl(handle, IOCTL_NONE, str);               /* 发送无数据命令给驱动 */
        DELAY50
        printf("[user]:IOCTL_NONE, ret = %d\n", ret);       /* 无数据命令执行结果 */
        DELAY50

        memset(str, 0, sizeof(str));
        sprintf(str, "%s", "ABCDEFG");                      /* 填写测试的内容 */
        ret = ioctl(handle, IOCTL_WRITE, str);              /* 发送带数据的命令给驱动 */
        DELAY50
        printf("[user]:IOCTL_WRITE, ret = %d, str = %s\n", ret, str);              /* 写结果 */
        DELAY50
        
        memset(str, 0, sizeof(str));
        ret = ioctl(handle, IOCTL_READ, str);               /* 发送读数据的命令给驱动 */
        DELAY50
        printf("[user]:IOCTL_READ, ret = %d, str = %s\n", ret, str);               /* 读结果 */
        DELAY50
        
        memset(str, 0, sizeof(str));
        sprintf(str, "%s", "QWERT123456");                  /* 填写测试的内容 */
        ret = ioctl(handle, IOCTL_WRITE_READ, str);         /* 发送带数据的命令给驱动,并从驱动读取数据出来 */
        DELAY50
        printf("[user]:IOCTL_WRITE_READ, ret = %d, str = %s\n", ret, str);         /* 读结果 */
        DELAY50
        
        close(handle);

        return 0;
    }
    ```

8. 测试

```bash
$ ./hello 
[24853.936864] [KERNEL]:hello_open ------ 
[24853.990864] [KERNEL]:hello_ioctl ------ IOCTL_NONE
[user]:IOCTL_NONE, ret = 0
[24854.096308] [KERNEL]:hello_ioctl ------ IOCTL_WRITE
[24854.101198] [KERNEL]:ioctl_write ------ test_buff = ABCDEFG
[user]:IOCTL_WRITE, ret = 0, str = ABCDEFG
[24854.207285] [KERNEL]:hello_ioctl ------ IOCTL_READ
[24854.212089] [KERNEL]:ioctl_read test_buff = ABCDEFG from kernel!
[user]:IOCTL_READ, ret = 0, str = ABCDEFG from kernel!
[24854.318376] [KERNEL]:hello_ioctl ------ IOCTL_WRITE_READ
[24854.323721] [KERNEL]:ioctl_write_read ------ test_buff = QWERT123456
[24854.330081] [KERNEL]:ioctl_write_read test_buff = It is changed by kernel driver!
[user]:IOCTL_WRITE_READ, ret = 0, str = It is changed by kernel driver!
[24854.437848] [KERNEL]:hello_release ------ 
```