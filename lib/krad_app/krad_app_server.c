#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#define _GNU_SOURCE
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <sys/epoll.h>
#include <sys/signalfd.h>
#include <sys/syscall.h>
#include "krad_app_server.h"
#include "krad_pool.h"

#define KR_APP_EVENTS_MAX 16

typedef enum {
  KR_APP_STARTING = -1,
  KR_APP_RUNNING,
  KR_APP_DO_SHUTDOWN,
  KR_APP_SHUTINGDOWN,
} app_state;

struct kr_app_server {
  int sd;
  int pd;
  int app_pd;
  kr_pool *client_pool;
  kr_router *router;
  uint64_t num_clients;
  int socket_count;
  kr_app_server_client *current_client;
  int sfd;
  uint64_t start_time;
  app_state state;
  krad_control_t control;
  pid_t pid;
  pid_t tid;
  pthread_t thread;
};

struct kr_app_server_client {
  int sd;
  int valid;
  int local;
  kr_io2_t *in;
  kr_io2_t *out;
};

/*
  struct kr_app_server_client_setup {
  //callback for broadcast/serialize?
  int coconut;
};
*/

static int validate_client_header(uint8_t *header, size_t sz);
static int pack_server_header(uint8_t *buffer, size_t max);
static int local_client_pack_crate(uint8_t *buffer, kr_crate2 *crate, size_t max);
static int local_client_get_crate(kr_crate2 *crate, kr_io2_t *in);
static int handle_client(kr_app_server *server, kr_app_server_client *client);
static int validate_client(kr_app_server_client *client);
static void disconnect_client(kr_app_server *server, kr_app_server_client *client);
static kr_app_server_client *accept_client(kr_app_server *server);
static void *server_loop(void *arg);
static int setup_socket(char *appname, char *sysname);

static int validate_client_header(uint8_t *header, size_t sz) {
  kr_ebml2_t ebml;
  int ret;
  char doctype[32];
  uint32_t version;
  uint32_t read_version;
  size_t app_client_doctype_len;
  app_client_doctype_len = strlen(KR_APP_CLIENT_DOCTYPE);
  kr_ebml2_set_buffer(&ebml, header, sz);
  ret = kr_ebml2_unpack_header(&ebml, doctype, 32, &version, &read_version);
  if (ret > 0) {
    if ((version == KR_APP_DOCTYPE_VERSION)
     && (read_version == KR_APP_DOCTYPE_READ_VERSION)
     && (strlen(doctype) == app_client_doctype_len)
     && (strncmp(doctype, KR_APP_CLIENT_DOCTYPE, app_client_doctype_len) == 0)) {
        return ret;
    } else {
      printke("App Server: Header validation failed - mismatch %u %u %s",
       version, read_version, doctype);
    }
  } else {
    printke("App Server: Header validation failed -  %d", ret);
  }
  return -1;
}

static int pack_server_header(uint8_t *buffer, size_t max) {
  kr_ebml2_t ebml;
  kr_ebml2_set_buffer(&ebml, buffer, max);
  kr_ebml_pack_header(&ebml, KR_APP_SERVER_DOCTYPE, KR_APP_DOCTYPE_VERSION,
   KR_APP_DOCTYPE_READ_VERSION);
  return ebml.pos;
}

static int local_client_pack_crate(uint8_t *buffer, kr_crate2 *crate, size_t max) {
  kr_ebml2_t ebml;
  uint8_t *ebml_crate;
  printk("crate packing");
  kr_ebml2_set_buffer(&ebml, buffer, max);
  //if (kr_crate2_valid(info) < 0) {
  // return -1;
  //}
  kr_ebml2_start_element(&ebml, KR_EID_CRATE, &ebml_crate);
  kr_crate2_to_ebml(&ebml, crate);
  kr_ebml2_finish_element(&ebml, ebml_crate);
  return ebml.pos;
}

