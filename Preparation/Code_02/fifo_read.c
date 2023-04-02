/**
   read from fifo file.

 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

  int fd = open("fifo1", O_RDONLY);
  if (-1 == fd) {
    perror("open");
    exit(0);
  }

  while (1) {
    char buf[1024] = {0};
    int len = read(fd, buf, sizeof(buf));
    if (0 == len) {

      printf("read out connection off\n");
      break;
    }
    printf("read buf : %s\n", buf);
  }

  close(fd);

  return 0;
}
