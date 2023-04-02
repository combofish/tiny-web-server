/**
 // mkdir
 #include<sys/stat.h>
 #include <sys/types.h>

 int mkdir(const char *pathname, mode_t mode);
 参数： mode: 八进制的数

 // rmdir
 #include <unistd.h>

 int rmdir(const char *pathname);
 作用： 删除空目录

 // rename
 #include <stdio.h>

 int rename(const char *oldpath, const char *newpath);

 // chdir
 #include <unistd.h>

 int chdir(const char *path);
 作用：修改进程的工作目录

 // getcwd
 #include <unistd.h>

 char *getcwd(char *buf, size_t size);
 作用： 获取当前的工作目录
 参数： buf:存储的路径，指向的是一个数组（传出参数）
       size: 数组大小
返回值： 返回的指向的一块内存， 这个数据就是第一个参数

*/

#include <sys/unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){

  // 获取当前的工作目录
  char buff[128];
  getcwd(buff, sizeof(buff));

  printf("current work path:%s \n", buff);

  // 修改工作目录
  int ret = chdir("../");
  
  int fd = open("chdir.txt", O_CREAT | O_RDWR, 0664);
  if(-1 == fd){
    perror("open err");
    return -1;
  }

  close(fd);

  // 获取当前的工作目录
  char buffCur[128];
  getcwd(buffCur, sizeof(buffCur));
  printf("now current work path: %s\n", buffCur);
  return 0;
}
 
