//
// Created by larry on 23-4-8.
//
// TCP 通信服务端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // 创建用于监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }
    // 绑定
    struct sockaddr_in so_addr;
    so_addr.sin_family = AF_INET;
    // inet_pton(AF_INET, "127.0.0.1", &so_addr.sin_addr.s_addr);
    so_addr.sin_addr.s_addr = INADDR_ANY;
    so_addr.sin_port = htons(9999);
    int ret = bind(lfd, (struct sockaddr *) &so_addr, sizeof(so_addr));
    if (-1 == ret) {
        perror("bind");
        exit(-1);
    }


    ret = listen(lfd, 8);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    // accept
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int cfd = accept(lfd, (struct sockaddr *) &client_addr, &len);
    if (cfd == -1) {
        perror("error");
        exit(-1);
    }

    // output
    char client_ip[16];
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));

    unsigned short client_port = ntohs(client_addr.sin_port);
    printf("client ip is: %s, port is %d.\n", client_ip, client_port);


    while(1) {
        // get client's data
        char recv_buf[1024] = {0};
        int rlen = read(cfd, recv_buf, sizeof(recv_buf));
        if (rlen == -1) {
            perror("read");
            exit(-1);
        } else if (rlen > 0) {
            printf("recv client data: %s\n", recv_buf);
        } else if (rlen == 0) {
            printf("client closed...\n");
        }

        // send data to client.
        char *data = "hello, i am server.";
        write(cfd, data, strlen(data));

    }
    close(cfd);
    close(lfd);

    return 0;
}