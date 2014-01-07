#include "krad_app_server.h"

static kr_app_server *server_init(char *appname, char *sysname);
static void *server_loop(void *arg);
static void disconnect_client(kr_app_server *app, kr_app_server_client *client);
static void update_pollfds(kr_app_server *app_server);
static kr_app_server_client *accept_client(kr_app_server *app, int sd);
int current_client_is_subscriber(kr_app_server *server);

typedef enum {
  KR_APP_STARTING = -1,
  KR_APP_RUNNING,
  KR_APP_DO_SHUTDOWN,
  KR_APP_SHUTINGDOWN,
} app_state;

//typedef struct kr_app_address_route kr_app_address_route;
typedef struct kr_app_address_sliced kr_app_address_sliced;

struct kr_app_server {
  struct sockaddr_un saddr;
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
  kr_pool *maps;
};

struct kr_app_server_client {
  int sd;
  void *ptr;
  int valid;
  int broadcasts;
  kr_io2_t *in;
  kr_io2_t *out;
};

/*
  struct kr_app_server_client_setup {
  //callback for broadcast/serialize?
  int coconut;
};
*/

struct kr_app_address_sliced {
  char *slice[4];
  size_t slices;
};

/*
  struct kr_app_address_route {
  char name[48];
  void *ptr;
  kr_app_address_mapper *mapper;
};
*/

struct kr_app_server_map {
  char prefix[32];
  void *ptr; /* for create */
  kr_app_server_map_create_handler *create;
  kr_app_server_map_patch_handler *patch;
  kr_app_server_map_destroy_handler *destroy;
};

static kr_app_server *server_init(char *appname, char *sysname) {
  kr_app_server *server;
  socklen_t socket_sz;
  server = calloc(1, sizeof(kr_app_server));
  if (krad_control_init(&server->krad_control)) {
    free(server);
    return NULL;
  }
  server->state = KR_APP_STARTING;
  server->clients = calloc(KR_APP_SERVER_CLIENTS_MAX, sizeof(kr_app_server_client));
  server->sd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
  if (server->sd == -1) {
    printke ("Krad APP Server: Socket failed.\n");
    kr_app_server_destroy(server);
    return NULL;
  }
  server->saddr.sun_family = AF_UNIX;
  snprintf(server->saddr.sun_path, sizeof(server->saddr.sun_path),
   "@%s_%s_api", appname, sysname);
  socket_sz = sizeof(server->saddr) - sizeof(server->saddr.sun_path);
  socket_sz += strlen(server->saddr.sun_path);
  server->saddr.sun_path[0] = '\0';
  if (connect(server->sd, (struct sockaddr *)&server->saddr, socket_sz) != -1) {
    printke("Krad App Server: Socket name in use.");
    kr_app_server_destroy(server);
    return NULL;
  }
  if (bind(server->sd, (struct sockaddr *)&server->saddr, socket_sz) == -1) {
    printke("Krad App Server: Can't bind to socket.\n");
    kr_app_server_destroy(server);
    return NULL;
  }
  listen(server->sd, SOMAXCONN);
  return server;
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

int validate_client(kr_io2_t *in, kr_io2_t *out, kr_app_server_client *client) {
  int ret;
  ret = validate_header(in);
  if (ret > 0) {
    client->valid = 1;
    pack_client_header(out);
    printk("Valid client!");
    return 1;
  } else {
    printke("Invalid client!");
    return -1;
  }
}

uint32_t full_crate(kr_io2_t *in) {
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
    kr_crate2 crate;
    ret = kr_crate2_fr_ebml(&ebml, &crate);
    if (ret == 0) {
      char string[8192];
      ret = kr_crate2_to_text(string, &crate, sizeof(string));
      if (ret > 0) {
        printk("%"PRIu64" Byte Crate: \n%s\n", size, string);
      }
    }
  }
  return element;
}

