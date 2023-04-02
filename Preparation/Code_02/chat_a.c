
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  int ret = access("chat_fifo_1", F_OK);
  if (-1 == ret) {
    printf("fifo do not exist, create fifo.\n");
    ret = mkfifo("chat_fifo_1", 0664);
    if (-1 == ret) {
      perror("mkfifo");
      exit(-1);
    }
  }

  ret = access("chat_fifo_2", F_OK);
  if (-1 == ret) {
    printf("fifo do not exist, create fifo.\n");
    ret = mkfifo("chat_fifo_2", 0664);
    if (-1 == ret) {
      perror("mkfifo");
      exit(-1);
    }
  }

  int fd_r = open("chat_fifo_1", O_RDONLY);
  if (-1 == fd_r) {
    perror("open");
    exit(-1);
  }
  printf("Open chat_fifo_1 success, waiting read...\n");

  int fd_w = open("chat_fifo_2", O_WRONLY);
  if (-1 == fd_w) {
    perror("open");
    exit(-1);
  }
  printf("Open chat_fifo_1 success, waiting write...\n");

  char buf[128];
  while (1) {
    memset(buf, 0, 128);
    fgets(buf, 128, stdin);

    ret = write(fd_w, buf, strlen(buf));
    if (-1 == ret) {
      perror("write");
      exit(-1);
    }

    // 5. read data.
    memset(buf, 0, 128);
    ret = read(fd_r, buf, 128);
    if (ret <= 0) {
      perror("read ");
      break;
    }

    printf("A read buf: %s\n", buf);
  }

  close(fd_r);
  close(fd_w);

  return 0;
}
