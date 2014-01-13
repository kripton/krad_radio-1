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

#include "krad_app_server.h"
#include "krad_pool.h"

static int setup_socket(char *appname, char *sysname);
static void update_pollfds(kr_app_server *server);
static void *server_loop(void *arg);
static int  handle_client(kr_app_server *server, kr_app_server_client *client);
static void disconnect_client(kr_app_server *server, kr_app_server_client *client);
static kr_app_server_client *accept_client(kr_app_server *server, int sd);

typedef enum {
  KR_APP_STARTING = -1,
  KR_APP_RUNNING,
  KR_APP_DO_SHUTDOWN,
  KR_APP_SHUTINGDOWN,
} app_state;

struct kr_app_server {
  int sd;
  uint64_t start_time;
  app_state state;
  int socket_count;
  krad_control_t krad_control;
  uint64_t num_clients;
  kr_app_server_client *clients;
  kr_app_server_client *current_client;
  pthread_t thread;
  struct pollfd sockets[KR_APP_SERVER_CLIENTS_MAX + 2];
  kr_app_server_client *sockets_clients[KR_APP_SERVER_CLIENTS_MAX + 2];
  kr_router *router;
  kr_pool *client_pool;
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

static int setup_socket(char *appname, char *sysname) {
  int sd;
  struct sockaddr_un saddr;
  socklen_t socket_sz;
  sd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
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

int validate_header(kr_io2_t *in) {
  kr_ebml2_t ebml;
  int ret;
  char doctype[32];
  uint32_t version;
  uint32_t read_version;
  kr_ebml2_set_buffer(&ebml, in->rd_buf, in->len);
  ret = kr_ebml2_unpack_header(&ebml, doctype, 32, &version, &read_version);
  if (ret > 0) {
    if ((version == KRAD_APP_DOCTYPE_VERSION)
     && (read_version == KRAD_APP_DOCTYPE_READ_VERSION)
     && (strlen(KRAD_APP_CLIENT_DOCTYPE) == strlen(doctype))
     && (strncmp(doctype, KRAD_APP_CLIENT_DOCTYPE, strlen(KRAD_APP_CLIENT_DOCTYPE)) == 0)) {
        kr_io2_pulled(in, ret);
        return ret;
    } else {
      printke("validate header mismatch %u %u %s \n", version, read_version, doctype);
    }
  } else {
    printke("validate header err %d\n", ret);
  }
  return -1;
}

void pack_client_header(kr_io2_t *out) {
  kr_ebml2_t ebml;
  kr_ebml2_set_buffer(&ebml, out->buf, out->space);
  kr_ebml_pack_header(&ebml, KRAD_APP_SERVER_DOCTYPE, KRAD_APP_DOCTYPE_VERSION,
   KRAD_APP_DOCTYPE_READ_VERSION);
  kr_io2_advance(out, ebml.pos);
}

int validate_client(kr_app_server_client *client) {
  int ret;
  ret = validate_header(client->in);
  if (ret > 0) {
    client->valid = 1;
    pack_client_header(client->out);
    printk("Valid client!");
    return 1;
  } else {
    printke("Invalid client!");
    return -1;
  }
}

int local_client_get_crate(kr_crate2 *crate, kr_io2_t *in) {
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
    printke("full crate EBML ID Not found");
    return 0;
  }
  size += ebml.pos;
  if (in->len < size) {
    printke("Crate not full...");
    return 0;
  }
  if (element == KR_EID_CRATE) {
    ret = kr_crate2_fr_ebml(&ebml, crate);
    if (ret == 0) {
      char string[8192];
      ret = kr_crate2_to_text(string, crate, sizeof(string));
      if (ret > 0) {
        printk("%"PRIu64" Byte Crate: \n%s\n", size, string);
      }
      kr_io2_pulled(in, ebml.pos);
      return 1;
    }
  }
  return 0;
}

static kr_app_server_client *accept_client(kr_app_server *server, int sd) {
  kr_app_server_client *client;
  struct sockaddr_un sin;
  socklen_t slen;
  slen = sizeof(sin);
  client = kr_pool_slice(server->client_pool);
  if (client == NULL) {
    printke("App Server: Overloaded with clients!");
    return NULL;
  }
  client->sd = accept4(sd, (struct sockaddr *)&sin, &slen, SOCK_NONBLOCK);
  if (client->sd < 0) {
    kr_pool_recycle(server->client_pool, client);
    printke("App Server: accept() failed!");
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

static void disconnect_client(kr_app_server *server, kr_app_server_client *client) {
  close(client->sd);
  kr_io2_destroy(&client->in);
  kr_io2_destroy(&client->out);
  kr_pool_recycle(server->client_pool, client);
  server->num_clients--;
  printk("App Server: Client disconnected");
}

static void update_pollfds(kr_app_server *server) {
  kr_app_server_client *client;
  int s;
  int i;
  s = 0;
  server->sockets[s].fd = krad_controller_get_client_fd(&server->krad_control);
  server->sockets[s].events = POLLIN;
  s++;
  server->sockets[s].fd = server->sd;
  server->sockets[s].events = POLLIN;
  s++;
  i = 0;
  while ((client = kr_pool_iterate_active(server->client_pool, &i))) {
    server->sockets[s].fd = client->sd;
    server->sockets[s].events |= POLLIN;
    if (kr_io2_want_out(client->out)) {
      server->sockets[s].events |= POLLOUT;
    }
    server->sockets_clients[s] = client;
    s++;
  }
  server->socket_count = s;
  //printk("App Server: polled fds updated");
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
  kr_app_server_client *client;
  kr_ebml2_t ebml;
  if (crate == NULL) return -2;
  client = server->current_client;
  if (client == NULL) return -1;
  kr_ebml2_set_buffer(&ebml, client->out->buf, client->out->space);
  //if (kr_crate2_valid(info) < 0) {
  // return -1;
  //}
  uint8_t *ebml_crate;
  kr_ebml2_start_element(&ebml, KR_EID_CRATE, &ebml_crate);
  kr_crate2_to_ebml(&ebml, crate);
  kr_ebml2_finish_element(&ebml, ebml_crate);
  kr_io2_advance(client->out, ebml.pos);
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

static void *server_loop(void *arg) {
  kr_app_server *server;
  kr_app_server_client *client;
  int ret;
  int s;
  int read_ret;
  int hret;
  int32_t oret;
  server = (kr_app_server *)arg;
  krad_system_set_thread_name("kr_app_server");
  server->state = KR_APP_RUNNING;
  server->start_time = time(NULL);
  while (server->state == KR_APP_RUNNING) {
    s = 0;
    update_pollfds(server);
    ret = poll (server->sockets, server->socket_count, -1);
    if ((ret < 1) || (server->state) || (server->sockets[s].revents)) {
      break;
    }
    s++;
    // Linux abstract socket for local connections
    if (server->sockets[s].revents & POLLIN) {
      accept_client(server, server->sd);
      ret--;
    }
    s++;
    for (; ret > 0; s++) {
      if (server->sockets[s].revents) {
        ret--;
          client = server->sockets_clients[s];
          if (server->sockets[s].revents & POLLIN) {
            read_ret = kr_io2_read(client->in);
            if (read_ret > 0) {
              printk("App Server fd %d: Got %d bytes", s, read_ret);
              server->current_client = client;
              hret = handle_client(server, client);
              if (hret != 0) {
                disconnect_client(server, client);
                continue;
              } else {
                if (kr_io2_want_out (client->out)) {
                  server->sockets[s].events |= POLLOUT;
                }
              }
            } else {
              if (read_ret == 0) {
                //printk("App Server: Client EOF\n");
                disconnect_client(server, client);
                continue;
              }
              if (read_ret == -1) {
                printke ("Krad App Server: Client Socket Error");
                disconnect_client(server, client);
                continue;
              }
            }
          }
          if (server->sockets[s].revents & POLLOUT) {
            oret = kr_io2_output(client->out);
            if (oret != 0) {
              printke("App Server: panic drop");
              disconnect_client(server, client);
              continue;
            }
            if (!(kr_io2_want_out(client->out))) {
              server->sockets[s].events = POLLIN;
            }
          } else {
            if (server->sockets[s].revents & POLLHUP) {
              //printk("App Server %d: POLLHUP", s);
              disconnect_client(server, client);
              continue;
            }
          }
          if (server->sockets[s].revents & POLLERR) {
            printke("App Server: POLLERR");
            disconnect_client(server, client);
            continue;
          }
        }
    }
  }
  server->state = KR_APP_SHUTINGDOWN;
  server->start_time = 0;
  krad_controller_client_close(&server->krad_control);
  return NULL;
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
  if (!krad_controller_shutdown(&server->krad_control, &server->thread, 30)) {
    krad_controller_destroy(&server->krad_control, &server->thread);
  }
  if (server->sd != 0) {
    close(server->sd);
  }
  printk("App Server: Disabled");
  return 0;
}

int kr_app_server_enable(kr_app_server *server) {
  if (server == NULL) return -1;
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
  i = 0;
  while ((client = kr_pool_iterate_active(server->client_pool, &i))) {
    disconnect_client(server, client);
  }
  kr_router_destroy(server->router);
  kr_pool_destroy(server->client_pool);
  printk("App Server: Destroyed");
  return 0;
}

kr_app_server *kr_app_server_create(kr_app_server_setup *setup) {
  int sd;
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
  /*
  server->user = setup->user;
  server->event_cb = setup->event_cb;
  */
  if (krad_control_init(&server->krad_control)) {
    kr_app_server_destroy(server);
    return NULL;
  }
  server->state = KR_APP_STARTING;
  server->sd = sd;
  router_setup.routes_max = 64;
  router_setup.maps_max = 64;
  router_setup.user = server;
  router_setup.response = (kr_router_response_handler *)kr_app_server_crate_reply;
  server->router = kr_router_create(&router_setup);
  printk("App Server: Created");
  return server;
}
