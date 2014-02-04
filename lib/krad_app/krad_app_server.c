#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <time.h>
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
#include <pthread.h>
#include <ifaddrs.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
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
  uint64_t max_clients;
  int socket_count;
  kr_app_server_client *current_client;
  kr_app_server_client *new_clients[KR_APP_SERVER_CLIENTS_MAX];
  int sfd;
  int efd;
  uint64_t start_time;
  app_state state;
  krad_control_t control;
  pid_t pid;
  pid_t tid;
  pthread_t thread;
};

typedef enum {
  KR_APP_CLIENT_LOCAL_NEW = 1,
  KR_APP_CLIENT_LOCAL_VALID,
  KR_APP_CLIENT_REMOTE,
} kr_app_client_type;

struct kr_app_server_client {
  int sd;
  kr_app_client_type type;
  kr_io2_t *in;
  kr_io2_t *out;
  uint8_t state_tracker[256];
  size_t state_tracker_sz;
  kr_app_server_io_cb *input_cb;
  kr_app_server_io_cb *output_cb;
};

static int validate_local_client_header(uint8_t *header, size_t sz);
static int validate_local_client(kr_app_server_client *client);
static int pack_crate_remote(uint8_t *buffer, kr_crate2 *crate, size_t max);
static int pack_crate_local(uint8_t *buffer, kr_crate2 *crate, size_t max);
static int unpack_crate_remote(kr_crate2 *crate, kr_app_server_client *client);
static int unpack_crate_local(kr_crate2 *crate, kr_io2_t *in);
static int handle_client(kr_app_server *server, kr_app_server_client *client);
static void disconnect_client(kr_app_server *server, kr_app_server_client *client);
static kr_app_server_client *accept_client(kr_app_server *server);
static void *server_loop(void *arg);
static int setup_signals(kr_app_server *server);
static int setup_socket(char *appname, char *sysname);

static int validate_local_client_header(uint8_t *header, size_t sz) {
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

static int validate_local_client(kr_app_server_client *client) {
  kr_ebml2_t ebml;
  int ret;
  ret = validate_local_client_header(client->in->rd_buf, client->in->len);
  if (ret > 0) {
    kr_io2_pulled(client->in, ret);
    client->type = KR_APP_CLIENT_LOCAL_VALID;
    kr_ebml2_set_buffer(&ebml, client->out->buf, client->out->space);
    kr_ebml_pack_header(&ebml, KR_APP_SERVER_DOCTYPE, KR_APP_DOCTYPE_VERSION,
     KR_APP_DOCTYPE_READ_VERSION);
    if (ebml.pos < 1) {
      printke("App Server: Error packing client header");
    } else {
      printk("ret is %d ebmlpos is %d", ret, ebml.pos);
      kr_io2_advance(client->out, ebml.pos);
      return 0;
    }
  }
  printke("App Server: Invalid client!");
  return -1;
}

static int pack_crate_remote(uint8_t *buffer, kr_crate2 *crate, size_t max) {
  int res;
  res = kr_crate2_to_json((char *)buffer, crate, max);
  return res;
}

static int pack_crate_local(uint8_t *buffer, kr_crate2 *crate, size_t max) {
  kr_ebml2_t ebml;
  uint8_t *ebml_crate;
  kr_ebml2_set_buffer(&ebml, buffer, max);
  kr_ebml2_start_element(&ebml, KR_EID_CRATE, &ebml_crate);
  kr_crate2_to_ebml(&ebml, crate);
  kr_ebml2_finish_element(&ebml, ebml_crate);
  return ebml.pos;
}

static int unpack_crate_remote(kr_crate2 *crate, kr_app_server_client *client) {
  int ret;
  char json[8192];
  if (!(kr_io2_has_in(client->in))) {
    return 0;
  }
  ret = client->input_cb(&client->state_tracker, json, sizeof(json), client->in->rd_buf, client->in->len);
  if (ret < 1) return 0;
  kr_io2_pulled(client->in, ret);
  ret = kr_crate2_fr_json(json, crate);
  if (ret > 0) {
    return 1;
  } else {
    printk("Misunderstood: \n%s\n", json);
  }
  return 0;
}

static int unpack_crate_local(kr_crate2 *crate, kr_io2_t *in) {
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
      kr_io2_pulled(in, ebml.pos);
      return 1;
    }
  }
  return 0;
}

static int handle_client(kr_app_server *server, kr_app_server_client *client) {
  kr_crate2 crate;
  switch (client->type) {
    case KR_APP_CLIENT_REMOTE:
      while (unpack_crate_remote(&crate, client)) {
        kr_router_handle(server->router, &crate);
      }
      break;
    case KR_APP_CLIENT_LOCAL_VALID:
      while (unpack_crate_local(&crate, client->in)) {
        kr_router_handle(server->router, &crate);
      }
      break;
    case KR_APP_CLIENT_LOCAL_NEW:
      if (validate_local_client(client)) {
        return 1;
      }
      break;
    default:
      printke("App Server: Handle client fail");
      return -1;
  }
  return 0;
}

