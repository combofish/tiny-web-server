//
// Created by larry on 23-4-2.
//

/**

 使用多线程实现买票案例
 有三个窗口，一共是100张票。

*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int tickets = 100; // 全局变量，所有线程共享

void *sell_tickets(void *arg) {
    // 卖票
    // int tickets = 100; // 局部变量
    while (tickets > 0) {
        usleep(6000);
        printf("%ld, 正在卖第 %d 张票\n", pthread_self(), tickets);
        tickets--;
    }

    return NULL;
}

int main() {

    // 创建3个子线程
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, NULL, sell_tickets, NULL);
    pthread_create(&tid2, NULL, sell_tickets, NULL);
    pthread_create(&tid3, NULL, sell_tickets, NULL);

    // 回收子线程的资源, 阻塞
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    // 设置线程分离
    //    pthread_detach(tid1);
    //    pthread_detach(tid2);
    //    pthread_detach(tid3);

    pthread_exit(NULL);

    return 0;
}