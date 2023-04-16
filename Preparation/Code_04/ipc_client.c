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
    unlink("client.sock");

    int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (cfd == -1) {
        perror("socket");
        exit(-1);
    }

    // connect
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, "server.sock");
    int ret = connect(cfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("connect");
        exit(-1);
    }

    int num = 0;
    while (1) {
        char buf[128];
        sprintf(buf, "hello, i am client %d\n", num++);
        send(cfd, buf, strlen(buf) + 1, 0);
        printf("client say: %s\n", buf);

        int len = recv(cfd, buf, sizeof(buf), 0);
        if (len == -1) {
            perror("recv");
            exit(-1);
        } else if (len == 0) {
            printf("server close...\n");
            break;
        } else if (len > 0) {
            printf("server say: %s\n", buf);
        }
        sleep(1);
    }

    close(cfd);
    return 0;
}