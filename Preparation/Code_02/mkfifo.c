/**
   创建 fifo 文件
   1. 通过命令： mkfifo
   2. 通过函数:  mkfifo

   NAME
       mkfifo, mkfifoat - make a FIFO special file (a named pipe)

   SYNOPSIS
       #include <sys/stat.h>
       #include <sys/types.h>

       int mkfifo(const char *pathname, mode_t mode);

       参数： pathname: 管道名称的路径
       mode: 文件的权限， 和 open 的 mode 是一样的, 是一个八进制的数。
       返回值：成功返回0， 失败 -1， 并设置 errno。


 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  int ret = access("fifo1", F_OK);
  if (-1 == ret) {
    printf("FIFO file do not exists, creat fifo file.\n");
    int ret = mkfifo("fifo1", 0664);
    if (ret == -1) {
      perror("mkfifo");
      exit(0);
    }
  }

  return 0;
}
