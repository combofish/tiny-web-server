/**
 NAME
       wait, waitpid, waitid - wait for process to change state

SYNOPSIS
       #include <sys/types.h>
       #include <sys/wait.h>

       pid_t wait(int *wstatus);

       pid_t waitpid(pid_t pid, int *wstatus, int options);

       功能： 回收指定进程号的子进程，可以设置是否阻塞。
       参数：pid: pid > 0, 表示某个子进程的pid. pid = 0,
回收当前进程组的所有子进程。 pid = -1, 表示回收所有的子进程，相当于 wait(),
(最常用)。 pid < -1, 回收某个进程组的组id的绝对值，回收指定进程组中的子进程。
参数 options 设置阻塞和非阻塞， 0, 阻塞， WNOHANG 非阻塞。

       返回值: >0 返回子进程的id, =0 options = WNOHANG, 表示还有子进程活着，= -1
表示错误，没有子进程了。

*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  // 有一个父进程，产生5个子进程。
  pid_t pid;

  for (int i = 0; i < 5; i++) {
    pid = fork();
    if (pid == 0)
      break;
  }

  if (pid > 0) {
    // parent
    while (1) {
      printf("I am parent process, pid = %d\n", getpid());

      sleep(2);
      /* int ret = wait(NULL); */

      int st;
      //      int ret = waitpid(-1, &st, 0);

      int ret = waitpid(-1, &st, WNOHANG);
      if (ret == -1) {
        break;
      } else if (ret == 0) {
        // 还有子进程存在
        continue;
      } else if (ret > 0) {

        if (WIFEXITED(st)) {
          printf("exit with code: %d\n", WEXITSTATUS(st));
        }

        if (WIFSIGNALED(st)) {
          printf("exit with kill: %d\n", WTERMSIG(st));
        }

        printf("Chile process die, pid = %d\n", ret);
      }

      /* if (ret == -1) */
      /*   break; */

      /* if (WIFEXITED(st)) { */
      /*   printf("exit with code: %d\n", WEXITSTATUS(st)); */
      /* } */

      /* if (WIFSIGNALED(st)) { */
      /*   printf("exit with kill: %d\n", WTERMSIG(st)); */
      /* } */

      /* printf("Chile process die, pid = %d\n", ret); */

      /* printf("I am parent process, pid = %d\n", getpid()); */
    }

  } else if (pid == 0) {
    // child
    while (1) {
      printf("I am child process, pid = %d, ppid = %d\n", getpid(), getppid());
      sleep(1);
    }

    // exit(1);
    exit(0);
    //}
  }
  return 0;
}
