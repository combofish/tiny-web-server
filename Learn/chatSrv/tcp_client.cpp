//
// Created by larry on 23-6-7.
//

#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>


int main() {

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr.s_addr);

    // sin.sin_addr.s_addr = INADDR_ANY;

    int ret;
    ret = connect(sockfd, (struct sockaddr *) &sin, sizeof(sin));
    if (ret == -1) {
        perror("connect");
        return -1;
    }

    char rbuf[128], sbuf[128];
    int num = 0;
    while (1) {
        memset(rbuf, 0, sizeof(rbuf));
        memset(sbuf, 0, sizeof(sbuf));

        // sprintf(sbuf, "I am client(%d)", ++num);
//        sprintf(sbuf, "r,jack");
        ++num;


        std::string use_to_send;
        std::cin >> use_to_send;

        std::cout << "get input: [" << use_to_send << "]" << std::endl;

        ret = write(sockfd, use_to_send.c_str(), use_to_send.size() + 1);

//        ret = write(sockfd, sbuf, strlen(sbuf) + 1);
        if (ret == -1) {
            perror("send");
            return -1;
        }


        int len = read(sockfd, rbuf, sizeof(rbuf));
        if (len == -1) {
            perror("read");
            exit(-1);
        } else if (len > 0) {
            printf("recv from server: [%s]\n", rbuf);
        } else if (len == 0) {
            printf("server close \n");
            break;
        }
        sleep(1);
        // break;


        while (1) {
//            std::string use_to_send;
            std::cin >> use_to_send;

            std::cout << "get input: [" << use_to_send << "]" << std::endl;

            ret = write(sockfd, use_to_send.c_str(), use_to_send.size() + 1);

//        ret = write(sockfd, sbuf, strlen(sbuf) + 1);
            if (ret == -1) {
                perror("send");
                return -1;
            }
        }


    }

    return 0;
}