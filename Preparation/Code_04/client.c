//
// Created by larry on 23-4-8.
//

// TCP client

// #include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    if (ret == -1) {
        perror("connect");
        exit(-1);
    }

    while (1) {
        // send data to server
        char *data = "hello, i am client.";
        write(fd, data, strlen(data));

        sleep(1);

        // recv data from server
        char recv_buf[1024] = {0};
        int len = read(fd, recv_buf, sizeof(recv_buf));
        if (len == -1) {
            perror("read");
            exit(-1);
        } else if (len > 0) {
            printf("recv server data: %s\n", recv_buf);
        } else if (len == 0) {
            printf("server closed...\n");
            break;
        }

    }
    close(fd);

    return 0;
}