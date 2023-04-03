//
// Created by larry on 23-4-3.
//

/**


*/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int tickets = 1000;

// 创建一个互斥量
pthread_mutex_t mutex;


void *sell_tickets(void *arg) {
    // sale
    while (1) {
        // 加锁
        pthread_mutex_lock(&mutex);

        if (tickets > 0) {
            usleep(600);
            printf("child thread: %ld are selling %d ticket\n", pthread_self(), tickets);
            --tickets;
        } else {
            // 解锁
            pthread_mutex_unlock(&mutex);
            break;
        }

        // 解锁
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, NULL, sell_tickets, NULL);
    pthread_create(&tid2, NULL, sell_tickets, NULL);
    pthread_create(&tid3, NULL, sell_tickets, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);


    // 释放互斥量资源
    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);
    return 0;
}