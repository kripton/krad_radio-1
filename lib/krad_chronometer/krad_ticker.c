#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include "krad_system.h"
#include "krad_ticker.h"

struct kr_ticker {
  int fd;
  int pd;
  struct timespec start;
  uint64_t tick_ns;
  uint64_t ticks;
};

static inline uint64_t ts_to_nsec(struct timespec ts) {
  return (ts.tv_sec * 1000000000LL) + ts.tv_nsec;
}

static inline struct timespec nsec_to_ts(uint64_t nsecs) {
  struct timespec ts;
  ts.tv_sec = nsecs / (1000000000LL);
  ts.tv_nsec = nsecs % (1000000000LL);
  return ts;
}

struct timespec kr_ticker_add_ns(struct timespec ts, uint64_t ns) {
  uint64_t nsecs = ts_to_nsec(ts);
  nsecs += ns;
  return nsec_to_ts(nsecs);
}

struct timespec kr_ticker_add_ms(struct timespec ts, uint64_t ms) {
  return kr_ticker_add_ns(ts, ms * 1000000);
}

int kr_ticker_destroy(kr_ticker *ticker) {
  if (ticker == NULL) return -1;
  if (ticker->fd != -1) close(ticker->fd);
  ticker->fd = -1;
  if (ticker->pd != -1) close(ticker->fd);
  ticker->pd = -1;
  free(ticker);
  return 0;
}

kr_ticker *kr_ticker_create(int num, int den) {
  kr_ticker *ticker;
  struct epoll_event ev;
  int ret;
  ticker = kr_allocz(1, sizeof(kr_ticker));
  ticker->tick_ns = (1000000000 / num) * den;
  ticker->fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  ticker->pd = epoll_create1(EPOLL_CLOEXEC);
  ev.events = EPOLLIN;
  ev.data.fd = ticker->fd;
  ret = epoll_ctl(ticker->pd, EPOLL_CTL_ADD, ev.data.fd, &ev);
  if ((ret != 0) && (ticker->pd != -1) && (ticker->fd != -1)) {
    printke("Ticker: epoll timerfd fail");
    kr_ticker_destroy(ticker);
    return NULL;
  }
  return ticker;
}

/*
krad_ticker_t *krad_ticker_throttle_create() {
  krad_ticker_t *ticker;
  ticker = kr_allocz(1, sizeof (krad_ticker_t));
  return ticker;
}
*/

int kr_ticker_start(kr_ticker *ticker) {
  if (ticker == NULL) return -1;
  ticker->ticks = 0;
  clock_gettime(CLOCK_MONOTONIC, &ticker->start);
  return 0;
}

int kr_ticker_start_at(kr_ticker *ticker, struct timespec start_time) {
  if (ticker == NULL) return -1;
  ticker->ticks = 0;
  memcpy(&ticker->start, &start_time, sizeof(struct timespec));
  kr_ticker_wait(ticker);
  return 0;
}

/*
void krad_ticker_throttle(krad_ticker_t *ticker, uint64_t ms) {
  ticker->total_ms += ms;
  ticker->wakeup_time = timespec_add_ms (ticker->start_time, ticker->total_ms);
  if (clock_nanosleep (CLOCK_MONOTONIC,
                       TIMER_ABSTIME,
                       &ticker->wakeup_time,
                       NULL)) {
    failfast ("Krad Ticker: error while clock nanosleeping");
  }
}
*/
int kr_ticker_wait(kr_ticker *ticker) {
  int ret;
  ssize_t s;
  int n;
  uint64_t exp;
  struct itimerspec new_wakeup;
  struct timespec wakeup;
  struct epoll_event events[4];
  if (ticker == NULL) return -1;
  wakeup = kr_ticker_add_ns(ticker->start, ticker->tick_ns * ticker->ticks);
  if (0){
    if (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wakeup, NULL)) {
      failfast("Ticker: error while clock nanosleeping");
    }
  } else {
    memset(&new_wakeup, 0, sizeof(new_wakeup));
    new_wakeup.it_value = wakeup;
    ret = timerfd_settime(ticker->fd, TFD_TIMER_ABSTIME, &new_wakeup, NULL);
    if (ret == -1) return -2;

    n = epoll_wait(ticker->pd, events, 4, -1);
    if (n < 1) return -3;
    if (ticker->fd != events[0].data.fd) {
      return -4;
    }
  }
  s = read(ticker->fd, &exp, sizeof(uint64_t));
  if (s != sizeof(uint64_t)) return -5;
  ticker->ticks++;
  return 0;
}
