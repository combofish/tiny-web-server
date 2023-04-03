//
// Created by larry on 23-4-3.
//

/**
信号量的类型 sem_t
NAME
       sem_init - initialize an unnamed semaphore

SYNOPSIS
       #include <semaphore.h>

       int sem_init(sem_t *sem, int pshared, unsigned int value);
         - 参数： sem 信号变量的地址
         - pshared: 0 用在线程之间，非0用在进程之间
         - value: 信号量中的值

sem_destroy
 - 释放资源

NAME
       sem_wait, sem_timedwait, sem_trywait - lock a semaphore

SYNOPSIS
       #include <semaphore.h>

       int sem_wait(sem_t *sem);
        - 对信号量的值减1，如果值为0，就阻塞

       int sem_trywait(sem_t *sem);

       int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

       Link with -pthread.

sem_post
 - 对信号量解锁，调用一次信号量的值加1.
sem_getvalue

 sem_t psem;
 sem_t csem;
 init(psem,0, 8)
 init(csem,0, 0)

 producer(){
    sem_wait(&psem);
    sem_post(&csem);
 }

 customer(){
    sem_wait(&csem);
    sem_post(&psem);
 }

*/


#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <semaphore.h>

// 创建一个互斥量
pthread_mutex_t mutex;

// sem
sem_t psem, csem;

struct Node {
    int num;
    struct Node *next;
};

// head node
struct Node *head = NULL;

void *producer(void *arg) {
    // 不断创建新的节点添加到链表中
    while (1) {
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);

        struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));

        // 头插法
        newNode->next = head;
        head = newNode;

        newNode->num = rand() % 1000;
        printf("add node, num: %d, tid: %ld\n", newNode->num, pthread_self());

        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
        usleep(100);
    }
    return NULL;
}

void *customer(void *arg) {
    while (1) {
        sem_wait(&csem);

        pthread_mutex_lock(&mutex);
        // has data
        struct Node *tmp = head;
        head = head->next;
        printf("delete node num: %d, tid: %ld\n", tmp->num, pthread_self());
        free(tmp);
        pthread_mutex_unlock(&mutex);

        sem_post(&psem);
//        usleep(100);

    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);

    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);

    // 创建5个生产者和5个消费者模型
    pthread_t producer_tid_s[5], customer_tid_s[5];

    for (int i = 0; i < 5; i++) {
        pthread_create(&producer_tid_s[i], NULL, producer, NULL);
        pthread_create(&customer_tid_s[i], NULL, customer, NULL);
    }

    for (int i = 0; i < 5; i++) {
        pthread_detach(producer_tid_s[i]);
        pthread_detach(customer_tid_s[i]);
    }

    while (1) {
        sleep(10);

    }


    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);
    return 0;
}