//
// Created by larry on 23-6-21.
//

#include <event.h>
#include <evhttp.h>
#include <event2/listener.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

void event_cb(struct bufferevent *bev, short what, void *ctx) {
    printf("error account [%x]!\n", what);
    if (what & BEV_EVENT_EOF) {
        // printf("client [%d] outline\n", *(int *) ctx);
        printf("client [-] outline\n");
        bufferevent_free(bev);
    } else {
        printf("unknown\n");
    }
}

void read_cb(struct bufferevent *bev, void *ctx) {
    // int fd = *(int *) ctx;
    char buf[128] = {0};
    auto ret = bufferevent_read(bev, buf, sizeof(buf));
    if (ret < 0) {
        printf("bufferevent_read error!\n");
    } else {
        // printf("read fron [%d] [%s]\n", fd, buf);
        printf("read fron [-] [%s]\n", buf);
    }
}


void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
                 struct sockaddr *addr, int socklen, void *arg) {

    printf("accept [%d] connect\n", fd);

    auto base = (struct event_base *) arg;

    // 针对已经存在的socket，创建 bufferevent 对象
    // 事件集合 （从主函数传递来）， fd（代表 TCP 连接）、如果释放 bufferevent 对象则关闭连接
    auto bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (bev == nullptr) {
        perror("bufferevent_socket_new");
        exit(1);
    }

    // 给 bufferevent 设置回调对象
    // bufferevent 对象， 读事件回调函数，写事件回调函数、其他事件回调函数、参数
    // bufferevent_setcb(bev, read_cb, nullptr, event_cb, &fd);
    bufferevent_setcb(bev, read_cb, nullptr, event_cb, nullptr);

    // 使能事件类型
    bufferevent_enable(bev, EV_READ);


}

/**
 * socket
 * bind
 * listen
 * accept
 */
int main() {
    // 创建 socket 对象，绑定，监听，接受连接
    // 创建监听对象，在指定的地址上监听接下来的 TCP 连接

    auto event_base = event_base_new();
    if (event_base == nullptr) {
        perror("event_init");
        exit(1);
    }

    // struct event ev;


    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    auto listener = evconnlistener_new_bind(event_base, listener_cb, event_base,
                                            LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 10,
                                            (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (listener == nullptr) {
        perror("evconnlistener_new_bind");
        exit(1);
    }

    event_base_dispatch(event_base);

    evconnlistener_free(listener);
    event_base_free(event_base);
    return 0;
}