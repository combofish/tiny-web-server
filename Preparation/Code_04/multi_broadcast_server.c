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

    // 设置多播属性，设置外出接口
    struct in_addr imr_multiaddr;
    inet_pton(AF_INET, "239.0.0.10", &imr_multiaddr.s_addr);
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &imr_multiaddr, sizeof(imr_multiaddr));

    // 创建广播的地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "239.0.0.10",& addr.sin_addr.s_addr);

    int num = 0;
    while (1) {
        char send_buf[128] = {0};
        sprintf(send_buf, "hello, client....%d\n", num++);

        sendto(fd, send_buf, strlen(send_buf) + 1, 0, (struct sockaddr *) &addr, sizeof(addr));
        printf("组播的数据： %s\n", send_buf);
        sleep(1);
    }
    close(fd);
    return 0;
}