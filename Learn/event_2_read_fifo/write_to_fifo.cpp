//
// Created by larry on 23-6-21.
//
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main() {

    auto fd = open("fifo.tmp", O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    char buf[32] = {0};
    while (1) {
        scanf("%s", buf);
        auto ret = write(fd, buf, strlen(buf));
        if (ret == -1) {
            perror("write");
            exit(1);
        }
        if (!strcmp(buf, "bye")) {
            break;
        }

        memset(buf, 0, sizeof(buf));
    }
    return 0;
}