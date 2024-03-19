#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <sys/prctl.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <math.h>
#include <sys/time.h>
#include <sched.h>
#include <unistd.h> /* sysconf */
#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define TYPE unsigned long
#define arr_size_X  640 //1310720  //5M b//256000
#define arr_size_Y  1
#define STEP_SIZE   4

int mv = -1;

static inline __u64 rdtsc()
{
    __u32 lo,hi;

    __asm__ __volatile__
    (
        "rdtsc":"=a"(lo),"=d"(hi)
    );
    return (__u64)hi<<32|lo;
}

void sig_goon()
{
    mv = 1;
    printf("Go on. mv = %d\n", mv);
}

void myinit(TYPE *arr){
    int i, j;
    TYPE *arr1;
    TYPE *arr_end;

    arr_end = arr + (arr_size_X * arr_size_Y) + 4;

    for (i=0; i < arr_size_Y; i++) {
        for (arr1 = arr + i; arr1 < arr_end; arr1+=arr_size_Y){
            *arr1 = 0 ; //rand() & 0xf;
        }
    }
    for (i=0; i < arr_size_Y; i++) {
        for (arr1 = arr + i; arr1 < arr_end; arr1+=arr_size_Y){
            /*
             * GCC 内置的 clflush
             * CLFLUSH（Cache Line Flush，缓存行刷回）能够把指定缓存行（Cache Line）从所有级
             * 缓存中淘汰，若该缓存行中的数据被修改过，则将该数据写入主存；
            */
            _mm_clflush(arr1);
        }
    }
    return ;
}

int work(TYPE *mem, unsigned int *flagData)
{
    __u64 begin = 0, end = 0, tsc_l3 = 0;
    
    register int i, k = 0;
    unsigned int cur_cpu = 2;
    unsigned long tsc[3];

    cur_cpu = sched_getcpu();

    // // 第一个 CPU 初始化
    if (cur_cpu == flagData[1])
    {
        __asm__ __volatile__
        (
            "lea    %0,     %%r8\n\t"
            "mov    (%%r8), %%rax\n\t"  // TSC0, 将 TSC 低位部分写入 (r8)
            "mov 128(%%r8), %%rax\n\t"  // TSC1, 将 r8 中存放的地址值 + 128 得到新的内存地址，将新的地址指向的内存数据存入 rax
            "mov 256(%%r8), %%rax\n\t"  // TSC2, 将 r8 中存放的地址值 + 256 得到新的内存地址，将新的地址指向的内存数据存入 rax
            :
            :"m"( *mem )
            :"rax","r8"
        );
    }

    // 第二个 CPU 故意消耗一点时间，好让第一个 CPU 就位
    if (cur_cpu == flagData[2])
        for (i=0; i<=flagData[3]; i++)
            k+=i;

    /*
     * rdtsc 指令，会将 TSC 的数值存放在 EDX:EAX 中
    */
    // 第一个 CPU
    if ( cur_cpu == flagData[1] )
    {
        printf("1-----------SUB Process at CPU: %d----------\n", cur_cpu);       
        begin = rdtsc();
        __asm__ __volatile__
        (
            "lea    %0,     %%r8\n\t"       // &mem
            //"mov    (%%r8), %%rax\n\t"      // 11.30添加：1 验证MESI延迟
            //"mov 128(%%r8), %%rax\n\t"       // 12.3添加：2
            "TSC0:\n\t"
                "mov    (%%r8),  %%r9\n\t"         // 取出 r8 中存放的地址所指向的数据，放入 r9
                "cmp    $0,      %%r9\n\t"
                "je TSC0\n\t"               // 如果 r9 == 0 就跳转到 TSC0
            //"movq $0, 128(%%r8)\n\t"         // 11.30添加：1
            "mov $10,  %%rax\n\t"           // rax 存入 10
            "LOOP:\n\t"
                "dec %%rax\n\t"         // rax 减去 1
                "jne LOOP\n\t"          // rax 不为 0 则跳转到 LOOP
            "rdtscp \n\t"
            "movq   %%rax,      128(%%r8)\n\t"
            "movq   %%rdx,      136(%%r8)\n\t"
            //"LOCK add   %%rdx,  8(%%r8)\n\t"
            //"rdtscp \n\t"
            //"movq %%rax, 32(%%r8)\n\t"
            //"movq %%rdx, 40(%%r8)\n\t"
            :
            :"m"( *mem )
            :"rax","rdx","r8","r9"
        );
    }
    else
    {
        printf("2-----------SUB Process at CPU: %d----------\n", cur_cpu);       
        begin = rdtsc();

        __asm__ __volatile__
        (
            "lea    %0,     %%r8\n\t"       // &mem
            //"mov    (%%r8), %%rax\n\t"      // 11.30添加：1 验证MESI延迟
            //"mov 128(%%r8), %%rax\n\t"       // 12.3添加：2
            //"movq   $0,     8(%%r8)\n\t"     // 11.30添加：1
            "rdtscp \n\t"                   // TSC 0
            "mov    %%rax,  (%%r8)\n\t"
            "mov    %%rdx,  8(%%r8)\n\t"
            //"mfence\n\t"
            "TSC1:\n\t"
                "mov    128(%%r8),  %%r9\n\t"      // while load
                "cmp    $0,         %%r9\n\t"
                "je TSC1\n\t"
            "rdtscp \n\t"                   // TSC 3
            "movq   %%rax,      256(%%r8)\n\t"
            "movq   %%rdx,      264(%%r8)\n\t"
            :
            :"m"( *mem )
            :"rax","rdx","r8","r9"
        );
    }

    end = rdtsc();

    if (cur_cpu == flagData[2])
        for (i=0; i<=flagData[3]; i++)
            k+=i;

    printf("TSC: %d \n", (end-begin)); 

    tsc[0] = mem[0];
    tsc[1] = mem[16];
    tsc[2] = mem[32];

    for(i=0; i<3; i++)
    {
        unsigned long t = 0;
        if ( i>0 )
            t = tsc[i] - tsc[i-1];
        printf("TSC %d:%ld  %ld\n", i, tsc[i], t);
    }
    return k;
}

