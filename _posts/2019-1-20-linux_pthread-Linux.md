---
layout: post
title: "Linux线程"
date: 2019-1-20
excerpt: "Linux线程控制原语"
tags: [linux系统编程]
comments: true
---
## pthread_create()

```c
#include <pthread.h>
int pthread_create(phtread_t *thread , const pthread_attr_t *attr , 
                  void *(*start_routine)(void*) ,void *arg);
//Compile and link with -lpthread
//return value : success return 0 failed return error number
```

- start_routine函数指针，arg为start_routine的参数

## pthread_self()

```c
#include <pthread.h>
pthread_t pthread_self()
//always success 
```

## pthread_exit

```c
#include <pthread.h>
void pthread_exit(void *retval);
```

- 将指定线程退出

## pthread_join

```c
#include <pthread.h>
int pthread_join (pthread_t thread , void** retval);
//return value
//success: 0 failed : error number
```

- **阻塞等待**的方式回收

## pthread_detach

```c
#include <pthread.h>
int pthread_detach(pthread_t thread)；
//return value
//success: 0 faied : error number
```

- 线程主动与主控线程分开关系。线程结束后，其退出状态不由其他线程获取，自己直接释放。

## pthread_cancel

```c
#include <pthread.h>
int pthread_cancel(pthread_t thread);
//return value
//success: 0 failed : error number
```

- 杀死线程并不是实时的，需要等待线程到达某个取消点。

