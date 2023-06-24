//
// Created by larry on 23-6-21.
//

#include <event.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

void fifo_read(evutil_socket_t fd, short events, void *arg) {
    char buf[32] = {0};

    auto ret = read(fd, buf, sizeof(buf));
    if (-1 == ret) {
        perror("read");
        exit(1);
    }
    printf("Read from pipe : [%s]\n", buf);
}

int main() {
    auto ret = mkfifo("fifo.tmp", 00700);
    if (-1 == ret) {
        perror("mkfifo");
        exit(1);
    }

    auto fd = open("fifo.tmp", O_RDONLY);
    if (-1 == fd) {
        perror("open");
        exit(1);
    }

    struct event ev;
    auto event_base = event_init();
    if (event_base == nullptr) {
        perror("event_init");
        exit(1);
    }

    // event_set(&ev, fd, EV_READ, fifo_read, nullptr);
    event_set(&ev, fd, EV_READ | EV_PERSIST, fifo_read, nullptr);
    event_add(&ev, nullptr);

    event_dispatch();
    return 0;

}