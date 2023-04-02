//
// Created by larry on 23-4-2.
//

/**
NAME
       pthread_detach - detach a thread

SYNOPSIS
       #include <pthread.h>

       int pthread_detach(pthread_t thread);
       - 功能： 分离一个线程。被分离的线程在终止的时候，会自动释放资源给系统。
       1、不能多次分离，会产生不可预料的行为。
       2、不能去连接一个已经分离的线程，会报错。

       - 参数：需要分离的线程的ID
       - 返回值： 成功 1， 失败 返回错误号

       Compile and link with -pthread.

*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void *callback(void *arg) {
    printf("child pthread:%ld\n", pthread_self());
    return NULL;
}

int main() {
    pthread_t tid;

    int ret = pthread_create(&tid, NULL, callback, NULL);

    if (ret != 0) {
        char *err_str = strerror(ret);
        printf("err_str %s\n", err_str);
    }

    printf("tid: %ld, main thread tid: %ld\n", tid, pthread_self());

    // 设置子线程分离
    pthread_detach(tid);

    // 设置分离以后，对分离的线程进行连接
    ret = pthread_join(tid, NULL);
    if (ret != 0) {
        char *err_str = strerror(ret);
        printf("detach and then join, with:%s\n", err_str);
    }

    for (int i = 0; i < 5; i++)
        printf("%d\n", i);

    pthread_exit(NULL);


    return 0;
}