static void disconnect_client(kr_app_server *server, kr_app_server_client *client) {
  int ret;
  ret = epoll_ctl(server->pd, EPOLL_CTL_DEL, client->sd, NULL);
  if (ret != 0) {
    printke("App Server: remove client from epoll on disconnect failed!");
  }
  if (client->sd != -1) {
    close(client->sd);
    client->sd = -1;
  }
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
  client->type = KR_APP_CLIENT_LOCAL_NEW;
  client->in = kr_io2_create();
  client->out = kr_io2_create();
  kr_io2_set_fd(client->in, client->sd);
  kr_io2_set_fd(client->out, client->sd);
  server->num_clients++;
  printk("App Server: Local client accepted");
  return client;
}

int32_t json_hello(kr_app_server_client *client) {
  /* current js expects this and it makes the rack load */
  ssize_t ret;
  char json[128];
  snprintf(json, sizeof(json), "[{\"com\":\"kradradio\","
   "\"info\":\"sysname\",\"infoval\":\"%s\"}]", "bongohat");
  ret = client->output_cb(client->state_tracker, client->out->buf, client->out->space, (uint8_t *)json, strlen(json));
  if (ret > 0) {
    kr_io2_advance(client->out, ret);
    return 0;
  } else {
    return -1;
  }
}

static void accept_remote_client(kr_app_server *server) {
  struct epoll_event ev;
  int i;
  uint64_t u;
  ssize_t s;
  int ret;
  kr_app_server_client *new_client;
  kr_app_server_client *client;
  u = 0;
  s = read(server->efd, &u, sizeof(uint64_t));
  if (s != sizeof(uint64_t)) {
    printke("App Server: Error reading from eventfd");
    return;
  }
  /*printk("App Server: %"PRIu64" new client from ws", u);*/
  if (u == 0) return;
  client = NULL;
  new_client = NULL;
  for (i = 0; i < KR_APP_SERVER_CLIENTS_MAX; i++) {
    if (server->new_clients[i] != NULL) {
      new_client = server->new_clients[i];
      break;
    }
  }
  if (new_client == NULL) {
    printke("App Server: No new client really :/");
    return;
  }
  client = kr_pool_slice(server->client_pool);
  if (client == NULL) {
    printke("App Server: Overloaded2 can't accept client");
    return;
  }
  memcpy(client, new_client, sizeof(kr_app_server_client));
  memset(&ev, 0, sizeof(struct epoll_event));
  ev.events = EPOLLIN;
  if (client->state_tracker_sz > 0) {
    json_hello(client);
  }
  handle_client(server, client);
  if (kr_io2_want_out(client->out)) {
    ev.events = EPOLLIN | EPOLLOUT;
    /*printk("App Server: Yes we want out");*/
  }
  ev.data.ptr = client;
  ret = epoll_ctl(server->pd, EPOLL_CTL_ADD, client->sd, &ev);
  if (ret != 0) {
    kr_pool_recycle(server->client_pool, client);
    printke("App Server: Adding client to epoll after accept4 failed");
    close(client->sd);
    return;
  }
  server->num_clients++;
  printk("App Server: Remote client accepted");
  free(new_client);
  server->new_clients[i] = NULL;
}

