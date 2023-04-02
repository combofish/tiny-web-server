//
// Created by larry on 23-4-2.
//

/**
NAME
       pthread_cancel - send a cancellation request to a thread

SYNOPSIS
       #include <pthread.h>

       int pthread_cancel(pthread_t thread);
       - 功能： 取消线程（让线程终止）
       取消某个线程，可以终止某个线程的运行
       但是并不是立马终止，而是当子线程执行到一个取消点，线程才会终止。
       取消点：系统规定号的一些系统调用，我们可以粗略理解为从用户区到内核区的切换，这个位置称之为取消点。

       Compile and link with -pthread.

*/

#include <stdio.h>
#include <string.h>
#include <pthread.h>

void *callback(void *arg) {
    printf("in child thread: tid: %ld\n", pthread_self());

    for (int i = 0; i < 50; i++)
        printf("child %d\n", i);

    return NULL;
}

int main() {

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char *err_str = strerror(ret);
        printf("err_str: %s\n", err_str);
    }

    // 取消线程
    pthread_cancel(tid);

    for (int i = 0; i < 5; i++)
        printf("%d\n", i);

    printf("pid: %ld, main thread id: %ld\n", tid, pthread_self());

    pthread_exit(NULL);
    return 0;
}