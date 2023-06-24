//
// Created by larry on 23-6-21.
//

#include <event.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>

int signal_cnt = 0;

void signal_handler(evutil_socket_t fd, short events, void *arg) {
    auto ev = (struct event *) arg;

    ++signal_cnt;
    printf("receive signal [%d] (%d)\n", fd, signal_cnt);
    if (signal_cnt >= 2) {
        event_del(ev);
    }
}

int main() {


    struct event ev;
    auto event_base = event_init();

    event_assign(&ev, event_base, SIGINT, EV_SIGNAL | EV_PERSIST, signal_handler, &ev);
    event_add(&ev, nullptr);
    event_base_dispatch(event_base);
    return 0;
}