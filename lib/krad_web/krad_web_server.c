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
#include <poll.h>
#include <pthread.h>
#include <netinet/tcp.h>
#include <ifaddrs.h>

#include "krad_web_server.h"
#include "gen/embed.h"

enum krad_interweb_shutdown {
  KR_WEB_SERVER_STARTING = -1,
  KR_WEB_SERVER_RUNNING,
  KR_WEB_SERVER_DO_SHUTDOWN,
  KR_WEB_SERVER_SHUTINGDOWN,
};

typedef enum {
  KR_WS_HTTP = 0,
  KR_WS_WEBSOCKET,
  KR_WS_GET_FILE,
  KR_WS_PUT_FILE,
  KR_WS_POST_FILE,
  KR_WS_GET_STREAM,
  KR_WS_PUT_STREAM,
  KR_WS_REST_API,
  KR_REMOTE_LISTEN,
} kr_web_client_type;

typedef enum {
  KR_WS_NONE = 0,
  KR_WS_GET,
  KR_WS_PUT,
  KR_WS_SOURCE,
  KR_WS_POST,
  KR_WS_HEAD,
  KR_WS_PATCH,
  KR_WS_OPTIONS,
} kr_http_method;

typedef struct kr_web_client kr_web_client;
typedef struct kr_http_tracker kr_http_tracker;
typedef struct kr_websocket_tracker kr_websocket_tracker;

struct kr_web_server {
  char sysname[64];
  int32_t tcp_sd[MAX_REMOTES];
  int32_t tcp_port[MAX_REMOTES];
  char *tcp_interface[MAX_REMOTES];
  int32_t shutdown;
  int32_t socket_count;
  krad_control_t krad_control;
  kr_web_client *clients;
  pthread_t thread;
  struct pollfd sockets[KR_MAX_SDS];
  int32_t socket_type[KR_MAX_SDS];
  kr_web_client *sockets_clients[KR_MAX_SDS];
  void *user;
  kr_web_event_cb *event_cb;
  int32_t uberport;
  char *headcode_source;
  char *htmlheader_source;
  char *htmlfooter_source;
  char *html;
  int32_t html_len;
  char *api_js;
  int32_t api_js_len;
  char *iface_js;
  int32_t iface_js_len;
  char *deviface_js;
  int32_t deviface_js_len;
  char *headcode;
  char *htmlheader;
  char *htmlfooter;
};

struct kr_websocket_tracker {
  uint8_t mask[4];
  uint32_t pos;
  uint64_t len;
  uint8_t *input;
  uint32_t input_len;
  uint8_t *output;
  uint32_t output_len;
};

struct kr_http_tracker {
  uint16_t header_pos;
  uint8_t header_endstate;
  uint8_t got_headers;
  kr_http_method method;
  char address[128];
  uint8_t drop_after_sync;
};

struct kr_web_client {
  int sd;
  kr_web_server *server;
  kr_io2_t *in;
  kr_io2_t *out;
  kr_web_client_type type;
  kr_http_tracker http;
  kr_websocket_tracker ws;
};

static int handle_client(kr_web_client *client);
static void disconnect_client(kr_web_server *server, kr_web_client *client);
static kr_web_client *accept_client(kr_web_server *server, int sd);
static void update_pollfds(kr_web_server *server);
static void *web_server_loop(void *arg);
static int kr_web_server_disable(kr_web_server *server);
static int kr_web_server_run(kr_web_server *server);

int strmatch(char *string1, char *string2) {
  int len1;
  if ((string1 == NULL) || (string2 == NULL)) {
    if ((string1 == NULL) && (string2 == NULL)) {
      return 1;
    }
    return 0;
  }
  len1 = strlen (string1);
  if (len1 == strlen(string2)) {
    if (strncmp(string1, string2, len1) == 0) {
      return 1;
    }
  }
  return 0;
}

#include "setup.c"
#include "socket.c"
#include "stream.c"
#include "http.c"
#include "websocket.c"
/* #include "webrtc.c" */
#include "file.c"

ssize_t rest_pack(void *ctx, void *out, size_t max, void *in, size_t len) {
  if (len < 1) return -1;
  if (max < len) return -2;
  /* if (ctx == NULL) return -3; */
  if (out == NULL) return -4;
  if (in == NULL) return -5;
  memcpy(out, in, len);
  return len;
}

ssize_t rest_unpack(void *ctx, void *out, size_t max, void *in, size_t len) {
  kr_crate2 crate;
  memset(&crate, 0, sizeof(crate));
  strcpy(crate.address, in + 4);
  crate.method = KR_GET;
  kr_crate2_to_json(out, &crate, max);
  return len;
}

