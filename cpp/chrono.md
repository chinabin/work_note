# 0x00. 导读

# 0x01. 简介
先举个例子，如何知道看完这篇文章花了多长时间：
1. 首先需要有一个能提供时钟（clock）的设备（电脑、手机、手表、挂钟等）
2. 记录开始阅读和结束阅读的时间点（time_point）
3. 计算两次的时间之差就是阅读持续时间（duration）

稍微正式一点：
1. 有一个时间纪元(也就是一个时间开始的点)，叫做 epoch(例如 1970 年 1 月 1 号 0 时 0 分 0 秒)
2. epoch + 嘀嗒频率，就是 clocks 。例如 A 是一秒嘀嗒一次，B 是 0.000000001 秒嘀嗒一次。
3. 后续的 time point 和 duration 都是基于 clock 。

[cppreference chrono](https://en.cppreference.com/w/cpp/chrono)

C++11 只需要 `#include<chrono>`.

# 0x02. Clocks

> A clock consists of a starting point (or epoch) and a tick rate. 

- system_clock：系统的时钟，系统的时钟可以修改，甚至可以网络对时，因此使用系统时间计算时间差可能不准。
- steady_clock：是固定的时钟，相当于秒表。开始计时后，时间只会增长并且不能修改，适合用于记录程序耗时。
- high_resolution_clock：高精度时钟。

重要函数：
```c++
// 用于获取系统的当前时间
time_point now()
// time_point 转换成 std::time_t 秒
to_time_t() 
// 从 std::time_t 转换成 time_point
from_time_t() 
```


# 0x03. Time points

> A time point is a duration of time that has passed since the epoch of a specific clock.

```c++
/*
 * Clock 用来指定所要使用的时钟：system_clock ，steady_clock 和 high_resolution_clock
 * 第二个模板函数参数用来表示时间的计量单位
 * 从某个 clock 开始，持续了 duration 这么久之后，的时间点
*/
template<class Clock, class Duration = typename Clock::duration>
class time_point;
```

`std::chrono::time_point` 表示一个具体时间，如上个世纪80年代、你的生日、今天下午、火车出发时间等。

重要的函数：
```c++
// 返回 当前时间点 距离 clock 纪元的时间长度
// 例如 经常用来得到当前时间点到 1970 年 1 月 1 日 00:00 的时间距离
duration time_since_epoch();
```

# 0x04. duration

> A duration consists of a span of time, defined as some number of ticks of some time unit.

```c++
/*
 * Rep 表示一种数值类型，例如 int float double ，用来表示 Period 的数量。
 * Period 默认是秒，假如 Rep 是 int ，值为 3 ，则这个 duration 表示 3 秒。
*/
template<class Rep, class Period = ratio<l> >
class duration;
// duration::count() 成员函数返回 Rep 类型的 Period 数量
```

`std::chrono::duration` 表示一段时间，比如两个小时，12.88秒，半个时辰，一炷香的时间等等。

# 0x05. C++11 细节

## 5.1 精度

```c++
template<intmax_t N, intmax_t D = l> class ratio;
// N 表示分子， D 表示分母，默认用秒表示的时间单位。

// 常见定义已经预定义好了，在 std::chrono::
ratio<3600, 1>          hours
ratio<60, 1>            minutes
ratio<1, 1>             seconds
ratio<1, 1000>          milliseconds
ratio<1, 1000000>       microseconds
ratio<1, 1000000000>    nanosecons
```

# 0x06. 例子

```c++
#include <iostream>
#include <chrono>
#include <ctime>
using namespace std;


int main()
{

// 定义毫米级别的时钟类型
typedef chrono::time_point<chrono::system_clock, chrono::milliseconds> milliClockType;

// 获取当前时间点, window system_clock是100纳秒级别(不同系统可能会不一样)，所以要转换
milliClockType tp = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now());

// 转换为ctime,用于打印显示时间
time_t tt = chrono::system_clock::to_time_t(tp);

cout << "now time is : " << ctime(&tt);

// 计算距离1970-1-1 00:00 的时间长度，因为当前时间点定义的精度为毫秒，所以输出的是毫秒
cout << "to 1970-1-1 00:00 ->" << tp.time_since_epoch().count() << "ms" << endl;


return 0;
}
```