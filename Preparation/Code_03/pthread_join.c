//
// Created by larry on 23-4-2.
//
/**
NAME
       pthread_join - join with a terminated thread

SYNOPSIS
       #include <pthread.h>

       int pthread_join(pthread_t thread, void **retval);
       - 功能： 和一个已经终止的线程进行连接。
       回收子线程的资源，
       这个函数是一个阻塞函数，调用一次只能回收一个子线程。
       一般在主线程中使用。

       - 参数： thread 要回收的子线程的ID
       - retval： 接收子线程退出时的返回值。

       - 返回值：成功 0， 失败： errno.

       Compile and link with -pthread.

 */
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int value = 10; // 局部变量

void *callback(void *arg) {
    printf("child thread: tid: %ld\n", pthread_self());
    // sleep(3);
    // return NULL;

    // int value = 10; // 局部变量
    pthread_exit((void *) &value); // return (void*) &value;
}

int main() {
    pthread_t tid;

    int ret = pthread_create(&tid, NULL, callback, NULL);

    if (ret != 0) {
        char *err_str = strerror(ret);
        printf("err_str: %s\n", err_str);
    }

    // main thread
    for (int i = 0; i < 5; i++)
        printf("%d\n", i);

    printf("tid: %ld, main thread tid: %ld\n", tid, pthread_self());

    int *thread_return_value;

    ret = pthread_join(tid, (void *) &thread_return_value);
    if (ret != 0) {
        char *err_str = strerror(ret);
        printf("join err_str:%s\n", err_str);
    }

    printf("exit code:%d\n", *thread_return_value);
    printf("回收子线程资源成功！\n");

    pthread_exit(NULL);


    return 0;
}