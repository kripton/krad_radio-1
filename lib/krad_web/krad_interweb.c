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

#include "krad_interweb.h"
#include "gen/embed.h"

static void web_server_update_pollfds(kr_web_server *server);
static void *web_server_loop(void *arg);
static void disconnect_client(kr_web_server *server, kr_web_client *client);
static kr_web_client *accept_client(kr_web_server *server, int sd);
int32_t web_client_get_stream(kr_web_client *client);
static void web_server_pack_buffer(kr_web_client *c, void *buffer, size_t sz);

static void web_server_pack_buffer(kr_web_client *c, void *buffer, size_t sz) {
  kr_io2_pack(c->out, buffer, sz);
}

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

#include "socket.c"
#include "websocket.c"
//#include "webrtc.c"
#include "setup.c"
#include "header_out.c"
#include "request.c"
#include "stream.c"
#include "file.c"

int32_t krad_interweb_client_handle(kr_web_client *client) {
  int32_t ret;
  ret = -1;
  if (client->type == INTERWEB_UNKNOWN) {
    ret = krad_interweb_client_handle_request(client);
    if (client->type == INTERWEB_UNKNOWN) {
      return ret;
    }
  }
  switch (client->type) {
    case KR_IWS_WS:
      ret = websocket_client_handle(client);
      break;
    case KR_IWS_FILE:
      ret = web_file_client_handle(client);
      break;
    case KR_IWS_STREAM_OUT:
      ret = web_stream_client_handle(client);
      break;
    case KR_IWS_STREAM_IN:
      ret = web_stream_in_client_handle(client);
      break;
    default:
      break;
  }
  return ret;
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

static void disconnect_client(kr_web_server *server, kr_web_client *client) {
  /*kr_webrtc_unregister(client);*/
  close(client->sd);
  client->sd = 0;
  client->type = 0;
  client->drop_after_sync = 0;
  client->hdr_le = 0;
  client->hdr_pos = 0;
  client->hdrs_recvd = 0;
  client->verb = 0;
  memset(&client->ws, 0, sizeof(kr_websocket_client));
  memset(client->get, 0, sizeof(client->get));
  kr_io2_destroy(&client->in);
  kr_io2_destroy(&client->out);
  printk("Web Server: Client Disconnected");
}

static void web_server_update_pollfds(kr_web_server *server) {
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
  server->shutdown = KRAD_INTERWEB_RUNNING;
  while (!server->shutdown) {
    s = 0;
    web_server_update_pollfds(server);
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
            if (krad_interweb_client_handle(client) < 0) {
              disconnect_client(server, client);
              continue;
            }
            if (kr_io2_want_out (client->out)) {
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
            if (client->drop_after_sync == 1) {
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
  server->shutdown = KRAD_INTERWEB_SHUTINGDOWN;
  krad_controller_client_close(&server->krad_control);
  return NULL;
}

void kr_web_server_disable(kr_web_server *server) {
  printk("Web Server: Disabling");
  if (!krad_controller_shutdown(&server->krad_control, &server->thread,
      30)) {
    krad_controller_destroy(&server->krad_control, &server->thread);
  }
  kr_web_server_listen_off(server, "", 0);
  printk("Web Server: Disabled");
}

void kr_web_server_destroy(kr_web_server **serv) {
  int i;
  kr_web_server *server;
  if ((serv == NULL) || (*serv == NULL)) {
    return;
  }
  server = *serv;
  printk("Web Server: Destroying");
  if (server->shutdown != KRAD_INTERWEB_SHUTINGDOWN) {
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
}

void kr_web_server_run(kr_web_server *server) {
  pthread_create(&server->thread, NULL, web_server_loop, (void *)server);
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
  server->shutdown = KRAD_INTERWEB_STARTING;
  server->clients = kr_allocz(KR_WEB_CLIENTS_MAX, sizeof(kr_web_client));
  web_server_setup_html(server);
  kr_web_server_listen_on(server, NULL, setup->port);
  kr_web_server_run(server);
  printk("Web Server: Created");
  return server;
}
