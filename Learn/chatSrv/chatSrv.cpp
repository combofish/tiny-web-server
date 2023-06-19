//
// Created by larry on 23-6-18.
//

#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <cstdio>
#include <algorithm>
#include <iostream>

#include "mydb.h"

#define MAX_LINE 80
#define SERV_PORT 8000

const char CL_CMD_REG = 'r';
const char CL_CMD_LOGIN = 'l';
const char CL_CMD_CHAT = 'c';

int countChar(const char *p, const char chr) {
    int cnt = 0, i = 0;
    while (*(p + i)) {
        if (p[i] == chr) ++cnt;
        ++i;
    }
    return cnt;
}

int getName(char str[], char szName[]) {
    sprintf(szName, "%s", str + 2);

    printf("szName:[%s]\n", szName);
    return 0;
}


int main() {
    showTable();

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);

    int on = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&on, sizeof(on));
    auto ret = bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (ret == -1) {
        perror("bind");
    }

    ret = listen(sock_fd, 20);
    if (ret == -1) perror("listen");

    auto max_fd = sock_fd;
    int maxI = -1;
    int n_ready;
    int client[FD_SETSIZE];
    char szName[255] = {0}, szPwd[128] = {0}, repBuf[512] = {0};

    printf("szPwd: %s", szPwd);

    fd_set r_set, all_set;
    for (int i = 0; i < FD_SETSIZE; ++i) client[i] = -1;
    FD_ZERO(&all_set);
    FD_SET(sock_fd, &all_set);

    while (1) {
        std::cout << "Starting loop...\n" << std::endl;

        r_set = all_set;
        n_ready = select(max_fd + 1, &r_set, NULL, NULL, NULL);
        if (n_ready < 0) perror("select");

        if (FD_ISSET(sock_fd, &r_set)) {

            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            auto conn_fd = accept(sock_fd, (struct sockaddr *) &client_addr, &client_len);
            printf("generate new conn_fd: [%d]\n", conn_fd);

            char ip_buf[16];
            printf("received from %s at PORT %d\n",
                   inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip_buf, sizeof(ip_buf)),
                   ntohs(client_addr.sin_port));

            int i = 0;
            for (; i < FD_SETSIZE; ++i) {
                if (client[i] == -1) {
                    client[i] = conn_fd;
                    printf("add new conn_fd : [%d]\n", conn_fd);
                    break;
                }
            }

            if (i == FD_SETSIZE) {
                perror("too many clients\n");
                exit(1);
            }

            FD_SET(conn_fd, &all_set);
            max_fd = std::max(max_fd, conn_fd);
            maxI = std::max(maxI, i);
            if (--n_ready == 0) continue;
        }

        int cur_fd;
        for (int i = 0; i <= maxI; ++i) {
            cur_fd = client[i];
            if (cur_fd < 0) continue;
            if (FD_ISSET(cur_fd, &r_set)) {

                int n;
                char buf[512];
                if ((n = read(cur_fd, buf, MAX_LINE)) == 0) {
                    close(cur_fd);
                    FD_CLR(cur_fd, &all_set);
                    client[i] = -1;
                } else {

                    printf("read buf from client:[%s]\n", buf);

                    char code = buf[0];
                    printf("code:[%c]\n", code);
                    switch (code) {
                        case CL_CMD_REG:
                            if (countChar(buf, ',') != 1) {
                                printf("invalid proto!\n");
                                break;
                            }

                            printf("get buf: [%s]\n", buf);
                            getName(buf, szName);
                            if (isExist(szName) == 1) {
                                sprintf(repBuf, "r,exist");
                            } else {
                                insert(szName);
                                showTable();
                                sprintf(repBuf, "r,ok");
                                printf("reg ok,%s\n", szName);
                            }

                            write(cur_fd, repBuf, strlen(repBuf));
                            break;

                        case CL_CMD_LOGIN:
                            if (countChar(buf, ',') != 1) {
                                printf("invalid proto!\n");
                                break;
                            }

                            getName(buf, szName);
                            if (isExist(szName) == 1) {
                                sprintf(repBuf, "l,ok");
                                printf("login ok,%s\n", szName);
                            } else {
                                sprintf(repBuf, "l,noexist");
                            }

                            write(cur_fd, repBuf, strlen(repBuf));
                            break;

                        case CL_CMD_CHAT:
                            printf("----------------------------> send all\n");

                            for (int j = 0; j <= maxI; ++j) {
                                if (client[j] != -1 && client[j] != cur_fd) {
                                    printf("---> send to [%d] with buf [%s].\n", client[j], buf + 2);
                                    write(client[j], buf + 2, n);
                                    // break;
                                }
                            }
                    }


                }
                if (--n_ready == 0) break;
            }
        }

    }
    close(sock_fd);
    return 0;
}