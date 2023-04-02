#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/**

 */
int main(int argc, char *argv[]) {

  // 判断输入的参数是否正确
  if (argc < 2) {
    printf("%s filename\n", argv[0]);
    return -1;
  }

  // 通过 stat 过去文件的信息
  struct stat buffstat;
  int ret = stat(argv[1], &buffstat);
  if (-1 == ret) {
    perror("stat err");
    return -1;
  }

  // 获取文件类型和文件权限
  char permit[11] = {0};
  switch (buffstat.st_mode & S_IFMT) {
  case S_IFLNK:
    permit[0] = '1';
    break;
  case S_IFDIR:
    permit[0] = 'd';
    break;
  case S_IFREG:
    permit[0] = '-';
    break;
  case S_IFBLK:
    permit[0] = 'b';
    break;
  case S_IFSOCK:
    permit[0] = 's';
    break;
  case S_IFCHR:
    permit[0] = 'c';
    break;
  case S_IFIFO:
    permit[0] = 'p';
    break;
  default:
    permit[0] = '?';
    break;
  }

  // 判断文件的访问权限

  permit[1] = (buffstat.st_mode & S_IRUSR) ? 'r' : '-';
  permit[2] = (buffstat.st_mode & S_IWUSR) ? 'w' : '-';
  permit[3] = (buffstat.st_mode & S_IXUSR) ? 'x' : '-';

  permit[4] = (buffstat.st_mode & S_IRGRP) ? 'r' : '-';
  permit[5] = (buffstat.st_mode & S_IWGRP) ? 'w' : '-';
  permit[6] = (buffstat.st_mode & S_IXGRP) ? 'x' : '-';

  permit[7] = (buffstat.st_mode & S_IROTH) ? 'r' : '-';
  permit[8] = (buffstat.st_mode & S_IWOTH) ? 'w' : '-';
  permit[9] = (buffstat.st_mode & S_IXOTH) ? 'x' : '-';

  // 硬连接数
  int linkNum = buffstat.st_nlink;
  char *fileUser = getpwuid(buffstat.st_uid)->pw_name;
  char *fileGroup = getpwuid(buffstat.st_gid)->pw_name;

  // 文件大小
  long int fileSize = buffstat.st_size;

  // 获取修改时间
  char *changeTime = ctime(&buffstat.st_mtime);
  char mtime[512] = {0};
  strncpy(mtime, changeTime, strlen(changeTime) - 1);

  char buf[1024];
  sprintf(buf, "%s %d %s %s %ld %s %s", permit, linkNum, fileUser, fileGroup,
          fileSize, mtime, argv[1]);

  printf("%s\n", buf);
  return 0;
}
