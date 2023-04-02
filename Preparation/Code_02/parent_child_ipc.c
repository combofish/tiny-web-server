/**
 实现 ps aux | grep xxx
 父子进程间的通信，

 子进程 ps aux , 进程结束以后，将数据发送到父进程
 父进程， 获取数据，过滤

 pipe()
 execlp()

 将子进程的标准输出重定向到管道的写端， dup2

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int pipefp[2];

  int ret = pipe(pipefp);

  if (-1 == ret) {
    perror("pipe");
    exit(0);
  }

  // 创建子进程
  pid_t pid;
  pid = fork();

  if (pid > 0) {
    // parent process
    // 关闭写端
    close(pipefp[1]);

    // 从管道中读取数据
    char buf[1024] = {0};
    int len = -1;
    while ((len = read(pipefp[0], buf, sizeof(buf) - 1)) > 0) {
      // 过滤数据输出
      printf("%s", buf);
      memset(buf, 0, 1024);
    }

    wait(NULL);

  } else if (pid == 0) {
    // child process
    // 关闭读端
    close(pipefp[0]);

    // 文件描述符重定向 stdout_fileno -> pipefd[1]
    dup2(pipefp[1], STDOUT_FILENO);

    // 执行 ps aux
    execlp("ps", "ps", "aux", NULL);
    perror("execlp");
    exit(0);

  } else {
    perror("fork");
    exit(0);
  }

  return 0;
}
