#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  // 创建一个新的文件
  int fd = open("create.txt", O_RDWR |O_CREAT, 0775);
  if(-1 == fd) {
    perror("open create err");
  }

  close(fd);


  return 0;

  }