static int handle_app_events(kr_app_server *server) {
  int cfd;
  int fd;
  int n;
  int i;
  /*printk("App Server: Got app event");*/
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
    if (fd == server->efd) {
      /*printk("App Server: Must be a new client from remote!");*/
      accept_remote_client(server);
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
      /*printk("App Server: Got client event");*/
      client = events[i].data.ptr;
      server->current_client = client;
      if (events[i].events & EPOLLIN) {
          ret = kr_io2_read(client->in);
          if (ret > 0) {
            printk("App Server: Got %d bytes from client", ret);
            if (handle_client(server, client)) {
              printk("App Server: Handle client ret %d", ret);
              disconnect_client(server, client);
              continue;
            } else {
              if (kr_io2_want_out(client->out)) {
                /*printk("client did want out");*/
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
          printk("App Server: EPOLLOUT");
          ret = kr_io2_output(client->out);
          if (ret != 0) {
            printke("App Server: panic drop");
            disconnect_client(server, client);
            continue;
          }
          if ((client->type == KR_APP_CLIENT_REMOTE)
           && (client->state_tracker_sz == 0)) {
            printk("App Server: Dropping REST client after io drain");
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
          printk("App Server: EPOLLHUP");
          disconnect_client(server, client);
          continue;
      }
      if (events[i].events & EPOLLERR) {
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

int kr_app_server_client_create(kr_app_server *server,
 kr_app_server_client_setup *setup) {
  int i;
  uint64_t u;
  ssize_t s;
  kr_app_server_client *client;
  if (server == NULL) return -1;
  if (setup == NULL) return -2;
  if (server->num_clients == server->max_clients) {
    printke("App Server: To many clients to add ws client %d", setup->fd);
    return -3;
  }
  if (setup->state_tracker_sz > sizeof(client->state_tracker)) {
    printke("App Server: Client state tracker size to large!");
    return -4;
  }
  client = NULL;
  for (i = 0; i < KR_APP_SERVER_CLIENTS_MAX; i++) {
    if (server->new_clients[i] == NULL) {
      client = kr_allocz(1, sizeof(kr_app_server_client));
      break;
    }
  }
  if (client == NULL) {
    printke("App Server: too many new clients");
    return -5;
  }
  client->type = KR_APP_CLIENT_REMOTE;
  client->sd = setup->fd;
  client->input_cb = setup->input_cb;
  client->output_cb = setup->output_cb;
  client->state_tracker_sz = setup->state_tracker_sz;
  memcpy(client->state_tracker, setup->state_tracker, client->state_tracker_sz);
  client->in = kr_io2_create();
  client->out = kr_io2_create();
  kr_io2_pack(client->in, setup->in->buffer, setup->in->len);
  kr_io2_pack(client->out, setup->out->buffer, setup->out->len);
  kr_io2_set_fd(client->in, client->sd);
  kr_io2_set_fd(client->out, client->sd);
  server->new_clients[i] = client;
  printk("client add happen! fd is %d", setup->fd);
  u = 1;
  s = write(server->efd, &u, sizeof(uint64_t));
  if (s != sizeof(uint64_t)) {
    printke("App Server: Error writing to eventfd");
    return -2;
  }
  return 0;
}

int kr_app_server_crate_reply(kr_app_server *server, kr_crate2 *crate) {
  int ret;
  printk("crate response");
  kr_app_server_client *client;
  if (crate == NULL) return -2;
  client = server->current_client;
  if (client == NULL) return -1;
  ret = 0;
  switch (client->type) {
    case KR_APP_CLIENT_REMOTE:
      ret = pack_crate_remote(client->out->buf, crate, client->out->space);
      /* Kludge don't send null for rest */
      if (ret > 1) client->out->buf[ret - 1] = '\n';
      break;
    case KR_APP_CLIENT_LOCAL_VALID:
      ret = pack_crate_local(client->out->buf, crate, client->out->space);
      break;
    default:
      printke("App Server: Unimplmented protocal type");
      break;
  }
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
  server->state = KR_APP_RUNNING;
  pthread_create(&server->thread, NULL, server_loop, (void *)server);
  return 0;
}

int kr_app_server_destroy(kr_app_server *server) {
  int i;
  kr_app_server_client *client;
  if (server == NULL) return -1;
  printk("App Server: Destroying");
  if (server->state == KR_APP_RUNNING) {
    kr_app_server_disable(server);
  }
  if (server->sd != 0) {
    close(server->sd);
  }
  i = 0;
  while ((client = kr_pool_iterate_active(server->client_pool, &i))) {
    disconnect_client(server, client);
  }
  if (server->efd > -1) {
    close(server->efd);
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
  server->max_clients = KR_APP_SERVER_CLIENTS_MAX;
  /*
  server->user = setup->user;
  server->event_cb = setup->event_cb;
  */
  if (krad_control_init(&server->control)) {
    printke("App Server: Control init failed");
    kr_app_server_destroy(server);
    return NULL;
  }
  server->efd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (server->efd == -1) {
    printke("App Server: efd evenfd created failed");
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
    printke("App Server: epoll ctl add controller to app pd fail");
    kr_app_server_destroy(server);
    return NULL;
  }
  ev.data.fd = server->sd;
  ret = epoll_ctl(server->app_pd, EPOLL_CTL_ADD, ev.data.fd, &ev);
  if (ret != 0) {
    printke("App Server: epoll ctl add abstractsocket to app pd fail");
    kr_app_server_destroy(server);
    return NULL;
  }
  ev.data.fd = server->efd;
  ret = epoll_ctl(server->app_pd, EPOLL_CTL_ADD, ev.data.fd, &ev);
  if (ret != 0) {
    printke("App Server: epoll ctl add efd to app_pd fail");
    kr_app_server_destroy(server);
    return NULL;
  }
  ev.data.ptr = server;
  ret = epoll_ctl(server->pd, EPOLL_CTL_ADD, server->app_pd, &ev);
  if (ret != 0) {
    printke("App Server: epoll ctl add apppd to pd failed");
    kr_app_server_destroy(server);
    return NULL;
  }
  snprintf(router_setup.name, sizeof(router_setup.name), "%s", setup->appname);
  router_setup.routes_max = 64;
  router_setup.maps_max = 64;
  router_setup.user = server;
  router_setup.response = (kr_router_response_handler *)kr_app_server_crate_reply;
  server->router = kr_router_create(&router_setup);
  printk("App Server: Created");
  return server;
}
