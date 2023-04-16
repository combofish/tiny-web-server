//
// Created by larry on 23-4-5.
//

/**
 字节序： 字节在内存中存储的顺序。

 小端字节序： 数据的高位字节存储在内存的高位地址，低位字节存储在内存的低位地址。
 大端字节序： 数据的高位字节存储在内存的低位地址，低位字节存储在内存的高位地址。

*/

#include <stdio.h>

int main() {
    union {
        short value; // 2 bytes
        char bytes[sizeof(short)]; // char [2]
    } test;

    test.value = 0x0102;
    if ((test.bytes[0]) == 1 && (test.bytes[1] == 2)) {
        printf("Big Endian\n");
    } else if ((test.bytes[0] == 2) && (test.bytes[1] == 1)) {
        printf("Little Endian.\n");
    } else {
        printf("Unknown!\n");
    }

    return 0;
}