#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/signalfd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "krad_radio.h"

static int systemd_notify(const char *data) {
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
    if (fd < 0) {
      return -1;
    }
    sendto(fd, data, strlen(data), MSG_NOSIGNAL, (struct sockaddr*) &un,
     sizeof(un) - sizeof(un.sun_path) + strlen(e));
    close(fd);
    return 0;
  }
  return -1;
}

static void daemonize() {
  pid_t pid;
  pid_t sid;
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

int main(int argc, char *argv[]) {
  kr_radio *radio;
  int sfd;
  struct signalfd_siginfo fdsi;
  ssize_t s;
  sigset_t mask;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [station sysname]\n", argv[0]);
    exit(1);
  }
  if ((getuid() == 0) || (geteuid() == 0)) {
    fprintf(stderr, "Should not be run as root!\n");
    exit(1);
  }
  if (!kr_sysname_valid(argv[1])) exit(1);
  sigemptyset(&mask);
  sigfillset(&mask);
  if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
    failfast("Daemon: Could not set signal mask!");
  }
  sfd = signalfd(-1, &mask, SFD_CLOEXEC);
  if (sfd == -1) {
    failfast("Daemon: could not setup signalfd");
  }
  radio = kr_radio_create(argv[1]);
  if (!radio) {
    exit(1);
  }
  if (systemd_notify("READY=1") != 0) {
    daemonize();
  }
  printk("Daemon: Waiting on signals..");
  for (;;) {
    s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
    if (s != sizeof(struct signalfd_siginfo)) {
      failfast("Daemon: Error reading signalfd");
    }
    switch (fdsi.ssi_signo) {
      case SIGHUP:
        printk("Daemon: Got HANGUP Signal!");
        break;
      case SIGINT:
        printk("\nDaemon: Got INT Signal!");
        printk("Daemon: Shutting down");
        kr_radio_destroy(radio);
        return 0;
      case SIGTERM:
        printk("Daemon: Got TERM Signal!");
        printk("Daemon: Shutting down");
        kr_radio_destroy(radio);
        return 0;
      default:
        printk("Daemon: Got Signal %u", fdsi.ssi_signo);
    }
  }
  return 1;
}
