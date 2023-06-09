/**

        #include <unistd.h>

        int dup(int oldfd);
作用： 复制一个新的文件描述符，和原来的文件描述符指向同一个文件，
从空闲文件描述符表中找一个最小的，作为新的拷贝的文件描述符

        int dup2(int oldfd, int newfd);
        作用： 重定向文件描述符，
        oldfd 指向 a.txt, newfd 指向 b.txt, 调用函数成功后， newfd 和 b.txt
close, newfd 指向了a.txt。 oldfd 必须是一个有效的文件描述符, oldfd 和 newfd
相同，相当于什么都没做



 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  //
  int fd = open("1.txt", O_RDWR | O_CREAT, 0664);
  if (fd == -1) {
    perror("open");
    return -1;
  }

  int fd1 = open("2.txt", O_RDWR | O_CREAT, 0664);
  if (-1 == fd1) {
    perror("open");
    return -1;
  }

  printf("fd: %d, fd1 %d \n", fd, fd1);
  int fd2 = dup2(fd, fd1);
  if (fd2 == -1) {
    perror("dup2");
    return -1;
  }

  // 通过 fd1 去写数据， 实际操作的是 1.txt, 而不是 2.txt
  char str[] = "hello dup2";
  int len = write(fd1, str, strlen(str));
  if (len == -1) {
    perror("write");
    return -1;
  }

  printf("fd: %d, fd1: %d, fd2: %d\n", fd, fd1, fd2);

  close(fd1);
  //  close(fd2);
  close(fd);

  return 0;
}
