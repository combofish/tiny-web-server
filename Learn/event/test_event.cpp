//
// Created by larry on 23-6-19.
//
#include <iostream>
#include<event.h>
#include <event2/event-config.h>
#include <event2/event.h>

struct event ev;
struct timeval tv;

void time_cb(int fd, short event, void *argc) {
    printf("timer wakeup...\n");
    event_add(&ev, &tv);
}

int main() {

    struct event_base *base = event_init();
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    evtimer_set(&ev, time_cb, nullptr);
    event_base_set(base, &ev);
    event_add(&ev, &tv);
    event_base_dispatch(base);


    return 0;
}