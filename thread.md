## 头文件
`#include <thread>`

## 创建线程
构造一个 `thread` 对象，并指定入口函数。此时编译器便会为我们创建一个新的操作系统线程，并在新的线程中执行我们的入口函数。

注意：参数是以拷贝的形式进行传递的。因此对于拷贝耗时的对象你可能需要传递指针或者引用类型作为参数。但是，如果是传递指针或者引用，你还需要考虑参数对象的生命周期。

一旦启动线程之后，我们必须决定是要等待直接它结束（通过 `join` ），还是让它独立运行（通过 `detach` ），我们必须二者选其一。如果在 `thread` 对象销毁的时候我们还没有做决定，则 `thread` 对象在析构函数出将调用 `std::terminate()` 从而导致我们的进程异常退出。
- 需要注意的是：在我们做决定的时候，很可能线程已经执行完了（例如线程的逻辑仅仅是一句打印，执行时间会很短）。新的线程创建之后，究竟是新的线程先执行，还是当前线程的下一条语句先执行这是不确定的，因为这是由操作系统的调度策略决定的。不过这不要紧，我们只要在thread对象销毁前做决定即可。
- `detach` 是让目标线程成为守护线程（daemon threads）。一旦 `detach` 之后，目标线程将独立执行，即便其对应的 `thread` 对象销毁也不影响线程的执行。并且，你无法再与之通信。

## 管理线程
- std::this_thread::yield 让出处理器给其他任务使用。
- std::this_thread::get_id 返回当前线程的id，可以以此来标识不同的线程。
- std::this_thread::sleep_for 是让当前线程停止一段时间。`sleep_for(chrono::seconds(3));`
- std::thread::hardware_concurrency 可以获取到当前硬件支持多少个线程并行执行。




使用的时候需要在 CMakeLists.txt 中链接 `Threads::Threads`:
```
find_package(Threads REQUIRED)
target_link_libraries(LALA PUBLIC Threads::Threads)
```