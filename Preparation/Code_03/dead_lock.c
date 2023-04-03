//
// Created by larry on 23-4-3.
//

/**
    构成死锁的情况
    1. 忘记解锁
    2. 重复加锁
    3.
 */
#include <stdio.h>
#include <pthread.h>

int tickets = 100;

pthread_mutex_t mutex;

void *sale_tickets(void *arg) {
    while (1) {
        // add lock
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex);

        if (tickets > 0) {
            printf("child thread %ld, selling %d ticket\n", pthread_self(), tickets);
            --tickets;
        } else {
            pthread_mutex_unlock(&mutex);
            break;
        }

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sale_tickets, NULL);
    pthread_create(&tid2, NULL, sale_tickets, NULL);
    pthread_create(&tid3, NULL, sale_tickets, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);
    return 0;

}