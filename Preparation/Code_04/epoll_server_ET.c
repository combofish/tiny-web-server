//
// Created by larry on 23-4-16.
//
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main() {

    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in s_addr;
    s_addr.sin_port = htons(9999);
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;

    // bind
    int ans = bind(lfd, (struct sockaddr *) &s_addr, sizeof(s_addr));
    if (ans == -1) {
        perror("bind");
        exit(-1);
    }

    // listen;
    listen(lfd, 8);

    // create epoll
    int efd = epoll_create(1);

    // 将监听号的文件描述符相关的检测信息添加到 epoll
    struct epoll_event epev;
    epev.events = EPOLLIN | EPOLLOUT; // EPOLLIN | EPOLLOUT
    epev.data.fd = lfd;
    epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[1024];

    while (1) {
        int ret = epoll_wait(efd, epevs, 1024, -1);
        if (ret == -1) {
            perror("epoll_wait");
            exit(-1);
        }

        printf("ret = %d\n", ret);

        for (int i = 0; i < ret; ++i) {
            int cur_fd = epevs[i].data.fd;
            if (cur_fd == lfd) { // 监听的文件描述符有数据到达，有客户端连接
                struct sockaddr_in client_addr;
                int len = sizeof(client_addr);
                int cfd = accept(lfd, (struct sockaddr *) &client_addr, &len);

                // 设置 cfd 非阻塞属性
                int flag = fcntl(cfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(cfd, F_SETFL, flag);

                epev.events = EPOLLIN | EPOLLET; // 设置边沿触发
                epev.data.fd = cfd;
                epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &epev);
            } else { // 有数据到达，需要通信 // 说明客户端发来了数据
                if (epevs[i].events & EPOLLOUT) {
                    continue;
                }
                char send_buf[1024];
                char recv_buf[5] = {0};
                int len = 0;
                int send_n = 0;
                while (1) {
                    memset(recv_buf, 0, sizeof(recv_buf));
                    int len = read(cur_fd, &recv_buf, sizeof(recv_buf) - 1);
                    if (len > 0) {
                        printf("recv client data: %s, len = %d\n", recv_buf, len);
                        memcpy(&send_buf[send_n], recv_buf, len);
                        send_n += len;
                    } else if (len == -1) {
                        if (errno == EAGAIN) {
                            printf("recv over...\n");
                            write(cur_fd, &send_buf, send_n);
                            break;
                        }
                        perror("read");
                        exit(-1);
                    } else if (len == 0) {
                        printf("client close fd: %d\n", cur_fd);
                        epoll_ctl(efd, EPOLL_CTL_DEL, cur_fd, NULL);
                        close(cur_fd);
                        continue;
                    }
                }
            }
        }
    }

    close(lfd);
    close(efd);
    return 0;
}