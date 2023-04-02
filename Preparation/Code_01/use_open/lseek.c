/**
   // 标准C库函数
   #include <stdio.h>
   int fseek(FILE *stream, long offset, int whence);


   // Linux 系统函数库
    #include <sys/types.h>
    #include <unistd.h>

    off_t lseek(int fd, off_t offset, int whence);
        参数:  fd: 通过 open 得到，文件描述符
               offset: 偏移量
               whence:  SEEK_SET 设置文件指针的偏移量
                        SEEK_CUR 设置偏移量，当前位置 + 第二个参数的 offset 的值
                        SEEK_END 设置偏移量，文件大小 + 第二个参数的 offset 的值
        返回值： 返回文件指针的位置

        作用： 1. 移动文件指针到文件头
                   lseek(fd, 0, SEEK_SET);
               2. 获取当前文件指针的位置
                   lseek(fd, 0，SEEK_SET);
               3. 获取文件长度
                   lseek(fd, 0, SEEK_END);
               4. 扩展文件的长度， 当前文件 10b, 110b, 增加了100个字节
                   lseek(fd, 100, SEEK_END)


 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  int fd = open("cpy.txt", O_RDWR);
  if (-1 == fd) {
    perror("read err");
    return -1;
  }

  // 扩展文件的长度
  int ret = lseek(fd, 100, SEEK_END);
  if (-1 == ret) {
    perror("lseek");
    return -1;
  }

  write(fd , " ", 1);

  close(fd);

  return 0;
}
