//
// Created by larry on 23-4-3.
//

/**

pthread_rwlock_t

 pthread_rwlock_init
 pthread_rwlock_destroy
 pthread_rwlock_rdlock
 pthread_rwlock_tryrdlock
 pthread_rwlock_wrlock
 pthread_rwlock_trywrlock
 pthread_rwlock_unlock

 案例：创建8个线程操作同一个全局变量
 3个线程不定时写这个全局变量
 其它线程不定时读这个全局变量
 */

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

int num = 1;

pthread_rwlock_t rwlock; // 读时可多个并发读
//pthread_mutex_t mutex; // 多个线程读是需要阻塞

void *writeNum(void *arg) {
    while (1) {
//        pthread_mutex_lock(&mutex);

        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("++write, tid: %ld, num = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);

//        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}

void *readNum(void *arg) {
    while (1) {
//        pthread_mutex_lock(&mutex);

        pthread_rwlock_rdlock(&rwlock);
        printf("===read, tid: %ld, num = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);

//        pthread_mutex_unlock(&mutex);

        usleep(100);
    }
    return NULL;
}

int main() {
    pthread_rwlock_init(&rwlock, NULL);

//    pthread_mutex_init(&mutex, NULL);

    pthread_t w_t_ids[3], r_t_ids[5];
    for (int i = 0; i < 3; i++)
        pthread_create(&w_t_ids[i], NULL, writeNum, NULL);

    for (int i = 0; i < 5; i++)
        pthread_create(&r_t_ids[i], NULL, readNum, NULL);

    for (int i = 0; i < 3; i++)
        pthread_detach(w_t_ids[i]);

    for (int i = 0; i < 5; i++)
        pthread_detach(r_t_ids[i]);

    pthread_rwlock_destroy(&rwlock);

//    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);

    return 0;
}