void * smem()
{   
    void *ptr;
    ptr = mmap(0, (arr_size_X * arr_size_Y + 4 ) * sizeof(TYPE), PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS|0x40000, -1, 0); // 大页
    if (ptr == MAP_FAILED)
        printf("Failed to allocate shared memory\n");
    return ptr;
}

void setcpu(int cpu)
{
    int i, nrcpus, cur_cpu;
    cpu_set_t mask;
    unsigned long bitmask = 0;

    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    printf("%x\n", mask);

    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1) 
    {   
        perror("sched_setaffinity fail!");
        exit(EXIT_FAILURE);
    }
}

void main(int argc, char **argv)
{
    TYPE *arr1;
    int k, t, p_stat, cur_cpu, cpu = 0;
    __u64 begin = 0, end = 0;
    unsigned int control_number[4], flag = 1;
    pid_t pid[8];
    struct  timeval    tv1, tv2;

    if (argc < 5) 
    {
        fprintf(stderr, "./mesi start_cpu cpu_id1 cpu_id2 loop_number\n");
        return ;
    }

    control_number[0] = atoi(argv[1]);
    control_number[1] = atoi(argv[2]);
    control_number[2] = atoi(argv[3]);
    control_number[3] = atoi(argv[4]);             // 初始化时的循环次数

    cur_cpu = sched_getcpu();
    printf("Old CPU: %d\n", cur_cpu);       
    setcpu(control_number[0]);                     // 按参数指定CPU进行初始化
    cur_cpu = sched_getcpu();
    printf("Current CPU: %d\n", cur_cpu);       
    
	if(SIG_ERR == signal(SIGUSR1 , sig_goon))
		perror("SIGINT install err\n");

    arr1=(TYPE *)smem();
    if ( arr1 == MAP_FAILED )
    {
        printf("Failed to allocate shared memory\n");
        return ;
    }

    myinit(arr1);

    gettimeofday(&tv1, 0);
    begin = rdtsc();

    cpu = 0;
    flag = 0;
    for (t=0; t<2; t++)
    {
        setcpu( control_number[t + 1] );
        pid[t] = fork();
        if ( pid[t] < 0 ) {
            printf("fork error\n");
        }
        else if (pid[t] == 0)
        {
            // fork 完毕，子进程会进入这里
            k = work(arr1, &control_number[0]);
            return ;
        }
        else {
            // fork 完毕，父进程会进入这里
            ;
        }
        flag = 1;
    }

    printf("==========Parent PID is %d==========\n", getpid());
//    pause();

    for (t=0; t<2 ; t++)
    {
        if ( pid[t] > 0 )
        {
            printf("PID is %d \n", pid[t]);
            waitpid(pid[t], &p_stat, 0);
        }
        else if ( pid[t] < 0 )
            printf("fork error\n");
        printf("============================\n");
    }
    end = rdtsc();
    gettimeofday(&tv2, 0);
    printf("TSC: %f %d\n", (float)(end-begin)/1000/1000.0, k);
    printf("%d.%ld - %d.%ld = %f\n", tv2.tv_sec, tv2.tv_usec, tv1.tv_sec, tv1.tv_usec, (tv2.tv_sec - tv1.tv_sec) * 1000 + ((float)tv2.tv_usec - (float)tv1.tv_usec)/1000);
    munmap(arr1, arr_size_X * arr_size_Y * sizeof(TYPE)); 
    return ;
}
