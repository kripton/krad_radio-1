#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sched.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#include "kr_loop.h"

enum kr_loop_state {
  KR_LOOP_START = 0,
  KR_LOOP_LOOPING,
  KR_LOOP_DONE
};

#define KR_LOOP_NEVENTS 16
#define KR_LOOP_NHARNESSES 16

#define __exit_thread_inline(val) \
  asm volatile ("syscall" :: "a" (__NR_exit), "D" (val))

typedef void (thread_func)(void *arg);
typedef struct kr_thread_params kr_thread_params;
typedef struct kr_thread kr_thread;

struct kr_thread_params {
  thread_func *fn;
  void *arg;
  size_t stack_sz;
};

struct kr_thread {
  void *stack;
  pid_t tid;
  kr_thread_params params;
};

struct kr_loop {
  kr_thread *thread;
  int ed;
  int pd;
  int state;
  kr_harness harness[KR_LOOP_NHARNESSES];
};

static void thread_exit();
static int kr_thread_run(void *arg);
static int kr_thread_free(kr_thread *thread);
static kr_thread *kr_thread_create(kr_thread_params *params);
static void loop_cycle(void *arg);

static void thread_exit() {
  __exit_thread_inline(0);
}

static int kr_thread_run(void *arg) {
  kr_thread *thread;
  thread = (kr_thread *)arg;
  thread->params.fn(thread->params.arg);
  thread_exit();
  return -1;
}

static int kr_thread_free(kr_thread *thread) {
  void *save;
  if (thread == NULL) return -1;
  save = thread;
  memset(thread, 0, sizeof(*thread));
  free(save);
  return 0;
}

static kr_thread *kr_thread_create(kr_thread_params *params) {
  int ret;
  void *top;
  int flags;
  kr_thread thread;
  if (params == NULL) return NULL;
  flags = 0;
  flags |= CLONE_THREAD | CLONE_VM | CLONE_SIGHAND | CLONE_SYSVSEM;
  flags |= CLONE_FS | CLONE_FILES | CLONE_IO;
  thread.params = *params;
  thread.stack = calloc(1, thread.params.stack_sz + sizeof(kr_thread));
  top = thread.stack + thread.params.stack_sz;
  memcpy(top, &thread, sizeof(kr_thread));
  ret = clone(kr_thread_run, top, flags, top);
  if (ret == -1) {
    free(thread.stack);
    return NULL;
  }
  return (kr_thread *)thread.stack;
}

#ifndef printk
#define printk printf
#endif
#ifndef printke
#define printke printf
#endif

static void loop_cycle(void *arg) {
  int n;
  int i;
  kr_loop *loop;
  kr_harness *harness;
  kr_event event;
  struct epoll_event events[KR_LOOP_NEVENTS];
  loop = (kr_loop *)arg;
  loop->state = KR_LOOP_LOOPING;
  printk("Loop: starting\n");
  while (loop->state == KR_LOOP_LOOPING) {
    n = epoll_wait(loop->pd, events, KR_LOOP_NEVENTS, -1);
    if (n < 1) {
      printk("Loop: error on epoll wait\n");
      break;
    }
    for (i = 0; i < n; i++) {
      if (events[i].data.fd == loop->ed) {
        loop->state = KR_LOOP_DONE;
        break;
      } else {
        harness = (kr_harness *)events[i].data.ptr;
        event.ev.data.fd = harness->fd;
        event.ev.events = events[i].events;
        event.user = harness->user;
        harness->handler(&event);
      }
    }
  }
  printk("Loop: exiting\n");
  loop->state = KR_LOOP_DONE;
}

int kr_loop_remove(kr_loop *loop, int fd) {
  int i;
  int ret;
  if (loop == NULL) return -1;
  if (fd < 0) return -2;
  for (i = 0; i < KR_LOOP_NHARNESSES; i++) {
    if (loop->harness[i].fd == fd) {
      ret = epoll_ctl(loop->pd, EPOLL_CTL_DEL, fd, NULL);
      if (ret != 0) {
        printke("Loop: epoll ctl del harness fd to loop pd fail");
        return -5;
      }
      loop->harness[i].fd = 0;
      return 0;
    }
  }
  return -6;
}

int kr_loop_harness(kr_loop *loop, kr_harness *harness) {
  int i;
  int ret;
  struct epoll_event ev;
  if (loop == NULL) return -1;
  if (harness == NULL) return -1;
  if (harness->fd < 0) return -2;
  if (harness->handler == NULL) return -2;
  for (i = 0; i < KR_LOOP_NHARNESSES; i++) {
    if (loop->harness[i].fd == 0) {
      loop->harness[i] = *harness;
      memset(&ev, 0, sizeof(ev));
      ev.events = EPOLLIN;
      ev.data.ptr = &loop->harness[i];
      ret = epoll_ctl(loop->pd, EPOLL_CTL_ADD, harness->fd, &ev);
      if (ret != 0) {
        printke("Loop: epoll ctl add harness fd to loop pd fail");
        return -5;
      }
      return 0;
    }
  }
  return -6;
}

int kr_loop_destroy(kr_loop *loop) {
  if (loop == NULL) return -1;
  printk("Loop: destroy\n");
  uint64_t u;
  int s;
  u = 666;
  s = write(loop->ed, &u, sizeof(uint64_t));
  if (s != sizeof(uint64_t)) {
    printk("Loop: error writing to loop ed\n");
  }
  while (loop->state != KR_LOOP_DONE) {
    usleep(420);
  }
  close(loop->ed);
  close(loop->pd);
  kr_thread_free(loop->thread);
  usleep(420);
  free(loop);
  printk("Loop: destroyed\n");
  return 0;
}

kr_loop *kr_loop_create() {
  int ret;
  kr_loop *loop;
  kr_thread_params params;
  struct epoll_event ev;
  loop = calloc(1, sizeof(*loop));
  params.fn = loop_cycle;
  params.arg = loop;
  params.stack_sz = 1024 * 1024;
  loop->ed = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (loop->ed == -1) {
    printke("Loop: efd evenfd created failed");
    free(loop);
    return NULL;
  }
  loop->pd = epoll_create1(EPOLL_CLOEXEC);
  if (loop->pd == -1) {
    printke("Loop: pd epoll created failed");
    close(loop->ed);
    free(loop);
    return NULL;
  }
  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = loop->ed;
  ret = epoll_ctl(loop->pd, EPOLL_CTL_ADD, loop->ed, &ev);
  if (ret != 0) {
    printke("Loop: epoll ctl add loop ed to loop pd fail");
    close(loop->ed);
    close(loop->pd);
    free(loop);
    return NULL;
  }
  loop->thread = kr_thread_create(&params);
  if (!loop->thread) {
    printf("Thread launch Error: %s\n", strerror(errno));
    close(loop->ed);
    close(loop->pd);
    free(loop);
    return NULL;
  }
  return loop;
}
