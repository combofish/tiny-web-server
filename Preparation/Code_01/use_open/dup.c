/**
        #include <unistd.h>

        int dup(int oldfd);
作用： 复制一个新的文件描述符，和原来的文件描述符指向同一个文件，
从空闲文件描述符表中找一个最小的，作为新的拷贝的文件描述符

        int dup2(int oldfd, int newfd);



 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd1 = open("a.txt", O_RDWR | O_CREAT, 0664);
  int fd2 = dup(fd1);
  if (fd2 == -1) {
    perror("dup");
    return -1;
  }

  printf("fd1: %d, fd2: %d\n", fd1, fd2);

  close(fd1);

  char *str = "helloworld";
  int ret = write(fd2, str, strlen(str));
  if (-1 == ret) {
    perror("write");
    return -1;
  }

  close(fd2);

  return 0;
}
