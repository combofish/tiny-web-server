//
// Created by larry on 23-4-3.
//

/**
条件变量的类型 pthread_cond_t, 它不是锁

pthread_cond_init
pthread_cond_destroy
pthread_cond_wait
 - 等待，调用了该函数，线程会阻塞
pthread_cond_timedwait
 - 等待了多长时间，调用了这个函数，线程会阻塞，知道指定的时间结束
pthread_cond_signal
 - 唤醒一个或多个等待的线程
pthread_cond_broadcast
 - 唤醒所有等待的线程
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>

// 创建一个互斥量
pthread_mutex_t mutex;

//
pthread_cond_t cond;

struct Node {
    int num;
    struct Node *next;
};

// head node
struct Node *head = NULL;

void *producer(void *arg) {
    // 不断创建新的节点添加到链表中
    while (1) {
        pthread_mutex_lock(&mutex);

        struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));

        // 头插法
        newNode->next = head;
        head = newNode;

        newNode->num = rand() % 1000;
        printf("add node, num: %d, tid: %ld\n", newNode->num, pthread_self());

        // 只要生产了一个就通知消费者消费
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}

void *customer(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        if (head != NULL) {
            // has data
            struct Node *tmp = head;
            head = head->next;
            printf("delete node num: %d, tid: %ld\n", tmp->num, pthread_self());
            free(tmp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
        } else {
            // no data, need to wait.

            // 当着函数调用阻塞的时候，会对互斥锁进行解锁，当不阻塞的时候，会重新对互斥加锁。
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
        }


    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

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

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);
    return 0;
}