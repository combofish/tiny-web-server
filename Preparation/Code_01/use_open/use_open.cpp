/**
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {

  // 打开一个已经存在的文件
  // int open(const char* pathname, int flags);
  // int open(const char* pathname, int flags， mode_t mode );
  // 参数： pathname: 要创建的文件路径
  //       flags: 对文件的操作权限和其他设置
  //       mode: 八进制数，表示用户对新创建的文件的操作权限， 最终权限是 (mode & ~umask)
  // 返回一个新的文件描述符， 如果失败，返回 -1。
  // errno: 属于Linux 系统函数库, 库里面的一个全局变量，记录的是最近的错误号。

  /**
     #include<stdio.h>
     void perror(const char *s);
     作用： 打印 errno 对应的错误描述
     s 参数： 用户描述，比如 hello， 最终输出的内容的是 hello: xxx
     (实际的错误输出)
   */

  int fd = open("./a.txt", O_RDONLY);
  if (-1 == fd) {
    perror("open");
  }

  close(fd);

  return 0;
}
