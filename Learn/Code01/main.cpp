//
// Created by larry on 23-6-7.
//
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>

char rbuf[50], sbuf[100];

int main() {
    int sock_fd, size, ret;

    struct sockaddr_in s_addr, r_addr;
    size = sizeof(struct sockaddr_in);
    memset(&s_addr, 0, size);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(8888);
// s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        return -1;
    }

    char on = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    ret = bind(sock_fd, (struct sockaddr *) &s_addr, sizeof(s_addr));
    if (ret < 0) {
        perror("bind");
        return -1;
    }
    int val = sizeof(struct sockaddr);


    while (1) {
        puts("waiting data");
        ret = recvfrom(sock_fd, rbuf, 50, 0, (struct sockaddr *) &r_addr, (socklen_t *) &val);
        if (ret < 0) perror("recvform failed");
        printf("recv data: %s\n", rbuf);
        sprintf(sbuf, "Server has received your data(%s)\n", rbuf);

        char ip_buf[16];
        printf("client ip: %s, port: %d\n",
               inet_ntop(AF_INET, &r_addr.sin_addr.s_addr, ip_buf, sizeof(ip_buf)),
               ntohs(r_addr.sin_port));

        ret = sendto(sock_fd, sbuf, strlen(sbuf), 0, (struct sockaddr *) &r_addr, sizeof(struct sockaddr));
        if (ret < 0) perror("sendto failed");
        memset(rbuf, 0, 50);
    }
    close(sock_fd);
    return 0;

}