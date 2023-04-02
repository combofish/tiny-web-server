/**
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/**
设置管道非阻塞，
fcntl(fd[0], F_GETFL);  // 获取原来的 flag,;
flags |= O_NONBLOCK;  // 修改 flag 的值
fcntl(fd[0], F_SETFL, flags); // 设置新的 flag.

 */
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
    printf("I am parent process, pid = %d\n", getpid());
    close(pipefd[1]);

    int flages = fcntl(pipefd[0], F_GETFL);
    flages |= O_NONBLOCK;
    fcntl(pipefd[0], F_SETFL, flages);

    char buf[1024] = {0};
    while (1) {
      int len = read(pipefd[0], buf, sizeof(buf));
      printf("len = %d ", len);
      printf("Parent recv ： %s, pid = %d\n", buf, getpid());
      memset(buf, 0, 1024);
      sleep(1);

      // write
      /* char *str = "Hello, I am parent process"; */
      /* write(pipefd[1], str, strlen(str)); */
      /* sleep(1); */
    }

  } else if (pid == 0) {

    // sleep(2);
    // child process
    printf("I am child process, pid = %d", getpid());
    close(pipefd[0]);

    char buf[1024] = {0};
    while (1) {
      char *str = "Hello, I am child process";
      write(pipefd[1], str, strlen(str));
      sleep(3);

      /* int len = read(pipefd[0], buf, sizeof(buf)); */
      /* printf("Child recv ： %s, pid = %d\n", buf, getpid()); */
    }
  }

  return 0;
}
