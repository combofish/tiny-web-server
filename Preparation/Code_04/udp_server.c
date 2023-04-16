//
// Created by larry on 23-4-16.
//
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(fd, &addr, sizeof(addr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    while (1) {
        char buf[128] = {0};
        char ip_buf[16];
        struct sockaddr_in client_addr;
        int len = sizeof(client_addr);
        int num = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *) &client_addr, &len);

        printf("client ip: %s, port: %d\n",
               inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip_buf, sizeof(ip_buf)),
               ntohs(client_addr.sin_port));

        printf("client say: %s\n", buf);

        sendto(fd, buf, strlen(buf) + 1, 0, (struct sockaddr *) &client_addr, sizeof(client_addr));
    }
    close(fd);
    return 0;
}