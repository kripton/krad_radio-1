#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <inttypes.h>
#include <sys/utsname.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/mman.h>

#include "krad_app_client.h"

struct kr_app_client {
  char sysname[64];
  int sd;
  int tcp_port;
  char host[256];
  char api_path[256];
  int api_path_pos;
  int on_linux;
  struct stat info;
  struct utsname unixname;
};

static int kr_app_client_init(kr_app_client *client, int timeout_ms);

int kr_app_client_local(kr_app_client *client) {
  if (client->tcp_port == 0) {
    return 1;
  }
  return 0;
}

int kr_app_client_get_fd(kr_app_client *client) {
  if (client != NULL) {
    return client->sd;
  }
  return -1;
}

void kr_app_disconnect(kr_app_client *client) {
  if (client != NULL) {
    if (client->sd != 0) {
      close (client->sd);
    }
    free(client);
  }
}

kr_app_client *kr_app_connect(char *sysname, int timeout_ms) {
  kr_app_client *client;
  client = kr_allocz(1, sizeof(kr_app_client));
  if (client == NULL) {
    failfast("Krad APP Client mem alloc fail");
    return NULL;
  }
  uname (&client->unixname);
  if (krad_valid_host_and_port (sysname)) {
    krad_get_host_and_port (sysname, client->host, &client->tcp_port);
  } else {
    strncpy (client->sysname, sysname, sizeof (client->sysname));
    if (strncmp(client->unixname.sysname, "Linux", 5) == 0) {
      client->on_linux = 1;
      client->api_path_pos = sprintf(client->api_path, "@krad_radio_%s_api", sysname);
    } else {
      client->api_path_pos = sprintf(client->api_path, "%s/krad_radio_%s_api", "/tmp", sysname);
    }
    if (!client->on_linux) {
      if(stat(client->api_path, &client->info) != 0) {
        kr_app_disconnect(client);
        failfast ("Krad APP Client: API PATH Failure\n");
        return NULL;
      }
    }
  }
  if (kr_app_client_init(client, timeout_ms) == 0) {
    printke ("Krad APP Client: Failed to init!");
    kr_app_disconnect(client);
    return NULL;
  }
  return client;
}

static int kr_app_client_init(kr_app_client *client, int timeout_ms) {
  int rc;
  socklen_t socket_sz;
  char port_string[6];
  struct sockaddr_un unix_saddr;
  struct in6_addr serveraddr;
  struct addrinfo hints;
  struct addrinfo *res;
  res = NULL;
  //FIXME make connect nonblocking
  if (client->tcp_port != 0) {
    //FIXME hrm we don't know the sysname of a remote connect! crazy ?
    //printf ("Krad APP Client: Connecting to remote %s:%d", client->host, client->tcp_port);
    memset(&hints, 0x00, sizeof(hints));
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    rc = inet_pton (AF_INET, client->host, &serveraddr);
    if (rc == 1) {
      hints.ai_family = AF_INET;
      hints.ai_flags |= AI_NUMERICHOST;
    } else {
      rc = inet_pton (AF_INET6, client->host, &serveraddr);
      if (rc == 1) {
        hints.ai_family = AF_INET6;
        hints.ai_flags |= AI_NUMERICHOST;
      }
    }
    snprintf (port_string, 6, "%d", client->tcp_port);
    rc = getaddrinfo (client->host, port_string, &hints, &res);
    if (rc != 0) {
       printf ("Krad APP Client: Host not found --> %s\n", gai_strerror(rc));
       return 0;
    }
    client->sd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
    if (client->sd < 0) {
      printf ("Krad APP Client: Socket Error");
      if (res != NULL) {
        freeaddrinfo (res);
        res = NULL;
      }
      return 0;
    }
    rc = connect (client->sd, res->ai_addr, res->ai_addrlen);
    if (rc < 0) {
      printf ("Krad APP Client: Remote Connect Error\n");
      if (res != NULL) {
        freeaddrinfo (res);
        res = NULL;
      }
      return 0;
    }
    if (res != NULL) {
      freeaddrinfo (res);
      res = NULL;
    }
  } else {
    client->sd = socket (AF_UNIX, SOCK_STREAM, 0);
    if (client->sd == -1) {
      failfast ("Krad APP Client: socket fail");
      return 0;
    }
    memset(&unix_saddr, 0x00, sizeof(unix_saddr));
    unix_saddr.sun_family = AF_UNIX;
    snprintf (unix_saddr.sun_path, sizeof(unix_saddr.sun_path), "%s", client->api_path);
    socket_sz = sizeof(unix_saddr) - sizeof(unix_saddr.sun_path) + strlen(unix_saddr.sun_path);
    if (client->on_linux) {
      unix_saddr.sun_path[0] = '\0';
    }
    if (connect (client->sd, (struct sockaddr *) &unix_saddr, socket_sz) == -1) {
      close (client->sd);
      client->sd = 0;
      printke ("Krad APP Client: Can't connect to socket %s", client->api_path);
      return 0;
    }
  }
  krad_system_set_socket_nonblocking(client->sd);
  return client->sd;
}

int kr_app_client_send_fd(kr_app_client *client, int fd) {
  krad_system_set_socket_blocking (client->sd);
  char buf[1];
  struct iovec iov;
  struct msghdr msg;
  struct cmsghdr *cmsg;
  int n;
  char cms[CMSG_SPACE(sizeof(int))];
  buf[0] = 0;
  iov.iov_base = buf;
  iov.iov_len = 1;
  memset(&msg, 0, sizeof msg);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = (caddr_t)cms;
  msg.msg_controllen = CMSG_LEN(sizeof(int));
  cmsg = CMSG_FIRSTHDR(&msg);
  cmsg->cmsg_len = CMSG_LEN(sizeof(int));
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  memmove(CMSG_DATA(cmsg), &fd, sizeof(int));
  if ((n=sendmsg(client->sd, &msg, 0)) != iov.iov_len) {
    krad_system_set_socket_nonblocking (client->sd);
    return 0;
  }
  return 1;
}
