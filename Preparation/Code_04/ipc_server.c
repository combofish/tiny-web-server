//
// Created by larry on 23-4-16.
//
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    unlink("server.sock");

    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "server.sock");
    int ret = bind(lfd, (struct sockaddr *) &addr, sizeof(addr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    ret = listen(lfd, 100);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    struct sockaddr_un client_addr;
    int len = sizeof(client_addr);
    int cfd = accept(lfd, (struct sockaddr *) &client_addr, &len);
    if (cfd == -1) {
        perror("accept");
        exit(-1);
    }

    printf("client socket filename: %s\n", client_addr.sun_path);

    while (1) {
        char buf[128];
        int len = recv(cfd, buf, sizeof(buf),0);
        if (len == -1) {
            perror("recv");
            exit(-1);
        } else if (len == 0) {
            printf("client close...\n");
            break;
        } else if (len > 0) {
            printf("client say: %s\n", buf);
            send(cfd, buf, len, 0);
        }
    }
    close(cfd);
    close(lfd);
    return 0;
}