/**

   write to fifo file.

 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  // 1. 判断文件是否存在
  int ret = access("fifo1", F_OK);
  if (-1 == ret) {
    printf("FIFO file do not exists, creat fifo file.\n");

    // 2. 创建管道文件
    int ret = mkfifo("fifo1", 0664);
    if (ret == -1) {
      perror("mkfifo");
      exit(0);
    }
  }

  //  打开管道文件, 以只写的方式打开文件
  int fd = open("fifo1", O_WRONLY);
  if (-1 == fd) {
    perror("open");
    exit(0);
  }

  // 写数据
  for (int i = 0; i < 100; i++) {
    char buf[1024] = {0};
    sprintf(buf, "hello, %d\n", i);
    printf("write data: %s\n", buf);
    write(fd, buf, strlen(buf));
    sleep(1);
  }

  close(fd);
  return 0;
}
