//
// Created by larry on 23-4-17.
//
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include <signal.h>
#include <cstring>
#include <cassert>

#include "locker.hpp"
#include "thread_pool.hpp"
#include "http_conn.hpp"

#define MAX_FD 65535 // 最大的文件描述符的个数
#define MAX_EVENT_NUMBER 10000 // 监听的最大的事件数量

// 添加信号捕捉
void add_sig(int sig, void( handler )(int)) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("usage: %s port_number\n", basename(argv[0]));
        return 1;
    }

    int port = atoi(argv[1]);
    add_sig(SIGPIPE, SIG_IGN);

    ThreadPool<http_conn> *pool = NULL;
    try {
        pool = new ThreadPool<http_conn>;
    } catch (...) {
        return 1;
    }

    http_conn *users = new http_conn[MAX_FD];

    int listen_fd = socket(PF_INET, SOCK_STREAM, 0);

    int ret = 0;
    struct sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    // 端口复用
    int reuse = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ret = bind(listen_fd, (struct sockaddr *) &address, sizeof(address));
    ret = listen(listen_fd, 5);

    // 创建epoll对象，和事件数组，添加
    epoll_event events[MAX_EVENT_NUMBER];
    int epoll_fd = epoll_create(5);
    // 添加到epoll对象中
    add_fd(epoll_fd, listen_fd, false);
    http_conn::m_epoll_fd = epoll_fd;

    while (true) {
        int number = epoll_wait(epoll_fd, events, MAX_EVENT_NUMBER, -1);
        if ((number < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        }

        for (int i = 0; i < number; i++) {
            int sock_fd = events[i].data.fd;
            if (sock_fd == listen_fd) {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int conn_fd = accept(listen_fd, (struct sockaddr *) &client_address, &client_addrlength);
                if (conn_fd < 0) {
                    printf("errno is: %d\n", errno);
                    continue;
                }
                if (http_conn::m_user_count >= MAX_FD) {
                    close(conn_fd);
                    continue;
                }
                users[conn_fd].init(conn_fd, client_address);
            } else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                users[sock_fd].close_conn();
            } else if (events[i].events & EPOLLIN) {
                if (users[sock_fd].read()) {
                    pool->append(users + sock_fd);
                } else {
                    users[sock_fd].close_conn();
                }
            } else if (events[i].events & EPOLLOUT) {

                if (!users[sock_fd].write()) {
                    users[sock_fd].close_conn();
                }

            }
        }
    }
    close(epoll_fd);
    close(listen_fd);
    delete[] users;
    delete pool;
    return 0;
}