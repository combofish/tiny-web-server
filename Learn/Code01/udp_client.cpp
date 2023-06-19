//
// Created by larry on 23-6-7.
//
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>

int main() {
    int sockfd, ret;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);

    int num = 0;
    while (1) {
        char sendbuf[128];
        sprintf(sendbuf, "hello, i am a client(%d).\n", ++num);
        sendto(sockfd, sendbuf, sizeof(sendbuf), 0, (struct sockaddr *) &saddr, sizeof(saddr));

        char recvbuf[128];
        ret = recvfrom(sockfd, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        printf("server say: %s\n", sendbuf);
        sleep(1);
    }

    close(sockfd);
    return 0;
}