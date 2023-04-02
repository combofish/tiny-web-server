/**
        #include <unistd.h>

        int access(const char *pathname, int mode);
        作用： 判断某个文件是否有某个权限， 或者判断文件是否存在
        参数： pathname: 判断文件路径
               mode: R_OK, W_OK, X_OK： 判断是否有相应权限
        返回值： 0 成功，-1 失败

 */

#include <stdio.h>
#include <unistd.h>

int main() {

  int ret = access("cpy.txt", R_OK);
  if (-1 == ret) {
    perror("access err");
    return -1;
  }

  printf("file exist!!! \n");
  return 0;
}
