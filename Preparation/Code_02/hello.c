#include <stdio.h>
#include <unistd.h>

int main() {
  printf("begin: \n");

  if (fork() != 0) {

    printf("In parent process, pid = %d, ppid = %d\n", getpid(), getppid());

    int i;
    for (i = 0; i < 10; i++) {
      printf("i = %d\n", i);
      sleep(i);
    }
  } else {
    printf("In child process, pid = %d, ppid = %d\n", getpid(), getppid());

    int j;
    for (j = 0; j < 10; j++) {
      printf("j = %d\n", j);
      sleep(j);
    }
  }

  return 0;
}