static int local_client_get_crate(kr_crate2 *crate, kr_io2_t *in) {
  kr_ebml2_t ebml;
  uint32_t element;
  uint64_t size;
  int ret;
  if (!(kr_io2_has_in(in))) {
    return 0;
  }
  kr_ebml2_set_buffer(&ebml, in->rd_buf, in->len);
  ret = kr_ebml2_unpack_id(&ebml, &element, &size);
  if (ret < 0) {
    printke("App Server: Full crate EBML ID Not found");
    return 0;
  }
  size += ebml.pos;
  if (in->len < size) {
    printke("App Server: Crate not full...");
    return 0;
  }
  if (element == KR_EID_CRATE) {
    ret = kr_crate2_fr_ebml(&ebml, crate);
    if (ret == 0) {
      char string[8192];
      ret = kr_crate2_to_text(string, crate, sizeof(string));
      if (ret > 0) {
        printk("App Server: %"PRIu64" byte crate: \n%s\n", size, string);
      }
      kr_io2_pulled(in, ebml.pos);
      return 1;
    }
  }
  return 0;
}

static int handle_client(kr_app_server *server, kr_app_server_client *client) {
  int ret;
  kr_crate2 crate;
  if (client->local) {
    if (!client->valid) {
      ret = validate_client(client);
      if (ret != 1) {
        return 1;
      }
    } else {
      while (local_client_get_crate(&crate, client->in)) {
        kr_router_handle(server->router, &crate);
      }
    }
  }
  return 0;
}

static int validate_client(kr_app_server_client *client) {
  int ret;
  ret = validate_client_header(client->in->rd_buf, client->in->len);
  if (ret > 0) {
    kr_io2_pulled(client->in, ret);
    client->valid = 1;
    printk("client is valid");
    ret = pack_server_header(client->out->buf, client->out->space);
    if (ret < 1) {
      printke("App Server: Error packing client header");
    } else {
      printk("packed server header %d", ret);
      kr_io2_advance(client->out, ret);
      return 1;
    }
  }
  printke("App Server: Invalid client!");
  return -1;
}

static void disconnect_client(kr_app_server *server, kr_app_server_client *client) {
  int ret;
  ret = epoll_ctl(server->pd, EPOLL_CTL_DEL, client->sd, NULL);
  if (ret != 0) {
    printke("App Server: remove client from epoll on disconnect failed!");
  }
  close(client->sd);
  kr_io2_destroy(&client->in);
  kr_io2_destroy(&client->out);
  kr_pool_recycle(server->client_pool, client);
  server->num_clients--;
  printk("App Server: Client disconnected");
}

static kr_app_server_client *accept_client(kr_app_server *server) {
  struct epoll_event ev;
  kr_app_server_client *client;
  struct sockaddr_un sin;
  socklen_t slen;
  int ret;
  slen = sizeof(sin);
  client = kr_pool_slice(server->client_pool);
  if (client == NULL) {
    printke("App Server: Overloaded can't accept client");
    return NULL;
  }
  client->sd = accept4(server->sd, (struct sockaddr *)&sin, &slen,
   SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (client->sd < 0) {
    kr_pool_recycle(server->client_pool, client);
    printke("App Server: accept4() failed!");
    return NULL;
  }
  memset(&ev, 0, sizeof(struct epoll_event));
  ev.events = EPOLLIN;
  ev.data.ptr = client;
  ret = epoll_ctl(server->pd, EPOLL_CTL_ADD, client->sd, &ev);
  if (ret != 0) {
    kr_pool_recycle(server->client_pool, client);
    printke("App Server: Adding client to epoll after accept4 failed");
    close(client->sd);
    return NULL;
  }
  client->local = 1;
  client->valid = 0;
  client->in = kr_io2_create();
  client->out = kr_io2_create();
  kr_io2_set_fd(client->in, client->sd);
  kr_io2_set_fd(client->out, client->sd);
  server->num_clients++;
  printk("App Server: Local client accepted");
  return client;
}

static int setup_signals(kr_app_server *server) {
  int ret;
  sigset_t mask;
  struct epoll_event ev;
  printk("App Server: Was run from a thread so we handle signals");
  sigemptyset(&mask);
  sigfillset(&mask);
  if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
    printke("App Server: Could not set signal mask!");
    return -1;
  }
  server->sfd = signalfd(-1, &mask, SFD_CLOEXEC | SFD_NONBLOCK);
  if (server->sfd == -1) {
    return -1;
  }
  memset(&ev, 0, sizeof(struct epoll_event));
  ev.events = EPOLLIN;
  ev.data.fd = server->sfd;
  ret = epoll_ctl(server->app_pd, EPOLL_CTL_ADD, ev.data.fd, &ev);
  if (ret != 0) {
    printke("App Server: epoll ctl app pd sfd fail");
    return -1;
  }
  return 0;
}

