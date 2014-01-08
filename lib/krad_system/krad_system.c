#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <pthread.h>
#include <inttypes.h>
#include <sys/prctl.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>

#include "krad_system.h"

static int print_lock;

int krad_control_init(krad_control_t *krad_control) {
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, krad_control->sockets)) {
    printke("Krad System: can't socketpair errno: %d", errno);
    krad_control->sockets[0] = 0;
    krad_control->sockets[1] = 0;
    return -1;
  }
  return 0;
}

int krad_controller_get_client_fd(krad_control_t *krad_control) {
  if ((krad_control->sockets[0] != 0) && (krad_control->sockets[1] != 0)) {
    return krad_control->sockets[1];
  }
  return -1;
}

int krad_controller_get_controller_fd(krad_control_t *krad_control) {
  if ((krad_control->sockets[0] != 0) && (krad_control->sockets[1] != 0)) {
    return krad_control->sockets[1];
  }
  return -1;
}

int krad_controller_client_close(krad_control_t *krad_control) {
  if ((krad_control->sockets[0] != 0) && (krad_control->sockets[1] != 0)) {
    close(krad_control->sockets[1]);
    krad_control->sockets[1] = 0;
    return 0;
  }
  return -1;
}

int krad_controller_client_wait(krad_control_t *krad_control, int timeout) {
  struct pollfd pollfds[1];
  pollfds[0].fd = krad_control->sockets[1];
  pollfds[0].events = POLLIN;
  if (poll(pollfds, 1, timeout) != 0) {
    return 1;
  }
  return 0;
}

int krad_controller_shutdown(krad_control_t *krad_control, pthread_t *thread,
 int timeout) {
  struct pollfd pollfds[1];
  int ret;
  timeout = (timeout / 2) + 2;
  pollfds[0].fd = krad_control->sockets[0];
  pollfds[0].events = POLLOUT;
  if (poll(pollfds, 1, timeout) == 1) {
    ret = write(krad_control->sockets[0], "0", 1);
    if (ret == 1) {
      pollfds[0].fd = krad_control->sockets[0];
      pollfds[0].events = POLLIN;
      if (poll(pollfds, 1, timeout) == 1) {
        pthread_join(*thread, NULL);
        close(krad_control->sockets[0]);
        krad_control->sockets[0] = 1;
        return 1;
      }
    }
  }
  return 0;
}

void krad_controller_destroy(krad_control_t *krad_control, pthread_t *thread) {
  if (krad_control->sockets[1] != 0) {
    close(krad_control->sockets[1]);
    krad_control->sockets[1] = 0;
  }
  if (*thread) {
    pthread_cancel(*thread);
    pthread_join(*thread, NULL);
  }
  if (krad_control->sockets[1] != 0) {
    close(krad_control->sockets[1]);
    krad_control->sockets[1] = 0;
  }
}

void failfast(char* format, ...) {
  va_list args;
  while (!__sync_bool_compare_and_swap(&print_lock, 0, 1));
  fprintf(stderr, "***FAILURE!: ");
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\nError TS: %"PRIu64"\n", krad_unixtime());
  while (!__sync_bool_compare_and_swap(&print_lock, 1, 0));
  exit(2);
}

void printke(char* format, ...) {
  va_list args;
  while (!__sync_bool_compare_and_swap(&print_lock, 0, 1));
  fprintf(stderr, "***ERROR!: ");
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\nError TS: %"PRIu64"\n", krad_unixtime());
  while (!__sync_bool_compare_and_swap(&print_lock, 1, 0));
}

void printk(char* format, ...) {
  va_list args;
  while (!__sync_bool_compare_and_swap(&print_lock, 0, 1));
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
  printf("\n");
  while (!__sync_bool_compare_and_swap(&print_lock, 1, 0));
}

void kr_systm_get_thread_name(char *name) {
  if ((name == NULL)
   || (prctl(PR_GET_NAME, (unsigned long)name, 0, 0, 0) != 0)) {
    printke("Could not get thread name");
  }
}

void krad_system_set_thread_name(char *name) {
  if ((name == NULL || strlen(name) > 15) ||
      (prctl(PR_SET_NAME, (unsigned long)name, 0, 0, 0) != 0)) {
    printke("Could not set thread name: %s", name);
  }
}

int kr_sys_port_valid (int port) {
  if ((port >= 0) && (port <= 65535)) {
    return 1;
  }
  return 0;
}

uint64_t krad_unixtime() {
  uint64_t seconds;
  struct timeval tv;
  gettimeofday (&tv, NULL);
  seconds = tv.tv_sec;
  return seconds;
}

int dir_exists(char *dir) {
  int err;
  struct stat s;
  err = stat (dir, &s);
  if (err == -1) {
    if(ENOENT == errno) {
      // does not exist
      return 0;
    } else {
      // another error
      return 0;
    }
  } else {
    if(S_ISDIR(s.st_mode)) {
      // it's a directory
      return 1;
    } else {
      // exists but is no dir
      return 0;
    }
  }
  return 0;
}

