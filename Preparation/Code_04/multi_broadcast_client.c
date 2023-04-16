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

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(fd, (struct sockaddr*)&server_addr, sizeof (server_addr));
    if(ret == -1){
        perror("read");
        exit(-1);
    }

    struct ip_mreq op;
    inet_pton(AF_INET, "239.0.0.10", &op.imr_multiaddr.s_addr);
    op.imr_interface.s_addr = INADDR_ANY;

    // 加入多播组
    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &op, sizeof(op));

    int num = 0;
    while (1) {
        char buf[128];
        int ans = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        printf("server say: %s\n", buf);
    }
    close(fd);
    return 0;
}