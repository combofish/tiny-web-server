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


int main() {

    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in s_addr;
    s_addr.sin_port = htons(9999);
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind(lfd, (struct sockaddr *) &s_addr, sizeof(s_addr));

    // listen;
    listen(lfd, 8);

    // create fd_sets
    fd_set rd_set, tmp;
    FD_ZERO(&rd_set);
    FD_SET(lfd, &rd_set);
    int max_fd = lfd;

    while (1) {
        tmp = rd_set;

        // 调用 select 系统函数，让系统内核帮检测哪些文件描述符有数据
        int ret = select(max_fd + 1, &tmp, NULL, NULL, NULL);
        if (ret == -1) {
            perror("select");
            exit(-1);
        } else if (ret == 0) {
            continue;
        } else if (ret > 0) { // 说明检测到了有文件描述符对应的缓冲区的数据发生论改变
            if (FD_ISSET(lfd, &tmp)) {  // 表示有新的客户端连接进来
                struct sockaddr_in client_addr;
                int len = sizeof(client_addr);
                int cfd = accept(lfd, (struct sockaddr *) &client_addr, &len);

                FD_SET(cfd, &rd_set); // 将新的文件描述符加入到集合中
                max_fd = max_fd > cfd ? max_fd : cfd; // 更新最大的文件描述符
            }

            for (int i = lfd + 1; i <= max_fd; i++) {
                if (FD_ISSET(i, &tmp)) {
                    // 说明客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(i, &buf, sizeof(buf));
                    if (len == -1) {
                        perror("read");
                        exit(-1);
                    } else if (len == 0) {
                        printf("client closed...\n");
                        close(i);
                        FD_CLR(i, &rd_set);
                    } else if (len > 0) {
                        printf("read buf = %s\n", buf);
                        write(i, buf, strlen(buf) + 1);
                    }

                }
            }
        }
    }

    close(lfd);

    return 0;

}