int handle_rest_api(kr_web_client *client) {
  kr_web_server *server;
  kr_web_event event;
  server = client->server;
  event.type = KR_WEB_CLIENT_CREATE;
  event.fd = client->sd;
  kr_io2_restart(client->in);
  kr_io2_pack(client->in, (uint8_t *)(client->http.address), strlen(client->http.address) + 1);
  event.in = client->in;
  pack_http_header(client, "application/json");
  event.out = client->out;
  event.state_tracker = NULL;
  event.state_tracker_sz = 0;
  event.output_cb = rest_pack;
  event.input_cb = rest_unpack;
  event.user = server->user;
  server->event_cb(&event);
  client->sd = -1;
  return -1;
}

static int handle_client(kr_web_client *client) {
  int ret;
  ret = -1;
  if (client->type == KR_WS_HTTP) {
    ret = handle_http_request(client);
    if (client->type == KR_WS_HTTP) {
      return ret;
    }
  }
  switch (client->type) {
    case KR_WS_WEBSOCKET:
      ret = handle_websocket(client);
      break;
    case KR_WS_GET_FILE:
      ret = handle_get_file(client);
      break;
    case KR_WS_PUT_FILE:
      ret = handle_put_file(client);
      break;
    case KR_WS_POST_FILE:
      ret = handle_post_file(client);
      break;
    case KR_WS_REST_API:
      ret = handle_rest_api(client);
      break;
    case KR_WS_GET_STREAM:
      ret = handle_get_stream(client);
      break;
    case KR_WS_PUT_STREAM:
      ret = handle_put_stream(client);
      break;
    default:
      break;
  }
  return ret;
}

static void disconnect_client(kr_web_server *server, kr_web_client *client) {
  /*kr_webrtc_unregister(client);*/
  if (client->sd != -1) {
    close(client->sd);
  } else {
    printk("Web Server: looks like a app server client handoff");
  }
  kr_io2_destroy(&client->in);
  kr_io2_destroy(&client->out);
  memset(client, 0, sizeof(*client));
  printk("Web Server: Client Disconnected");
}