static kr_app_server_client *accept_client(kr_app_server *server, int sd) {
  kr_app_server_client *client = NULL;
  int i;
  struct sockaddr_un sin;
  socklen_t sin_len;
  while (client == NULL) {
    for(i = 0; i < KR_APP_SERVER_CLIENTS_MAX; i++) {
      if (server->clients[i].sd == 0) {
        client = &server->clients[i];
        break;
      }
    }
    if (client == NULL) {
      printke("Krad App Server: Overloaded with clients!");
      return NULL;
    }
  }
  sin_len = sizeof (sin);
  client->sd = accept(sd, (struct sockaddr *)&sin, &sin_len);
  if (client->sd > 0) {
    krad_system_set_socket_nonblocking(client->sd);
    client->in = kr_io2_create();
    client->out = kr_io2_create();
    kr_io2_set_fd(client->in, client->sd);
    kr_io2_set_fd(client->out, client->sd);
    //client->ptr = server->client_create(server->pointer);
    server->num_clients++;
    //printk("Krad App Server: Client accepted!");
    return client;
  } else {
    printke("Krad App Server: accept() failed!");
  }
  return NULL;
}

static void disconnect_client(kr_app_server *server, kr_app_server_client *client) {
  close(client->sd);
  client->sd = 0;
  client->broadcasts = 0;
  //server->client_destroy(client->ptr);
  client->ptr = NULL;
  kr_io2_destroy(&client->in);
  kr_io2_destroy(&client->out);
  server->num_clients--;
  //printk("Krad App Server: Client disconnected");
}

static void update_pollfds(kr_app_server *server) {
  int c;
  int s;
  s = 0;
  server->sockets[s].fd = krad_controller_get_client_fd(&server->krad_control);
  server->sockets[s].events = POLLIN;
  s++;
  server->sockets[s].fd = server->sd;
  server->sockets[s].events = POLLIN;
  s++;
  for (c = 0; c < KR_APP_SERVER_CLIENTS_MAX; c++) {
    if (server->clients[c].sd > 0) {
      server->sockets[s].fd = server->clients[c].sd;
      server->sockets[s].events |= POLLIN;
      if (kr_io2_want_out(server->clients[c].out)) {
        server->sockets[s].events |= POLLOUT;
      }
      server->sockets_clients[s] = &server->clients[c];
      s++;
    }
  }
  server->socket_count = s;
  //printk("Krad App Server: polled fds updated");
}

int kr_app_server_current_client_is_subscriber (kr_app_server *app) {
  if (app->current_client->broadcasts > 0) {
    return 1;
  }
  return 0;
}

