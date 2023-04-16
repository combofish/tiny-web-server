/**

*/

#include <stdio.h>
#include <arpa/inet.h>

#include <bits/socket.h>

struct sockaddr a;

int main() {

    // htons 转换端口
    unsigned short a = 0x0102;
    printf("a: %x\n", a);

    unsigned short b = htons(a);

    printf("b: %x\n", b);
    printf("===============================\n");

    // htonl 转换 IP
    char buf[4] = {192, 168, 1, 100};
    int num = *(int *) buf;

    int ans = htonl(num);

    unsigned char *p = (unsigned char *) &ans;

    printf("%d,%d,%d,%d\n", *p, *(p + 1), *(p + 2), *(p + 3));
    printf("===============================\n");

    // ntohs
    unsigned char buf1[4] = {1, 1, 168, 192};
    num = *(int *) buf1;
    ans = ntohl(num);

    p = (char *) &ans;
    printf("%d %d %d %d\n", *p, *(p + 1), *(p + 2), *(p + 3));
    printf("===============================\n");

    // ntohs
    a = 0x0108;
    printf("a: %x\n", a);
    b = ntohs(a);
    printf("b: %x\n", b);

    return 0;
}