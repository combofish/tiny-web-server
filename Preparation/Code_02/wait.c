/**

 NAME
       wait, waitpid, waitid - wait for process to change state

 SYNOPSIS
       #include <sys/types.h>
       #include <sys/wait.h>

       pid_t wait(int *wstatus);
       功能：
 等待任意一个子进程结束，如果任意一个子进程结束了，此函数会回收子进程。 参数：
 进程退出时的状态信息，传入 int 类型的地址， 传出参数。 返回值：
         - 成功，返回被回收的子进程的 id.
         - 失败， -1， （所有的子进程都结束，调用函数失败）

         调用 wait
 函数的进程会被挂起（阻塞），知道它的一个子进程退出或者收到一个不能被忽略的信号时，
         如果没有子进程了，函数立刻返回，返回 -1，
 如果子进程都结束了，也会立刻返回

       pid_t waitpid(pid_t pid, int *wstatus, int options);


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
      sleep(2);
      /* int ret = wait(NULL); */

      int st;
      int ret = wait(&st);
      if (ret == -1)
        break;

      if (WIFEXITED(st)) {
        printf("exit with code: %d\n", WEXITSTATUS(st));
      }

      if (WIFSIGNALED(st)) {
        printf("exit with kill: %d\n", WTERMSIG(st));
      }

      printf("Chile process die, pid = %d\n", ret);

      printf("I am parent process, pid = %d\n", getpid());
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