/*
kr_app_server_client *kr_app_server_client_create(kr_app_server_client_setup *s) {
  if (s == NULL) return NULL;
  client = calloc(1, sizeof(kr_app_server_client));
  //callbacks for broadcasting/serial?
  return client;
}

int kr_app_server_client_destroy(kr_app_server_client *client) {
  if (client == NULL) return -1;
  //disconnect them
  free(client);
  return 0;
}

int kr_radio_server_handle(kr_app_server_request *request) {
  kr_io2_t *in;
  kr_io2_t *out;
  void *ptr;
  ptr = request->ptr;
  in = request->in;
  out = request->out;
  kr_radio_client *client;
  int ret;
  uint32_t command;
  if (in == NULL) {
    printke("krad_radio_client handler called with null input buffer");
    return -1;
  }
  if (out == NULL) {
    printke("krad_radio_client handler called with null output buffer");
    return -1;
  }
  if (ptr == NULL) {
    printke("krad_radio_client handler called with null client pointerr");
    return -1;
  }
  client = (kr_radio_client *)ptr;
  if (!client->valid) {
    ret = validate_client(in, out, client);
    if (ret != 1) {
      return -1;
    }
  }
  for (;;) {
    command = full_crate(in);
    if (command == 0) {
      return 0;
    }
    switch (command) {
      case EBML_ID_KRAD_MIXER_CMD:
        //ret = kr_mixer_server_handle(request);
        break;
      case EBML_ID_KRAD_COMPOSITOR_CMD:
        //ret = kr_compositor_cmd(in, out, client);
        break;
      case EBML_ID_KRAD_TRANSPONDER_CMD:
        //ret = kr_transponder_cmd(in, out, client);
        break;
      case EBML_ID_KRAD_RADIO_CMD:
        //ret = kr_radio_cmd(request);
        break;
      default:
        ret = -1;
        break;
    }
    if (ret != 0) {
      return -1;
    }
  }
  return 0;
}
*/

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
    // Unix socket for local connections
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
            read_ret = kr_io2_read (client->in);
            if (read_ret > 0) {
              //printk("Krad APP Server %d: Got %d bytes\n", s, read_ret);
              /*
              server->current_client = client;
              kr_app_server_request request;
              request.ptr = client->ptr;
              request.in = client->in;
              request.out = client->out;
              request.app = server;
              hret = server->client_handler(&request);
              */
              hret = 0;
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
                //printk("Krad App Server: Client EOF\n");
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
              printke("Krad App Server: panic drop");
              disconnect_client(server, client);
              continue;
            }
            if (!(kr_io2_want_out (client->out))) {
              server->sockets[s].events = POLLIN;
            }
          } else {
            if (server->sockets[s].revents & POLLHUP) {
              //printk("Krad App Server %d: POLLHUP", s);
              disconnect_client(server, client);
              continue;
            }
          }
          if (server->sockets[s].revents & POLLERR) {
            printke("Krad App Server: POLLERR");
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

int kr_app_server_disable(kr_app_server *server) {
  if (server == NULL) return -1;
  printk("Krad APP Server: Disabling");
  if (!krad_controller_shutdown(&server->krad_control, &server->thread, 30)) {
    krad_controller_destroy(&server->krad_control, &server->thread);
  }
  if (server->sd != 0) {
    close(server->sd);
  }
  printk("Krad App Server: Disabled");
  return 0;
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

int kr_app_server_destroy(kr_app_server *server) {
  int i;
  if (server == NULL) return -1;
  printk("Krad App Server: Destroying");
  if (server->state != KR_APP_SHUTINGDOWN) {
    kr_app_server_disable(server);
  }
  for (i = 0; i < KR_APP_SERVER_CLIENTS_MAX; i++) {
    if (server->clients[i].sd > 0) {
      disconnect_client(server, &server->clients[i]);
    }
  }
  kr_pool_destroy(server->maps);
  free(server->clients);
  free(server);
  printk("Krad App Server: Destroyed");
  return 0;
}

int kr_app_server_enable(kr_app_server *server) {
  if (server == NULL) return -1;
  pthread_create(&server->thread, NULL, server_loop, (void *)server);
  return 0;
}

kr_app_server *kr_app_server_create(kr_app_server_setup *setup) {
  kr_app_server *server;
  server = server_init(setup->appname, setup->sysname);
  if (server == NULL) {
    return NULL;
  }
  kr_pool_setup pool_setup;
  pool_setup.size = sizeof(kr_app_server_map);
  pool_setup.slices = 8;
  pool_setup.shared = 0;
  pool_setup.overlay_sz = 0;
  server->maps = kr_pool_create(&pool_setup);
  return server;
}

void kr_app_server_slice_address(kr_app_address_sliced *sliced, char *addr) {
  char *slice;
  memset(sliced, 0, sizeof(kr_app_address_sliced));
  slice = addr;
  while ((slice = strchr(slice, '/'))) {
    sliced->slice[sliced->slices] = slice + 1;
    sliced->slices++;
    slice[0] = '\0';
    slice += 1;
    if (!strlen(slice)) {
      sliced->slices = 0;
      printke("zero length address slice");
      return;
    }
    if (sliced->slices == 3) break;
  }
}

void kr_app_server_address_slices_print(kr_app_address_sliced *sliced) {
  int i;
  printk("%zu slices", sliced->slices);
  for (i = 0; i < sliced->slices; i++) {
    printk("Slice %d: %s", i, sliced->slice[i]);
  }
}

int kr_app_server_route(kr_app_server *app, kr_crate2 *crate) {
  int i;
  int len;
  kr_app_address_sliced sliced;
  kr_app_server_map *map;
  if ((app == NULL) || (crate == NULL)) return -1;
  i = 0;
  printk("Routing: %s", crate->address);
  kr_app_server_slice_address(&sliced, crate->address);
  if (sliced.slices < 1) return -1;
  if (sliced.slices > 2) {
    printke("to many slices in address");
    return -2;
  }
  if ((sliced.slices == 1) && (crate->method != KR_GET)) {
    printk("incompatible slice count and method");
    return -3;
  }
  //printk("method and slice count compatible");
  while ((map = kr_pool_iterate_active(app->maps, &i))) {
    //printk("Checking Mapper Prefix: %s", mapper->prefix);
    len = strlen(map->prefix + 1);
    if (((strlen(sliced.slice[0])) == len)
     && (memcmp(map->prefix + 1, sliced.slice[0], len) == 0)) {
      /*
      if ((crate->method == KR_POST) && (sliced.slices == 1)) {
        printk("Found route! Its a post to %s!", sliced.slice[0]);
        return 0;
      }
      */
      if ((crate->method == KR_GET) && (sliced.slices == 1)) {
        printk("Found route! GET list of %s!", sliced.slice[0]);
        return 0;
      }
      if ((crate->method == KR_GET) && (sliced.slices == 2)) {
        printk("Found route! GET %s from %s!", sliced.slice[1],
         sliced.slice[0]);
        return 0;
      }
      if ((crate->method == KR_PATCH) && (sliced.slices == 2)) {
        printk("Found route! PATCH for %s on %s!", sliced.slice[1],
         sliced.slice[0]);
        return 0;
      }
      if ((crate->method == KR_DELETE) && (sliced.slices == 2)) {
        printk("Found route! DELETE %s from %s!", sliced.slice[1],
         sliced.slice[0]);
        return 0;
      }
      if ((crate->method == KR_PUT) && (sliced.slices == 2)) {
        printk("Found route! PUT %s in %s!", sliced.slice[1],
         sliced.slice[0]);
        printk("I should call %p with %p!", map->create, map->ptr);
        return 0;
      }
      printke("hrm wtf!");
      return -4;
    }
  }
  printke("Route not found :/");
  kr_app_server_address_slices_print(&sliced);
  return -5;
}

static void router_test(kr_app_server *server) {
  kr_crate2 crate;
  memset(&crate, 0, sizeof(kr_crate2));

  strcpy(crate.address, "/mixer/Music3");
  crate.method = KR_PUT;
  kr_app_server_route(server, &crate);

  strcpy(crate.address, "/mixer/Music3");
  crate.method = KR_GET;
  kr_app_server_route(server, &crate);

  strcpy(crate.address, "/mixer");
  crate.method = KR_GET;
  kr_app_server_route(server, &crate);

  strcpy(crate.address, "/mixer");
  crate.method = KR_PUT;
  kr_app_server_route(server, &crate);

  strcpy(crate.address, "/mixer/Music3");
  crate.method = KR_DELETE;
  kr_app_server_route(server, &crate);

  strcpy(crate.address, "/mixer");
  crate.method = KR_PUT;
  kr_app_server_route(server, &crate);
}

int kr_app_server_map_destroy(kr_app_server_map *map) {
  return -1;
}

kr_app_server_map *kr_app_server_map_create(kr_app_server *server, kr_app_server_map_setup *setup) {
  if ((server == NULL) || (setup == NULL)) return NULL;
  //check all for null basically
  void *slice;
  slice = kr_pool_slice(server->maps);
  if (slice == NULL) return NULL;
  memcpy(slice, setup, sizeof(kr_app_server_map));
  printk("Added mapper for: %s", setup->prefix);
  router_test(server);
  return NULL;
}
