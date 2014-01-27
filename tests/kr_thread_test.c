#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sched.h>

#define __exit_thread_inline(val) \
  asm volatile ("syscall" :: "a" (__NR_exit), "D" (val))

static void thread_exit() {
  __exit_thread_inline(0);
}

static void print_id(char *name) {
  pid_t pid;
  pid_t tid;
  pid = getpid();
  tid = syscall(SYS_gettid);
  printf("%s: PID: %d TID: %d\n", name, pid, tid);
}

static int run(void *arg) {
  char *name;
  name = (char *)arg;
  print_id(name);
  thread_exit();
  return -1;
}

int kr_thread_create(char *name) {
  int ret;
  size_t stack_sz;
  int flags;
  void *arg;
  void *stack;
  arg = (void *)name;
  flags = 0;
  flags |= CLONE_THREAD | CLONE_SIGHAND | CLONE_SYSVSEM;
  flags |= CLONE_VM;
  flags |= CLONE_FS | CLONE_FILES | CLONE_IO;
  stack_sz = 8192;
  stack = calloc(1, stack_sz);
  stack += stack_sz;
  ret = clone(run, stack, flags, arg);
  return ret;
}

static int test() {
  int thread_id;
  sigset_t mask;
  sigemptyset(&mask);
  sigfillset(&mask);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) != 0) {
    fprintf(stderr, "Could not set signal mask!");
    return -1;
  }
  print_id("main program");
  thread_id = kr_thread_create("Electric");
  if (thread_id == -1) {
    printf("Thread launch Error: %s\n", strerror(errno));
    return -1;
  }
  usleep(500000);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  ret = test();
  if (ret) {
    fprintf(stderr, "Fail.\n");
  } else {
    printf("It worked!\n");
  }
  return ret;
}
