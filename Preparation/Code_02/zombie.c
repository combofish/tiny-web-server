
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  pid_t pid = fork();
  if (pid > 0) {

    while (1) {
      printf("I am parent process, pid = %d, ppid = %d\n", getpid(), getppid());
      sleep(1);
    }

  } else {

    printf("I am child process, pid = %d, ppid = %d\n", getpid(), getppid());
  }

  for (int i = 0; i < 5; i++) {
    printf("i = %d, pid = %d, ppid = %d\n", i, getpid(), getppid());
  }

  return 0;
}