static kr_web_client *accept_client(kr_web_server *server, int sd) {
  kr_web_client *client;
  int outsize;
  int i;
  struct sockaddr_un sin;
  socklen_t slen;
  client = NULL;
  outsize = MAX(server->api_js_len, server->html_len);
  outsize = MAX(outsize + server->deviface_js_len,
   outsize + server->iface_js_len);
  outsize += 1024;
  outsize += outsize % 1024;
  while (client == NULL) {
    for(i = 0; i < KR_WEB_CLIENTS_MAX; i++) {
      if (server->clients[i].sd == 0) {
        client = &server->clients[i];
        break;
      }
    }
    if (client == NULL) {
      //printk ("Krad Interweb Server: Overloaded with clients!\n");
      return NULL;
    }
  }
  slen = sizeof(sin);
  client->sd = accept4(sd, (struct sockaddr *)&sin, &slen,
   SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (client->sd > -1) {
    krad_system_set_socket_nonblocking(client->sd);
    client->in = kr_io2_create();
    client->out = kr_io2_create_size(outsize);
    kr_io2_set_fd(client->in, client->sd);
    kr_io2_set_fd(client->out, client->sd);
    client->server = server;
    printk("Web Server: Client accepted!");
    return client;
  } else {
    printke("Web Server: Client NOT accepted!");
    client->sd = 0;
  }
  return NULL;
}

static void update_pollfds(kr_web_server *server) {
  int r;
  int c;
  int s;
  s = 0;
  server->sockets[s].fd = krad_controller_get_client_fd(&server->krad_control);
  server->sockets[s].events = POLLIN;
  s++;
  for (r = 0; r < MAX_REMOTES; r++) {
    if (server->tcp_sd[r] != 0) {
      server->sockets[s].fd = server->tcp_sd[r];
      server->sockets[s].events = POLLIN;
      s++;
      server->socket_type[s] = KR_REMOTE_LISTEN;
    }
  }
  for (c = 0; c < KR_WEB_CLIENTS_MAX; c++) {
    if (server->clients[c].sd > 0) {
      server->sockets[s].fd = server->clients[c].sd;
      server->sockets[s].events |= POLLIN;
      if (kr_io2_want_out (server->clients[c].out)) {
        server->sockets[s].events |= POLLOUT;
      }
      server->sockets_clients[s] = &server->clients[c];
      server->socket_type[s] = server->clients[c].type;
      s++;
    }
  }
  server->socket_count = s;
  //printk ("Krad Interweb Server: sockets rejiggerd!\n");
}

static void *web_server_loop(void *arg) {
  kr_web_server *server = (kr_web_server *)arg;
  kr_web_client *client;
  int32_t oret;
  int32_t ret;
  int32_t s;
  int32_t r;
  int32_t read_ret;
  krad_system_set_thread_name("kr_web");
  server->shutdown = KR_WEB_SERVER_RUNNING;
  while (!server->shutdown) {
    s = 0;
    update_pollfds(server);
    ret = poll(server->sockets, server->socket_count, -1);
    if ((ret < 1) || (server->shutdown) || (server->sockets[s].revents)) {
      break;
    }
    s++;
    for (r = 0; r < MAX_REMOTES; r++) {
      if (server->tcp_sd[r] != 0) {
        if ((server->tcp_sd[r] != 0)
         && (server->sockets[s].revents & POLLIN)) {
          accept_client(server, server->tcp_sd[r]);
          ret--;
        }
        s++;
      }
    }
    for (; ret > 0; s++) {
      if (server->sockets[s].revents) {
        ret--;
        client = server->sockets_clients[s];
        if (server->sockets[s].revents & POLLIN) {
          read_ret = kr_io2_read(client->in);
          if (read_ret > 0) {
            if (handle_client(client) < 0) {
              disconnect_client(server, client);
              continue;
            }
            if (kr_io2_want_out(client->out)) {
              server->sockets[s].events |= POLLOUT;
            }
          } else {
            if (read_ret == 0) {
              //printk("Interweb Server: Client EOF\n");
            }
            if (read_ret == -1) {
              printke("Web Server: Client Socket Error");
            }
            disconnect_client(server, client);
            continue;
          }
        }
        if (server->sockets[s].revents & POLLOUT) {
          oret = kr_io2_output(client->out);
          if (oret != 0) {
            printke("Web Server: panic dropping the client");
            disconnect_client(server, client);
            continue;
          }
          if (!(kr_io2_want_out (client->out))) {
            if (client->http.drop_after_sync == 1) {
              disconnect_client(server, client);
              continue;
            }
            server->sockets[s].events = POLLIN;
          }
        } else {
          if (server->sockets[s].revents & POLLHUP) {
            //printk ("Krad Interweb Server %d : POLLHUP\n", s);
            disconnect_client(server, client);
            continue;
          }
        }
        if (server->sockets[s].revents & POLLERR) {
          printke("Web Server: POLLERR");
          disconnect_client(server, client);
          continue;
        }
      }
    }
  }
  server->shutdown = KR_WEB_SERVER_SHUTINGDOWN;
  krad_controller_client_close(&server->krad_control);
  return NULL;
}

static int kr_web_server_disable(kr_web_server *server) {
  if (server == NULL) return -1;
  printk("Web Server: Disabling");
  if (!krad_controller_shutdown(&server->krad_control, &server->thread,
      30)) {
    krad_controller_destroy(&server->krad_control, &server->thread);
  }
  kr_web_server_listen_off(server, "", 0);
  printk("Web Server: Disabled");
  return 0;
}

static int kr_web_server_run(kr_web_server *server) {
  if (server == NULL) return -1;
  pthread_create(&server->thread, NULL, web_server_loop, (void *)server);
  return 0;
}

int kr_web_server_destroy(kr_web_server **serv) {
  int i;
  kr_web_server *server;
  if ((serv == NULL) || (*serv == NULL)) {
    return -1;
  }
  server = *serv;
  printk("Web Server: Destroying");
  if (server->shutdown != KR_WEB_SERVER_SHUTINGDOWN) {
    kr_web_server_disable(server);
  }
  for (i = 0; i < KR_WEB_CLIENTS_MAX; i++) {
    if (server->clients[i].sd > 0) {
      disconnect_client(server, &server->clients[i]);
    }
  }
  free(server->html);
  free(server->clients);
  free(server);
  *serv = NULL;
  printk ("Web Server: Destroyed");
  return 0;
}

kr_web_server *kr_web_server_create(kr_web_server_setup *setup) {
  kr_web_server *server;
  printk("Web Server: Creating");
  server = kr_allocz(1, sizeof(kr_web_server));
  if (krad_control_init(&server->krad_control)) {
    free(server);
    return NULL;
  }
  strcpy(server->sysname, setup->sysname);
  server->headcode_source = setup->headcode;
  server->htmlheader_source = setup->htmlheader;
  server->htmlfooter_source = setup->htmlfooter;
  server->event_cb = setup->event_cb;
  server->user = setup->user;
  server->shutdown = KR_WEB_SERVER_STARTING;
  server->clients = kr_allocz(KR_WEB_CLIENTS_MAX, sizeof(kr_web_client));
  web_server_setup_html(server);
  kr_web_server_listen_on(server, NULL, setup->port);
  kr_web_server_run(server);
  printk("Web Server: Created");
  return server;
}
