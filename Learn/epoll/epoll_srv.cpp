//
// Created by larry on 23-6-16.
//
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/epoll.h>
#include <iostream>

#define OPEN_MAX 1024
#define MAX_LINE 80

int main() {
    struct sockaddr_in sock_srv;
    memset(&sock_srv, 0, sizeof(sock_srv));
    sock_srv.sin_port = htons(9999);
    sock_srv.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &sock_srv.sin_addr.s_addr);

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        exit(-1);
    }
    char reuse = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, (socklen_t) sizeof(reuse));

    int ret = bind(sock_fd, (struct sockaddr *) &sock_srv, sizeof(sock_srv));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    ret = listen(sock_fd, 20);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }
    printf("listen ok\n");


    int client[OPEN_MAX];
    for (int i = 0; i < OPEN_MAX; ++i) client[i] = -1;

    int maxI = -1;

    auto efd = epoll_create(OPEN_MAX);
    if (efd == -1) {
        perror("epoll_create");
        exit(-1);
    }

    struct epoll_event tep, ep[OPEN_MAX];

    tep.events = EPOLLIN;
    tep.data.fd = sock_fd;
    ret = epoll_ctl(efd, EPOLL_CTL_ADD, sock_fd, &tep);
    if (ret == -1) {
        perror("epoll_ctl");
        exit(-1);
    }

    struct sockaddr_in cli_addr;
    char str[INET_ADDRSTRLEN];
    char buf[MAX_LINE];

    while (1) {
        auto n_ready = epoll_wait(efd, ep, OPEN_MAX, -1); // 阻塞监听
        if (n_ready == -1) { perror("epoll_wait"); }

        for (int i = 0; i < n_ready; ++i) {
            if (!(ep[i].events & EPOLLIN)) continue;

            if (ep[i].data.fd == sock_fd) {
                socklen_t cli_len = sizeof(cli_addr);
                auto conn_fd = accept(sock_fd, (struct sockaddr *) &cli_addr, &cli_len);
                printf("received from %s at PORT %d\n",
                       inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, str, sizeof(str)),
                       ntohs(cli_addr.sin_port));

                int j;
                for (j = 0; j < OPEN_MAX; ++j) {
                    if (client[j] < 0) {
                        client[j] = conn_fd;
                        break;
                    }
                }

                if (j == OPEN_MAX) {
                    perror("too many clients");
                }

                if (j > maxI) maxI = j;

                tep.events = EPOLLIN;
                tep.data.fd = conn_fd;
                ret = epoll_ctl(efd, EPOLL_CTL_ADD, conn_fd, &tep);
                if (ret == -1) {
                    perror("epoll_ctl");
                }
            } else {
                auto cli_sock_fd = ep[i].data.fd;
                int n = read(cli_sock_fd, buf, MAX_LINE);
                int j;
                if (n == 0) {
                    for (j = 0; j <= maxI; ++j) {
                        if (client[j] == cli_sock_fd) {
                            client[j] = -1;
                            break;
                        }
                    }
                    ret = epoll_ctl(efd, EPOLL_CTL_DEL, cli_sock_fd, NULL);
                    if (ret == -1) {
                        perror("epoll_ctl");
                    }
                    close(cli_sock_fd);
                    printf("client[%d] closed connection\n", j);
                } else {
                    printf("receive client's data: %s\n", buf);
                    for (j = 0; j < n; ++j) {
                        buf[j] = std::toupper(buf[j]);
                    }
                    write(cli_sock_fd, buf, n);
                }
            }
        }
    }
    close(sock_fd);
    close(efd);
    return 0;
}