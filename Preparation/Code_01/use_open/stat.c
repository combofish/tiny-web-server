/**

        #include <sys/stat.h>
        #include <sys/types.h>
        #include <unistd.h>

        int stat(const char *pathname, struct stat *statbuf);
            作用：获取一个文件相关的一些信息
            参数：
             - pathname： 操作文件的路径
             - statbuf: 结构体变量，传出参数，用于保存获取到的文件信息。

            返回值：
              - 成功： 返回0，
              - 失败： 返回-1， 设置 errno

        int lstat(const char *pathname, struct stat *statbuf);
            作用： 获取软连接文件的信息

 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  struct stat statbuf;
  //  int ret = stat("cpy_s.txt", &statbuf);
  int ret = lstat("cpy_s.txt", &statbuf);

  if (-1 == ret) {
    perror("stat err");
    return -1;
  }

  printf("size: %1d\n", (int)statbuf.st_size);
  return 0;
}
