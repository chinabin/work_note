# 0x00. 导读

# 0x01. 简介

我们知道，工作队列可以用来处理中断的一些耗时的事情，但是他又一个缺点，就是如果有多个事件同时放入工作队列中，如果前面的事件处理得太久，那么就会影响到后面的事件的执行，因为内核线程事一个个依次执行的，如果一个事件真的非常耗时间，那么我们可以用 threaded irq 来处理，大概意思就是中断上半部不变，中断的下半部用一个内核线程来替代 tasklet，这个内核线程专门负责来执行某一个中断。

# 0x02. 

```c
int request_threaded_irq(unsigned int irq, irq_handler_t handler,
                     irq_handler_t thread_fn,
                     unsigned long flags, const char *name, void *dev);
int devm_request_threaded_irq(struct device *dev, unsigned int irq,
                          irq_handler_t handler, irq_handler_t thread_fn,
                          unsigned long irqflags, const char *devname,
                          void *dev_id);
```

它们比request_irq（）、devm_request_irq（）多了一个参数thread_fn。用这两个API申请中断的时候，内核会为相应的中断号分配一个对应的内核线程。注意这个线程只针对这个中断号，如果其他中断也通过request_threaded_irq（）申请，自然会得到新的内核线程。

参数handler对应的函数执行于中断上下文，thread_fn参数对应的函数则执行于内核线程。如果handler结束的时候，返回值是IRQ_WAKE_THREAD，内核会调度对应线程执行thread_fn对应的函数。