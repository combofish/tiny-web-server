/**
        #include <sys/types.h>
        #include <unistd.h>

        int truncate(const char *path, off_t length);
            作用： 缩减或者扩展文件的尺寸到指定大小
            参数： path: 需要修改的文件的路径
                  length: 需要最终文件变成的大小

            返回值： 成功 0， 失败 -1.
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  int ret = truncate("cpy.txt", 20);
  if (-1 == ret) {
    perror("truncate err");
    return -1;
  }
  return 0;
}
