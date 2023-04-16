//
// Created by larry on 23-4-15.
//

#include <stdio.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/select.h>
#include <poll.h>

int main() {

    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in s_addr;
    s_addr.sin_port = htons(9999);
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;

    // bind
    int ans = bind(lfd, (struct sockaddr *) &s_addr, sizeof(s_addr));
    if(ans == -1){
        perror("bind");
        exit(-1);
    }

    // listen;
    listen(lfd, 8);

    // create poll
    struct pollfd fds[1024];
    for (int i = 0; i < 1024; i++) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;
    int nfds = 0;

    while (1) {
        // 调用 poll 系统函数，让系统内核帮检测哪些文件描述符有数据
        int ret = poll(fds, nfds + 1, -1);

        if (ret == -1) {
            perror("poll");
            exit(-1);
        } else if (ret == 0) {
            continue;
        } else if (ret > 0) { // 说明检测到了有文件描述符对应的缓冲区的数据发生论改变
            if (fds[0].revents & POLLIN) {
                struct sockaddr_in client_addr;
                int len = sizeof(client_addr);
                int cfd = accept(lfd, (struct sockaddr *) &client_addr, &len);

                for (int i = 1; i < 1024; i++) {
                    if (fds[i].fd == -1) {
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }
                nfds = nfds > cfd ? nfds : cfd;
            }

            for (int i = 1; i <= nfds; i++) {
                if (fds[i].revents & POLLIN) {    // 说明客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(fds[i].fd, &buf, sizeof(buf));
                    if (len == -1) {
                        perror("read");
                        exit(-1);
                    } else if (len == 0) {
                        printf("client closed...\n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    } else if (len > 0) {
                        printf("read buf = %s\n", buf);
                        write(fds[i].fd, buf, strlen(buf) + 1);
                    }

                }
            }

        }
    }

    close(lfd);

    return 0;

}