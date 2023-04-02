#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

  int pipefd[2];
  int ret = pipe(pipefd);

  // 获取管道的大小
  long sz = fpathconf(pipefd[0], _PC_PIPE_BUF);

  printf("pipe size = %ld\n", sz);
  
  return 0;
}
