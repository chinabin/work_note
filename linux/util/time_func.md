# 一、简介

现在通用的标准时间为 Coordinated Universal Time, UTC，由世界上最精确的原子钟提供计时，而本地时间是 UTC+TimeZone，也就是日常使用的时间。

另外，说到 UTC 就不得不提格林威治平均时 Greenwich Mean Time, GMT，这是时区 0 的本地时间，也即是 GMT=UTC+0，所以 GMT 和 UTC 时间值是相等的。

在 *nix 系统中，还有一个词 Epoch，它指的是一个特定时间 1970-01-01 00:00:00 +0000 (UTC)。


----------------

[日期和时间库](https://www.zhihu.com/question/407012589/answer/2618402230)

[222](https://www.zhihu.com/question/21045190/answer/2013345266)


--------------

```c
// time_t 一般为 long int。
// 1970年到现在的秒数，精确到秒。
time_t tm_now;
time(&tm_now);// 或者 tm_now = time(NULL);


//转换成本地时间，精确到秒
struct tm *p_local_tm ;
p_local_tm = localtime(&tm_now) ;
printf("now datetime: %04d-%02d-%02d %02d:%02d:%02d\n",
    p_local_tm->tm_year+1900, 
    p_local_tm->tm_mon+1, 
    p_local_tm->tm_mday, 
    p_local_tm->tm_hour, 
    p_local_tm->tm_min, 
    p_local_tm->tm_sec);
// localtime函数是根据这个秒数和本机的时区，解析出年月日时分秒等信息。
// localtime函数不是多线程安全的，localtime_r才是。


struct timeval tm_now;      // 里面两个成员 (tv_sec, tv_usec) ，秒和微秒
gettimeofday(&tm_now,NULL); // 第二个参数是时区



// 将 tm时间转化为 time_t
time_t mktime(struct tm *tmptr);





struct timespec tm_now;         // 里面两个成员 (tv_sec, tv_nsec) ，秒和纳秒
clock_gettime(CLOCK_MONOTONIC, &tm_now); // 单调时间，屏蔽手动修改时间
clock_gettime(CLOCK_REALTIME, &tm_now);  // 机器时间
```