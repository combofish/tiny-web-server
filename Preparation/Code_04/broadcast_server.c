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

    // 设置广播属性
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op));

    // 创建广播的地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, ".255",& addr.sin_addr.s_addr);

    int num = 0;
    while (1) {
        char send_buf[128] = {0};
        sprintf(send_buf, "hello, client....%d\n", num++);

        sendto(fd, send_buf, strlen(send_buf) + 1, 0, (struct sockaddr *) &addr, sizeof(addr));
        printf("广播的数据： %s\n", send_buf);
        sleep(1);
    }
    close(fd);
    return 0;
}