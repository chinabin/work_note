# 0x00. 导读

# 0x01. 简介

# 0x02. 

```c
// 条件被认为是非常非常有可能满足时
#define likely(x) __builtin_expect(!!(x), 1)

// 条件非常非常不可能或很难满足时
#define unlikely(x) __builtin_expect(!!(x), 0)
```

## 2.1 处理 Unpredictable Branch

```c
int a, b; // a and b are random
// ...
float c = a > b ? 1.1f : 2.0f;
```

改为

```c
float c_lut[2] = {1.1f , 2.0f};
float c = c_lut[a > b];
```