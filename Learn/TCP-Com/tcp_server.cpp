//
// Created by larry on 23-6-7.
//

#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>

int main() {

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    int ret;
    char on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    ret = bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        return -1;
    }

    ret = listen(sockfd, 8);
    if (ret == -1) {
        perror("listen");
        return -1;
    }

    struct sockaddr_in caddr;
    socklen_t size = sizeof(caddr);
    int cfd = accept(sockfd, (struct sockaddr *) &caddr, &size);
    if (cfd == -1) {
        perror("accept");
        return -1;
    }

    char ipbuf[16];
    printf("client's ip: %s, port: %d\n",
           inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)), htons(caddr.sin_port)
    );


    char rbuf[128], sbuf[128];
    while (1) {
        ret = read(cfd, rbuf, sizeof(rbuf));
        if (ret == -1) {
            perror("recvfrom");
            return -1;
        }else if(ret > 0){
            printf("recv from client: (%s)\n", rbuf);
        }else if(ret == 0){
            printf("client closed...\n");
        }

        sprintf(sbuf, "server recv: (%s)", rbuf);
        ret = write(cfd, sbuf, strlen(sbuf));
        if (ret == -1) {
            perror("sendto");
            return -1;
        }

        memset(rbuf, 0, sizeof(rbuf));
        memset(sbuf, 0, sizeof(sbuf));
    }

    close(sockfd);
    return 0;
}