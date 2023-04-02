/**

       #include <unistd.h>


       int execl(const char *pathname, const char *arg, ...);
       int execlp(const char *file, const char *arg, ...);
       int execle(const char *pathname, const char *arg, ...);
       int execv(const char *pathname, char *const argv[]);
       int execvp(const char *file, char *const argv[]);
       int execvpe(const char *file, char *const argv[],
                       char *const envp[]);

       path, 推荐使用绝对路径。
       arg：
   执行可执行的间的参数列表，第一个参数一般是可执行文件的名称，参数列表以空（NULL）结尾。

       返回值： 只有在出错的时候才有返回值，并设置 errno,
   如果调用成功，没有返回值。



 */

#include <stdio.h>
#include <unistd.h>

int main() {

  // 创建一个子进程，在子进程中执行 exec 函数族中的函数。

  pid_t pid = fork();
  if (pid > 0) {
    printf("I am parent process, pid = %d, ppid = %d\n", getpid(), getppid());
    sleep(1);
  } else if (pid == 0) {
    // child
    //    execl("hello", "hello", NULL);

    execl("/bin/ps", "ps", "aux", NULL);
    printf("I am child process, pid = %d\n", getpid());
  }

  for (int i = 0; i < 4; i++) {
    printf("i = %d, pid = %d\n", i, getpid());
  }
  return 0;
}
