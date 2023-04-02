//
// Created by larry on 23-4-2.
//

/**
NAME
       pthread_exit - terminate calling thread

SYNOPSIS
       #include <pthread.h>

       void pthread_exit(void *retval);
       - 功能： 终止一个线程，在那个线程中调用，就表示终止那个线程。
       - 参数： retval: 需要传递一个指针，作为一个返回值，可以在 pthread_join() 中获取到。

NAME
       pthread_self - obtain ID of the calling thread

SYNOPSIS
       #include <pthread.h>

       pthread_t pthread_self(void);

       Compile and link with -pthread.


 NAME
       pthread_equal - compare thread IDs

SYNOPSIS
       #include <pthread.h>

       int pthread_equal(pthread_t t1, pthread_t t2);
       - 功能： 比较两个线程ID是否相等
       不同的操作系统， pthread_t 类型的实现不一样，有的是无符号的长整型，有的是使用结构体去实现。

       Compile and link with -pthread.

*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>

void *callback(void *arg) {
    printf("child thread id: %ld\n", pthread_self());
    return NULL; // pthread_exit(NULL);
}

int main() {
    // 创建一个子线程

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);

    if (ret != 0) {
        char *err_str = strerror(ret);
        printf("errno: %s\n", err_str);
    }

    // main thread
    for (int i = 0; i < 20; i++)
        printf("%d\n", i);


    printf("tid:%ld, main thread id :%ld\n", tid, pthread_self());

    // 让主线程退出，当主线程退出时，不会影响其他正常运行的线程。
    pthread_exit(NULL);

    printf("main thread exit.\n");

    return 0;
}