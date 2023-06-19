//
// Created by larry on 23-6-19.
//

#include <cstdio>
#include <iostream>


int main() {

    printf("stdin:[%d], stdout:[%d].\n", fileno(stdin), fileno(stdout));
    return 0;
}