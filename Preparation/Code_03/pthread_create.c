/**
NAME
       pthread_create - create a new thread

SYNOPSIS
       #include <pthread.h>

       int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);

       Compile and link with -pthread.

       - 功能： 创建一个子进程
       - 参数： thread: 传出参数，线程创建成功后，子线程的线程ID被写到该变量中。
         - attr: 设置线程的属性，一般使用默认值 NULL。
         - start_routine: 函数指针，这个函数是子线程需要处理的逻辑代码
         - arg: 给第三个参数使用, 传参
       - 返回值：
          - 成功 0
          - 失败：返回错误号，这个错误号和之前的 errno 不太一样。
          获取错误号的信息， char* strerror(int errnum);

 */

#include <pthread.h>
#include<stdio.h>
#include <string.h>
//#include <stdlib.h>
//#include <time.h>
#include <unistd.h>

void *callback(void *arg) {
    printf("child thread...\n");
    printf("arg value : %d\n", *(int *) arg);
    return NULL;
}

int main() {

    pthread_t tid;

    int num = 10;

    // 创建一个子线程
    int ret = pthread_create(&tid, NULL, callback, (void *) &num);

    if (ret != 0) {
        char *err_str = strerror(ret);
        printf("errno: %s\n", err_str);
    }

    for (int i = 0; i < 5; i++)
        printf("%d\n", i);

    sleep(1);

    return 0;
}