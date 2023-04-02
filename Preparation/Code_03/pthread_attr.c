//
// Created by larry on 23-4-2.
//
/**
NAME
       pthread_attr_init, pthread_attr_destroy - initialize and destroy thread attributes object

SYNOPSIS
       #include <pthread.h>

       int pthread_attr_init(pthread_attr_t *attr);
       - 功能： 初始化线程属性变量

       int pthread_attr_destroy(pthread_attr_t *attr);
       - 释放线程属性资源

       Compile and link with -pthread.

NAME
       pthread_attr_setdetachstate, pthread_attr_getdetachstate - set/get detach state attribute in thread attributes object

SYNOPSIS
       #include <pthread.h>

       int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
       - 设置线程分离的状态属性

       int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
       - 获取线程分离的状态属性

       Compile and link with -pthread.
 */


#include <stdio.h>
#include <pthread.h>
#include <string.h>

void *callback(void *arg) {
    printf("in child thread: %ld", pthread_self());
    return NULL;
}

int main() {
    // 创建一个线程属性变量
    pthread_attr_t attr;

    // 初始化线程属性变量
    pthread_attr_init(&attr);

    // 设置属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t tid;
    int ret = pthread_create(&tid, &attr, callback, NULL);
    if (ret != 0) {
        char *str_err = strerror(ret);
        printf("str_err: %s", str_err);
    }

    // 获取线程栈的大小
    size_t sz;
    pthread_attr_getstacksize(&attr, &sz);
    printf("get stack size: %ld\n", sz); // 8388608
    printf("tid: %ld， main thread id:%ld", tid, pthread_self());

    // 释放线程属性资源
    pthread_attr_destroy(&attr);

    pthread_exit(NULL);

    return 0;
}