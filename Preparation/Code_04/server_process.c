//
// Created by larry on 23-4-11.
//

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>


void recyle_child(int arg) {
    while (1) {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret == -1) { // 所有子进程都回收了
            break;
        } else if (ret == 0) { // 还有子进程活着
            break;
        } else if (ret > 0) { // 被回收了
            printf("子进程 %d 被回收了\n", ret);
        }
    }
}

int main() {
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recyle_child;
    // 注册信号捕捉
    sigaction(SIGCHLD, &act, NULL);


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

    // 不断循环，等待客户端连接
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

        // 每一个连接进来，创建一个子进程跟客户端通信。
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
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
                } else if(len == 0){
                    printf("client closed...\n");
                    break;
                }
                write(cfd, recvBuf, strlen(recvBuf) + 1);
            }

            close(cfd);
            exit(0); // 退出当前子进程
        };
    }

    close(lfd);
    return 0;
}