/**
        #include <sys/stat.h>

        int chmod(const char *pathname, mode_t mode);
            作用： 修改文件的权限
            参数： pathname: 要修改文件的路径
                   mode:  需要修改的权限值，八进制的数

            返回值：成功 0， 失败 -1
 */
#include <stdio.h>
#include <sys/stat.h>

int main() {
  int ret = chmod("cpy.txt", 0775);
  if (-1 == ret) {
    perror("chmod err");
    return -1;
  }

  return 0;
}