int krad_system_set_socket_nonblocking(int sd) {
  int ret;
  int flags;
  flags = 0;
  ret = 0;
  flags = fcntl(sd, F_GETFL, 0);
  if (flags == -1) {
    failfast("Krad System: error on syscall fcntl F_GETFL");
    return -1;
  }
  flags |= O_NONBLOCK;
  ret = fcntl(sd, F_SETFL, flags);
  if (ret == -1) {
    failfast("Krad System: error on syscall fcntl F_SETFL");
    return -1;
  }
  return sd;
}

int krad_system_set_socket_blocking(int sd) {
  int ret;
  int flags;
  flags = 0;
  ret = 0;
  flags = fcntl(sd, F_GETFL, 0);
  if (flags == -1) {
    failfast("Krad System: error on syscall fcntl F_GETFL");
    return -1;
  }
  flags = flags & (~O_NONBLOCK);
  ret = fcntl(sd, F_SETFL, flags);
  if (ret == -1) {
    failfast("Krad System: error on syscall fcntl F_SETFL");
    return -1;
  }
  return sd;
}

int kr_sysname_valid(char *sysname) {
  int i = 0;
  char j;
  char requirements[512];
  sprintf(requirements, "sysname's must be atleast %d characters long, only lowercase letters and numbers, "
   "begin with a letter, and no longer than %d characters.",
   KRAD_SYSNAME_MIN, KRAD_SYSNAME_MAX);

  if ((sysname == NULL) || (strlen(sysname) < KRAD_SYSNAME_MIN)) {
    fprintf(stderr, "sysname %s is invalid, too short!\n", sysname);
    fprintf(stderr, "%s\n", requirements);
    return 0;
  }
  if (strlen(sysname) > KRAD_SYSNAME_MAX) {
    fprintf(stderr, "sysname %s is invalid, too long!\n", sysname);
    fprintf(stderr, "%s\n", requirements);
    return 0;
  }
  j = sysname[i];
  if (!((isalpha(j)) && (islower(j)))) {
    fprintf(stderr, "sysname %s is invalid, must start with a lowercase letter!\n", sysname);
    fprintf(stderr, "%s\n", requirements);
    return 0;
  }
  i++;
  while (sysname[i]) {
    j = sysname[i];
      if (!isalnum(j)) {
        fprintf(stderr, "sysname %s is invalid, alphanumeric only!\n", sysname);
        fprintf(stderr, "%s\n", requirements);
        return 0;
      }
      if (isalpha(j)) {
        if (!islower(j)) {
          fprintf(stderr, "Sysname %s is invalid lowercase letters only!\n", sysname);
          fprintf(stderr, "%s\n", requirements);
          return 0;
        }
      }
    i++;
  }
  return 1;
}

void krad_get_host_and_port(char *string, char *host, int *port) {
  *port = atoi (strrchr(string, ':') + 1);
  memset (host, '\0', 128);
  memcpy (host, string, strlen(string) - strlen(strrchr(string, ':')));
  //printk ("Got host %s and port %d\n", host, *port);
}

//FIXME not actually any good

int krad_valid_host_and_port(char *string) {
  int port;
  char host[128];
  if (strchr(string, ':') != NULL) {
    port = atoi(strrchr(string, ':') + 1);
    memset(host, '\0', 128);
    memcpy(host, string, strlen(string) - strlen(strrchr(string, ':')));
    //if (((port >= 0) && (port <= 65535)) && (strlen(host) > 3)) {
    if (kr_sys_port_valid(port)) {
      //printk("Got host %s and port %d\n", host, port);
      return 1;
    } else {
      printke("INVALID host %s port %d", host, port);
    }
  }
  return 0;
}

int krad_system_is_adapter(char *adapter) {
  int sd;
  struct ifconf ifconf;
  struct ifreq ifreq[20];
  int interfaces;
  int i;
  // Create a socket or return an error.
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    failfast("socket");
  }
  // Point ifconf's ifc_buf to our array of interface ifreqs.
  ifconf.ifc_buf = (char *)ifreq;
  // Set ifconf's ifc_len to the length of our array of interface ifreqs.
  ifconf.ifc_len = sizeof ifreq;
  //  Populate ifconf.ifc_buf (ifreq) with a list of interface names and addresses.
  if (ioctl(sd, SIOCGIFCONF, &ifconf) == -1) {
    failfast("ioctl");
  }
  // Divide the length of the interface list by the size of each entry.
  // This gives us the number of interfaces on the system.
  interfaces = ifconf.ifc_len / sizeof(ifreq[0]);
  for (i = 0; i < interfaces; i++) {
    if (strncmp(adapter, ifreq[i].ifr_name, strlen(adapter)) == 0) {
      close(sd);
      return 1;
    }
  }
  close(sd);
  return 0;
}