static int handle_app_events(kr_app_server *server) {
  int cfd;
  int fd;
  int n;
  int i;
  printk("App Server: Got app event");
  struct epoll_event events[KR_APP_EVENTS_MAX];
  n = epoll_wait(server->app_pd, events, KR_APP_EVENTS_MAX, -1);
  if (n < 1) {
    return -1;
  }
  cfd = krad_controller_get_client_fd(&server->control);
  for (i = 0; i < n; i++) {
    fd = events[i].data.fd;
    if (fd == server->sd) {
      accept_client(server);
      continue;
    }
    if (fd == server->sfd) {
      printk("App Server: Signal");
      server->state = KR_APP_DO_SHUTDOWN;
    }
    if (fd == cfd) {
      printk("App Server: Shutdown from controller");
      server->state = KR_APP_DO_SHUTDOWN;
    }
  }
  return 0;
}

static void *server_loop(void *arg) {
  kr_app_server *server;
  kr_app_server_client *client;
  struct epoll_event events[KR_APP_EVENTS_MAX];
  struct epoll_event ev;
  int n;
  int i;
  int ret;
  server = (kr_app_server *)arg;
  server->start_time = time(NULL);
  krad_system_set_thread_name("kr_app_server");
  if (server->pid != server->tid) {
    ret = setup_signals(server);
    if (ret != 0) {
      printke("App Server: Failed to setup signal handler");
    }
  }
  server->state = KR_APP_RUNNING;
  while (server->state == KR_APP_RUNNING) {
    n = epoll_wait(server->pd, events, KR_APP_EVENTS_MAX, -1);
    if ((n < 1) || (server->state)) {
      break;
    }
    for (i = 0; i < n; i++) {
      if (events[i].data.ptr == server) {
        handle_app_events(server);
        continue;
      }
      printk("App Server: Got client event");
      client = events[i].data.ptr;
      server->current_client = client;
      //switch (events[i].events) {
      //  case EPOLLIN:
      if (events[i].events & EPOLLIN) {
          ret = kr_io2_read(client->in);
          if (ret > 0) {
            printk("App Server: Got %d bytes from client", ret);
            if (handle_client(server, client)) {
              printk("App Server: Handle client ret %d", ret);
              disconnect_client(server, client);
              continue;
            } else {
              printk("client check");
              if (kr_io2_want_out(client->out)) {
                printk("client did want out");
                memset(&ev, 0, sizeof(struct epoll_event));
                ev.events = EPOLLIN | EPOLLOUT;
                ev.data.ptr = client;
                if (epoll_ctl(server->pd, EPOLL_CTL_MOD, client->sd, &ev)) {
                  printke("Error modding client epoll fd to also look for out");
                }
              }
            }
          } else {
            if (ret == 0) {
              printk("App Server: Client EOF");
              disconnect_client(server, client);
              continue;
            }
            if (ret == -1) {
              printke("App Server: Client Socket Error");
              disconnect_client(server, client);
              continue;
            }
          }
      }
      if (events[i].events & EPOLLOUT) {
//        case EPOLLOUT:
          printk("App Server: EPOLLOUT");
          ret = kr_io2_output(client->out);
          if (ret != 0) {
            printke("App Server: panic drop");
            disconnect_client(server, client);
            continue;
          }
          if (!(kr_io2_want_out(client->out))) {
            memset(&ev, 0, sizeof(struct epoll_event));
            ev.events = EPOLLIN;
            ev.data.ptr = client;
            if (epoll_ctl(server->pd, EPOLL_CTL_MOD, client->sd, &ev)) {
              printke("Error modding client epoll fd to also look for out");
            }
          }
      }
      if (events[i].events & EPOLLHUP) {
      //  case EPOLLHUP:
          printk("App Server: EPOLLHUP");
          disconnect_client(server, client);
          continue;
      }
      if (events[i].events & EPOLLERR) {
//        case EPOLLERR:
          printke("App Server: EPOLLERR");
          disconnect_client(server, client);
          continue;
      }
    }
  }
  server->start_time = 0;
  krad_controller_client_close(&server->control);
  printk("App Server: Mainloop exits");
  return NULL;
}

