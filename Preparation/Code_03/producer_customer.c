//
// Created by larry on 23-4-3.
//

/**
生产者消费者模型的简化版
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>

// 创建一个互斥量
pthread_mutex_t mutex;

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

        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}

void *customer(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        if(head != NULL){
            // no data
            struct Node *tmp = head;
            head = head->next;
            printf("delete node num: %d, tid: %ld\n", tmp->num, pthread_self());
            free(tmp);
        }
        pthread_mutex_unlock(&mutex);
        usleep(100);

    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);

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