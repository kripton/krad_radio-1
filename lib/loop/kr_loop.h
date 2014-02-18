#include <sys/epoll.h>

typedef struct kr_loop kr_loop;
typedef struct kr_harness kr_harness;
typedef struct kr_event kr_event;
typedef int (kr_handler)(kr_event *);

#ifndef KR_LOOP_H
#define KR_LOOP_H

struct kr_event {
  struct epoll_event ev;
  void *user;
};

struct kr_harness {
  int fd;
  void *user;
  kr_handler *handler;
};

int kr_loop_remove(kr_loop *loop, int fd);
int kr_loop_harness(kr_loop *loop, kr_harness *harness);
int kr_loop_destroy(kr_loop *loop);
kr_loop *kr_loop_create();

#endif