static int setup_socket(char *appname, char *sysname) {
  int sd;
  struct sockaddr_un saddr;
  socklen_t socket_sz;
  sd = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (sd == -1) {
    printke("App Server: Socket failed.");
    return -1;
  }
  saddr.sun_family = AF_UNIX;
  snprintf(saddr.sun_path, sizeof(saddr.sun_path),
   "@%s_%s_api", appname, sysname);
  socket_sz = sizeof(saddr) - sizeof(saddr.sun_path);
  socket_sz += strlen(saddr.sun_path);
  saddr.sun_path[0] = '\0';
  if (connect(sd, (struct sockaddr *)&saddr, socket_sz) != -1) {
    printke("App Server: Socket name in use.");
    close(sd);
    return -1;
  }
  if (bind(sd, (struct sockaddr *)&saddr, socket_sz) == -1) {
    printke("App Server: Can't bind to socket.");
    close(sd);
    return -1;
  }
  listen(sd, SOMAXCONN);
  return sd;
}

/*
kr_app_server_client *kr_app_server_client_create(kr_app_server_client_setup *s) {
  if (s == NULL) return NULL;
  client = kr_allocz(1, sizeof(kr_app_server_client));
  //callbacks for broadcasting/serial?
  return client;
}

int kr_app_server_client_destroy(kr_app_server_client *client) {
  if (client == NULL) return -1;
  //disconnect them
  free(client);
  return 0;
}
*/

int kr_app_server_crate_reply(kr_app_server *server, kr_crate2 *crate) {
  int ret;
  printk("crate response");
  kr_app_server_client *client;
  if (crate == NULL) return -2;
  client = server->current_client;
  if (client == NULL) return -1;
  ret = local_client_pack_crate(client->out->buf, crate, client->out->space);
  if (ret > 0) {
    kr_io2_advance(client->out, ret);
    return 0;
  }
  return -1;
}

int kr_app_server_map_destroy(kr_app_server *server, kr_router_map *map) {
  if (server == NULL) return -1;
  return kr_router_map_destroy(server->router, map);
}

kr_router_map *kr_app_server_map_create(kr_app_server *server, kr_router_map_setup *setup) {
  if (server == NULL) return NULL;
  return kr_router_map_create(server->router, setup);
}

kr_route *kr_app_server_route_create(kr_app_server *server, kr_route_setup *setup) {
  if (server == NULL) return NULL;
  return kr_route_create(server->router, setup);
}

int kr_app_server_info_get(kr_app_server *server, kr_app_server_info *info) {
  uint64_t now;
  if ((server == NULL) || (info == NULL)) return -1;
  if (server->state != KR_APP_RUNNING) {
   info->clients = 0;
   info->uptime = 0;
  } else {
    now = time(NULL);
    info->uptime = now - server->start_time;
    info->clients = server->num_clients;
  }
  return 0;
}

int kr_app_server_disable(kr_app_server *server) {
  if (server == NULL) return -1;
  printk("App Server: Disabling");
  if (!krad_controller_shutdown(&server->control, &server->thread, 30)) {
    krad_controller_destroy(&server->control, &server->thread);
  }
  printk("App Server: Disabled");
  return 0;
}

