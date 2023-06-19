#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include "lst_timer.h"

#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024
#define TIMESLOT 5

static int pipe_fd[2];
static sort_timer_lst timer_lst;
static int epollfd = 0;

int set_nonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void add_fd(int epoll_fd, int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    set_nonblocking(fd);
}

void sig_handler(int sig) {
    int save_errno = errno;
    int msg = sig;
    send(pipe_fd[1], (char *) &msg, 1, 0);
    errno = save_errno;
}

void add_sig(int sig) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, nullptr) != -1);
}

void timer_handler() {
    // 定时处理任务，实际上就是调用tick()函数
    timer_lst.tick();
    // 因为一次 alarm 调用只会引起一次SIGALARM 信号，所以我们要重新定时，以不断触发 SIGALARM信号。
    alarm(TIMESLOT);
}

// 定时器回调函数，它删除非活动连接socket上的注册事件，并关闭之。
void cb_func(client_data *user_data) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->sock_fd, 0);
    assert(user_data);
    close(user_data->sock_fd);
    printf("close fd %d\n", user_data->sock_fd);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("usage: %s port_number\n", basename(argv[0]));
        return 1;
    }
    int port = atoi(argv[1]);

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listen_fd >= 0);

    ret = bind(listen_fd, (struct sockaddr *) &address, sizeof(address));
    assert(ret != -1);

    ret = listen(listen_fd, 5);
    assert(ret != -1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epoll_fd = epoll_create(5);
    assert(epoll_fd != -1);
    add_fd(epoll_fd, listen_fd);

    // 创建管道
    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipe_fd);
    assert(ret != -1);
    set_nonblocking(pipe_fd[1]);
    add_fd(epoll_fd, pipe_fd[0]);

    // 设置信号处理函数
    add_sig(SIGALRM);
    add_sig(SIGTERM);
    bool stop_server = false;

    auto users = new client_data[FD_LIMIT];
    bool timeout = false;
    alarm(TIMESLOT);  // 定时,5秒后产生SIGALARM信号

    while (!stop_server) {
        int number = epoll_wait(epoll_fd, events, MAX_EVENT_NUMBER, -1);
        if ((number < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        }

        for (int i = 0; i < number; i++) {
            int sockfd = events[i].data.fd;
            if (sockfd == listen_fd) {
                struct sockaddr_in client_address;
                socklen_t client_addr_length = sizeof(client_address);
                int conn_fd = accept(listen_fd, (struct sockaddr *) &client_address, &client_addr_length);
                add_fd(epoll_fd, conn_fd);
                users[conn_fd].address = client_address;
                users[conn_fd].sock_fd = conn_fd;

                // 创建定时器，设置其回调函数与超时时间，然后绑定定时器与用户数据，最后将定时器添加到链表timer_lst中
                auto timer = new util_timer;
                timer->user_data = &users[conn_fd];
                timer->cb_func = cb_func;
                time_t cur = time(nullptr);
                timer->expire = cur + 3 * TIMESLOT;
                users[conn_fd].timer = timer;
                timer_lst.add_timer(timer);
            } else if ((sockfd == pipe_fd[0]) && (events[i].events & EPOLLIN)) {
                // 处理信号
                // int sig;
                char signals[1024];
                ret = recv(pipe_fd[0], signals, sizeof(signals), 0);
                if (ret == -1) {
                    continue;
                } else if (ret == 0) {
                    continue;
                } else {
                    for (int j = 0; j < ret; ++j) {
                        switch (signals[j]) {
                            case SIGALRM: {
                                // 用timeout变量标记有定时任务需要处理，但不立即处理定时任务
                                // 这是因为定时任务的优先级不是很高，我们优先处理其他更重要的任务。
                                timeout = true;
                                break;
                            }
                            case SIGTERM: {
                                stop_server = true;
                            }
                        }
                    }
                }
            } else if (events[i].events & EPOLLIN) {
                memset(users[sockfd].buf, '\0', BUFFER_SIZE);
                ret = recv(sockfd, users[sockfd].buf, BUFFER_SIZE - 1, 0);
                printf("get %d bytes of client data %s from %d\n", ret, users[sockfd].buf, sockfd);
                util_timer *timer = users[sockfd].timer;
                if (ret < 0) {
                    // 如果发生读错误，则关闭连接，并移除其对应的定时器
                    if (errno != EAGAIN) {
                        cb_func(&users[sockfd]);
                        if (timer) {
                            timer_lst.del_timer(timer);
                        }
                    }
                } else if (ret == 0) {
                    // 如果对方已经关闭连接，则我们也关闭连接，并移除对应的定时器。
                    cb_func(&users[sockfd]);
                    if (timer) {
                        timer_lst.del_timer(timer);
                    }
                } else {
                    // 如果某个客户端上有数据可读，则我们要调整该连接对应的定时器，以延迟该连接被关闭的时间。
                    if (timer) {
                        time_t cur = time(nullptr);
                        timer->expire = cur + 3 * TIMESLOT;
                        printf("adjust timer once\n");
                        timer_lst.adjust_timer(timer);
                    }
                }
            }

        }

        // 最后处理定时事件，因为I/O事件有更高的优先级。当然，这样做将导致定时任务不能精准的按照预定的时间执行。
        if (timeout) {
            timer_handler();
            timeout = false;
        }
    }

    close(listen_fd);
    close(pipe_fd[1]);
    close(pipe_fd[0]);
    delete[] users;
    return 0;
}
