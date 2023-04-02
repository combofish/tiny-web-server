#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  /**
     #include <unistd.h>
     ssize_t read(int fd, void *buf, size_t count);
     参数: fd: 文件描述符， open得到，通过这个文件描述符操作某个文件
          buf: 需要读取数据存放的地方，数组的地址
	  count: 指定数组的大小
     返回值: 成功， 大于0， 返回实际读取到的字节数， 0 文件读取完，
             失败-1 文件读取失败

    #include <unistd.h>	     
    ssize_t write(int fd, const void *buf, size_t count);

    
   */


  // 1. open 打开文件
  int fd = open("read_write.c", O_RDONLY);
  if(-1 == fd){
    perror("open err");
    return -1;
  }

  // 2. 创建一个新的文件（copy 文件）

  int dest_fd = open("cpy.txt", O_WRONLY| O_CREAT, 0777);
  if(-1 == dest_fd){
    perror("create err");
    return -1;
  }
  
  // 3. 频繁的读写操作

  char buffer[1024] = {0};
  int len = 0;
  while((len =   read(fd, buffer, sizeof(buffer))) > 0){
    write(dest_fd, buffer, len);
  }
  
  // 4. 关闭文件
  close(dest_fd);
  close(fd);
  
  return 0;
}
