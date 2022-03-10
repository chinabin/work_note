# 简介
先举个例子，如何知道看完这篇文章花了多长时间：
1. 首先需要有一个能提供时钟（clock）的设备（电脑、手机、手表、挂钟等）
2. 记录开始阅读和结束阅读的时间点（time_point）
3. 计算两次的时间之差就是阅读持续时间（duration）

稍微正式一点：
1. 有一个时间纪元(也就是一个时间开始的点)，叫做 epoch(例如 1970 年 1 月 1 号 0 时 0 分 0 秒)
2. epoch + 嘀嗒频率，就是 clocks 。例如 A 是一秒嘀嗒一次，B 是 0.000000001 秒嘀嗒一次。
3. 后续的 time point 和 duration 都是基于 clock 。

[官网](https://en.cppreference.com/w/cpp/chrono)


----


## Clocks
> A clock consists of a starting point (or epoch) and a tick rate. 

- system_clock：系统的时钟，系统的时钟可以修改，甚至可以网络对时，因此使用系统时间计算时间差可能不准。
- steady_clock：是固定的时钟，相当于秒表。开始计时后，时间只会增长并且不能修改，适合用于记录程序耗时。
- high_resolution_clock：高精度时钟。

```c++
// now() 当前时钟的 time_point
// to_time_t() time_point 转换成 std::time_t 秒
// from_time_t() 从 std::time_t 转换成 time_point
```


-----


## Time points
> A time point is a duration of time that has passed since the epoch of a specific clock.

`std::chrono::time_point` 表示一个具体时间，如上个世纪80年代、你的生日、今天下午、火车出发时间等。
```c++
template <class Clock, class Duration = typename Clock::duration>  class time_point;

// returns the time point as duration since the start of its clock
time_since_epoch();
```


----


## duration
> A duration consists of a span of time, defined as some number of ticks of some time unit.

`std::chrono::duration` 表示一段时间，比如两个小时，12.88秒，半个时辰，一炷香的时间等等。
```c++
/*
 * Rep表示一种数值类型，例如 int float double ，用来表示Period的数量。
*/
// ratio<1> 可以看出 duration 默认单位是秒
// count() 成员函数返回 Rep 类型的 Period 数量
template <class Rep, class Period = ratio<1> > class duration;
template <intmax_t N, intmax_t D = 1> class ratio;
// 常见定义已经预定义好了，在 std::chrono::
// ratio<3600, 1>          hours
// ratio<60, 1>            minutes
// ratio<1, 1>             seconds
// ratio<1, 1000>          milliseconds
// ratio<1, 1000000>       microseconds
// ratio<1, 1000000000>    nanosecons
```
使用 `duration_cast` 进行转换。