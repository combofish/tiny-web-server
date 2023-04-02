/**

        #include <dirent.h>
        #include <sys/types.h>

        DIR *opendir(const char *name);
        参数：name : 需要打开的目录的名称
        返回值： DIR* 类型，理解为名录流，错误返回 NULL;

        #include <dirent.h>

        struct dirent *readdir(DIR *dirp);
        作用： 读取目录中的数据
        参数： dirp 通过 opendir 返回的结果
        返回值：struct dirent :
   代表读取到的文件信息，读取到末尾或失败，返回NULL；

        #include <dirent.h>
        #include <sys/types.h>

        int closedir(DIR *dirp);
        作用： 关闭目录

 */

#include "string.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// 获取目录下所有普通文件的个数
int getFIleNumber(const char *path) {
  DIR *dir = opendir(path);
  if (dir == NULL) {
    perror("opendir");
    exit(0);
  }

  struct dirent *prt;
  int total_number = 0;

  while ((prt = readdir(dir))) {
    // 获取名称
    char *dname = prt->d_name;

    // 忽略 ./ 和 ../ 两个目录
    if (strcmp(dname, ".") == 0 || strcmp(dname, "..") == 0) {
      continue;
    }

    // 判断是否是普通文件还是目录
    if (prt->d_type == DT_DIR) {
      // 目录， 需要继续读取这个目录
      char newPath[256];
      sprintf(newPath, "%s/%s", path, dname);
      total_number += getFIleNumber(newPath);
    }

    if (prt->d_type == DT_REG) {
      /// 普通文件
      ++total_number;
    }
  }

  // 关闭目录
  closedir(dir);
  return total_number;
}

// 读取某个目录下文件的个数
int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("%s path\n", argv[0]);
    return -1;
  }

  int file_numbers = getFIleNumber(argv[1]);
  printf("file numbers: %d\n", file_numbers);

  return 0;
}
