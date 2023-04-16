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
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);

    int num = 0;
    while (1) {
        char send_buf[128];
        sprintf(send_buf, "hello, i am client %d\n", num++);
        sendto(fd, send_buf, strlen(send_buf) + 1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        int ret  = recvfrom(fd, send_buf, sizeof(send_buf), 0, NULL, NULL);
        printf("server say: %s\n", send_buf);
        sleep(1);
    }
    close(fd);
    return 0;
}