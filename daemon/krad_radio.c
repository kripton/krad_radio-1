#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "krad_radio.h"

static int launched_by_systemd() {
  const char *e;
  e = getenv("NOTIFY_SOCKET");
  if (e) {
    return 1;
  }
  return 0;
}

static void systemd_notify(const char *data) {
  const char *e;
  struct sockaddr_un un = { .sun_family = AF_UNIX };
  int fd;
  e = getenv("NOTIFY_SOCKET");
  if (e) {
    strncpy(un.sun_path, e, sizeof(un.sun_path));
    if (un.sun_path[0] == '@') {
      un.sun_path[0] = 0;
    }
    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0) return;
    sendto(fd, data, strlen(data), MSG_NOSIGNAL, (struct sockaddr*) &un,
     sizeof(un) - sizeof(un.sun_path) + strlen(e));
    close(fd);
  }
}

static void daemonize() {
  pid_t pid, sid;
  FILE *refp;
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }
  refp = freopen("/dev/null", "r", stdin);
  if (refp == NULL) {
    exit(EXIT_FAILURE);
  }
  refp = freopen("/dev/null", "w", stdout);
  if (refp == NULL) {
    exit(EXIT_FAILURE);
  }
  refp = freopen("/dev/null", "w", stderr);
  if (refp == NULL) {
    exit(EXIT_FAILURE);
  }
  umask(0);
  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }
  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }
}

static void wait_on_signals() {
  int caught;
  sigset_t mask;
  caught = 0;
  sigemptyset(&mask);
  sigfillset(&mask);
  if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
    failfast("Could not set signal mask!");
  }
  while (1) {
    if (sigwait(&mask, &caught) != 0) {
      failfast("Krad Radio: Error on sigwait!");
    }
    switch (caught) {
      case SIGHUP:
        printk("Got HANGUP Signal!");
        break;
      case SIGINT:
        printk("Got INT Signal!");
        printk("Krad Radio: Shutting down");
        return;
      case SIGTERM:
        printk("Got TERM Signal!");
        printk("Krad Radio: Shutting down");
        return;
      default:
        printk("Krad Radio: Got Signal %d", caught);
    }
  }
}

int main(int argc, char *argv[]) {
  kr_radio *radio;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [station sysname]\n", argv[0]);
    exit(1);
  }
  if ((getuid() == 0) || (geteuid() == 0)) {
    fprintf(stderr, "Should not be run as root!\n");
    exit(1);
  }
  if (!kr_sysname_valid(argv[1])) exit(1);
  radio = kr_radio_create(argv[1]);
  if (!radio) exit(1);
  if (launched_by_systemd()) {
    systemd_notify("READY=1");
  } else {
    daemonize();
  }
  wait_on_signals();
  kr_radio_destroy(radio);
  return 0;
}
