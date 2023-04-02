/**
       #include <fcntl.h>
       #include <unistd.h>

       int fcntl(int fd, int cmd, ... args);
       参数： fd 需要操作的文件描述符
       cmd : 表示对文件描述符进行如何操作
          1. 复制文件描述符， F_DUPFD
          2. F_GETFL 获取指定文件描述符状态 flag, 获取的flag 和通过 open
传递的flag 是一个东西。
          3. F_SETFL 设置文件描述符状态 flag, 必选项 O_RDONLY, O_WRONLY, O_RDWR,
可选项： O_APPEND, NONBLOCK, O_APPEND 表示追加数据， NONBLOK 设置能非阻塞。


阻塞和非阻塞： 描述的是函数调用的行为。

 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  // 1. 复制文件描述符
  /* int fd = open("1.txt", O_RDONLY); */

  /* int ret = fcntl(fd , F_DUPFD); */

  // 2. 修改或者获取文件状态 flag

  int fd = open("1.txt", O_RDWR);
  if (-1 == fd) {
    perror("open");
    return -1;
  }

  // 获取文件描述符的状态

  int flag = fcntl(fd, F_GETFL);
  if (-1 == flag) {
    perror("fcntl");
    return -1;
  }

  flag |= O_APPEND;

  // 修改文件描述符状态的 flag, 给 flag 加入 O_APPEND 这个标记
  int ret = fcntl(fd, F_SETFL, flag);

  if (-1 == ret) {
    perror("fcntl");
    return -1;
  }
  char str[] = "\nhello world from fcntl\n";

  write(fd, str, strlen(str));

  close(fd);

  return 0;
}
