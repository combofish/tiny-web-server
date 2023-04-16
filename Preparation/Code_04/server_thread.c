//
// Created by larry on 23-4-12.
//

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

struct sockInfo {
    int fd;
    pthread_t tid; // 线程号
    struct sockaddr_in addr;
};


struct sockInfo sock_infos[128];

void *working(void *arg) {
    // 子线程和客户端通信 cfd, 客户端的信息

    struct sockInfo *p_info = (struct sockInfo *) arg;
    struct sockaddr_in client_addr = p_info->addr;
    int cfd = p_info->fd;

    // 获取子进程的信息
    char client_ip[16];
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
    unsigned short client_port = ntohs(client_addr.sin_port);

    printf("client ip is: %s, port is %d\n", client_ip, client_port);

    // 接收客户端发来的数据
    char recvBuf[1024] = {0};
    while (1) {
        int len = read(cfd, &recvBuf, sizeof(recvBuf));
        if (len == -1) {
            perror("read");
            exit(-1);
        } else if (len > 0) {
            printf("recv client data: %s\n", recvBuf);
        } else if (len == 0) {
            printf("client closed...\n");
            break;
        }
        write(cfd, recvBuf, strlen(recvBuf) + 1);
    }

    close(cfd);


    return NULL;
}

int main() {

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(lfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("bind");
        exit(0);
    }
    // 监听
    ret = listen(lfd, 128);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 初始化 数据
    int max_infos = sizeof(sock_infos) / sizeof(sock_infos[0]);
    for (int i = 0; i < max_infos; i++) {
        bzero(&sock_infos[i], sizeof(sock_infos[i]));
        sock_infos[i].fd = -1;
        sock_infos[i].tid = -1;
    }

    // 循环等待客户端连接，一旦一个客户端连接进来，就创建一个子线程进行通信
    while (1) {
        // 接受连接
        struct sockaddr_in client_addr;
        int s_len = sizeof(client_addr);
        int cfd = accept(lfd, (struct sockaddr *) &client_addr, &s_len);
        if (cfd == -1) {
            if (errno == EINTR) {
                continue;
            }
            perror("accept");
            exit(-1);
        }

        struct sockInfo *p_info;
        for (int i = 0; i < max_infos; i++) {
            // 从数组中找到一个可用的 sockInfo 元素
            if (sock_infos[i].fd == -1) {
                p_info = &sock_infos[i];
                break;
            }
            if (i == max_infos - 1) {
                sleep(1);
                i--;
            }
        }

        p_info->fd = cfd;
        memcpy(&p_info->addr, &client_addr, s_len);


        // pthread_t tid;
        pthread_create(&p_info->tid, NULL, working, p_info);

        pthread_detach(p_info->tid);
    }
    close(lfd);

    return 0;
}