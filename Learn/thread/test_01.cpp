//
// Created by larry on 23-6-15.
//


#include <unistd.h>
#include <sys/types.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <icecream.hpp>

int main() {

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork error");
        exit(-1);
    } else if (pid == 0) {
        printf("child thread: %d,%d \n", pid, getpid());
    } else {
        printf("parent thread:  %d,%d \n", pid, getpid());
    }




    return 0;
}