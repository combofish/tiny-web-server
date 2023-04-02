/**
       #include <sys/types.h>
       #include <unistd.h>

       pid_t fork(void);
       作用： 用于创建子进程
       返回值： 返回值会返回两次，一次是在父进程中，一次是在子进程中。
       子父进程中返回创建的子进程的ID, 在子进程中返回0,
       在父进程中返回-1， 创建进程失败，会设置errno。

 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int num = 10;

  pid_t pid = fork();

  if (pid > 0) {
    printf("pid: %d\n", pid);
    printf("parent process, pid : %d, ppid: %d\n", getpid(), getppid());

    printf("num: %d\n", num);
    num += 10;
    printf("num + 10: %d\n", num);

  } else if (pid == 0) {
    printf("child process, pid : %d, ppid: %d\n", getpid(), getppid());

    printf("num: %d\n", num);
    num += 100;
    printf("num + 100: %d\n", num);
  }

  for (int i = 0; i < 3; i++) {
    printf("i: %d, pid: %d\n", i, getpid());
    sleep(1);
  }

  return 0;
}
