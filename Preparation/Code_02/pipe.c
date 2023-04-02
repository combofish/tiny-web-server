/**
NAME
       pipe, pipe2 - create pipe

SYNOPSIS
       #include <unistd.h>

       On Alpha, IA-64, MIPS, SuperH, and SPARC/SPARC64; see NOTES
       struct fd_pair {
           long fd[2];
       };
       struct fd_pair pipe();

       On all other architectures
       int pipe(int pipefd[2]);
       功能： 创建一个匿名管道，用来进程间通信。
       参数； int pipefd[2] 这个数组是一个传出参数。pipefd[0]
对应的是管道的读端，pipefd[1] 对应的是管道的写端。 返回值，成功 0, 失败 -1。

       管道默认是阻塞的，如果管道中没有数据，read 阻塞，如果管道满了， write
阻塞。 注意： 匿名管道只能用于具有关系的进程之间的通信（父子进程，兄弟进程）。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  // 子进程发送数据，父进程读取数据并输出

  // 在 fork 之前创建管道 pipe
  int pipefd[2];
  int ret = pipe(pipefd);
  if (-1 == ret) {
    perror("pipe");
    exit(0);
  }

  pid_t pid = fork();
  if (pid > 0) {
    printf("I am parent process, pid = %d", getpid());
    char buf[1024] = {0};

    while (1) {
      int len = read(pipefd[0], buf, sizeof(buf));
      printf("Parent recv ： %s, pid = %d\n", buf, getpid());

      // write
      char *str = "Hello, I am parent process";
      write(pipefd[1], str, strlen(str));
      sleep(1);
    }

  } else if (pid == 0) {

    // sleep(2);
    // child process
    printf("I am child process, pid = %d", getpid());
    char buf[1024] = {0};
    while (1) {
      char *str = "Hello, I am child process";
      write(pipefd[1], str, strlen(str));
      sleep(1);

      int len = read(pipefd[0], buf, sizeof(buf));
      printf("Child recv ： %s, pid = %d\n", buf, getpid());
    }
  }

  return 0;
}