int kr_app_server_enable(kr_app_server *server) {
  if (server == NULL) return -1;
  if (server->state != KR_APP_STARTING) return -2;
  printk("App Server: Enabling");
  server->pid = getpid();
  server->tid = syscall(SYS_gettid);
  pthread_create(&server->thread, NULL, server_loop, (void *)server);
  return 0;
}

int kr_app_server_destroy(kr_app_server *server) {
  int i;
  kr_app_server_client *client;
  if (server == NULL) return -1;
  printk("App Server: Destroying");
  if (server->state != KR_APP_SHUTINGDOWN) {
    kr_app_server_disable(server);
  }
  if (server->sd != 0) {
    close(server->sd);
  }
  i = 0;
  while ((client = kr_pool_iterate_active(server->client_pool, &i))) {
    disconnect_client(server, client);
  }
  if (server->app_pd > -1) {
    close(server->app_pd);
  }
  if (server->sfd > -1) {
    close(server->sfd);
  }
  if (server->pd > -1) {
    close(server->pd);
  }
  kr_router_destroy(server->router);
  kr_pool_destroy(server->client_pool);
  printk("App Server: Destroyed");
  return 0;
}

kr_app_server *kr_app_server_create(kr_app_server_setup *setup) {
  int sd;
  int ret;
  struct epoll_event ev;
  kr_app_server *server;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  kr_router_setup router_setup;
  if (setup == NULL) return NULL;
  printk("App Server: Creating");
  sd = setup_socket(setup->appname, setup->sysname);
  if (sd < 0) {
    return NULL;
  }
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(kr_app_server);
  pool_setup.size = sizeof(kr_app_server_client);
  pool_setup.slices = KR_APP_SERVER_CLIENTS_MAX;
  pool = kr_pool_create(&pool_setup);
  server = kr_pool_overlay_get(pool);
  memset(server, 0, sizeof(kr_app_server));
  server->client_pool = pool;
  server->sd = sd;
  server->sfd = -1;
  server->app_pd = -1;
  server->pd = -1;
  server->state = KR_APP_STARTING;
  /*
  server->user = setup->user;
  server->event_cb = setup->event_cb;
  */
  if (krad_control_init(&server->control)) {
    printke("App Server: Control init failed");
    kr_app_server_destroy(server);
    return NULL;
  }
  server->app_pd = epoll_create1(EPOLL_CLOEXEC);
  if (server->app_pd == -1) {
    printke("App Server: app_pd epoll created failed");
    kr_app_server_destroy(server);
    return NULL;
  }
  server->pd = epoll_create1(EPOLL_CLOEXEC);
  if (server->pd == -1) {
    printke("App Server: pd epoll create failed");
    kr_app_server_destroy(server);
    return NULL;
  }
  memset(&ev, 0, sizeof(struct epoll_event));
  ev.events = EPOLLIN;
  ev.data.fd = krad_controller_get_client_fd(&server->control);
  ret = epoll_ctl(server->app_pd, EPOLL_CTL_ADD, ev.data.fd, &ev);
  if (ret != 0) {
    printke("App Server: epoll ctl app pd fail");
    kr_app_server_destroy(server);
    return NULL;
  }
  ev.data.fd = server->sd;
  ret = epoll_ctl(server->app_pd, EPOLL_CTL_ADD, ev.data.fd, &ev);
  if (ret != 0) {
    printke("App Server: epoll ctl app pd 2 fail");
    kr_app_server_destroy(server);
    return NULL;
  }
  ev.data.ptr = server;
  ret = epoll_ctl(server->pd, EPOLL_CTL_ADD, server->app_pd, &ev);
  if (ret != 0) {
    printke("App Server: epoll ctl pd failed");
    kr_app_server_destroy(server);
    return NULL;
  }
  router_setup.routes_max = 64;
  router_setup.maps_max = 64;
  router_setup.user = server;
  router_setup.response = (kr_router_response_handler *)kr_app_server_crate_reply;
  server->router = kr_router_create(&router_setup);
  printk("App Server: Created");
  return server;
}
