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
#include <ifaddrs.h>

#include "krad_radio_version.h"
#include "krad_system.h"
#include "krad_ring.h"
#include "krad_io.h"
#include "krad_pool.h"
#include "krad_app_common.h"

#ifndef KRAD_APP_SERVER_H
#define KRAD_APP_SERVER_H

/*
 * Here we are dealing with the idea of an application server,
 * its clients, routing to its subcomponents, broadcasting its
 * events and minding its overall resources.
 *
 */

#define KRAD_APP_CLIENT_DOCTYPE "kr_app_client"
#define KRAD_APP_SERVER_DOCTYPE "kr_app_server"
#define KRAD_APP_DOCTYPE_VERSION KR_VERSION
#define KRAD_APP_DOCTYPE_READ_VERSION KR_VERSION

#define KR_APP_SERVER_CLIENTS_MAX 16
#define KR_APP_SERVER_BROADCASTS_MAX 128
#define KR_APP_SERVER_BROADCASTERS_MAX 16

typedef struct kr_app_server_setup kr_app_server_setup;
typedef struct kr_app_server_info kr_app_server_info;
//typedef struct kr_app_server_client_setup kr_app_server_client_setup;
typedef struct kr_app_server_map_setup kr_app_server_map_setup;
typedef struct kr_app_server kr_app_server;
typedef struct kr_app_server_client kr_app_server_client;
typedef struct kr_app_server_map kr_app_server_map;

//typedef void (kr_app_server_client_destroy_cb)(void *);
typedef void *(kr_app_server_map_create_handler)(void *, void *);
typedef int (kr_app_server_map_patch_handler)(void *, void *);
typedef int (kr_app_server_map_destroy_handler)(void *);

struct kr_app_server_setup {
  char appname[32];
  char sysname[64];
};

struct kr_app_server_info {
  uint64_t clients;
  uint64_t uptime;
};

struct kr_app_server_map_setup {
  char prefix[32];
  void *ptr; /* for create */
  kr_app_server_map_create_handler *create;
  kr_app_server_map_patch_handler *patch;
  kr_app_server_map_destroy_handler *destroy;
};

/*
kr_app_server_client *kr_app_server_client_create(kr_app_server_client_setup *s);
int kr_app_server_client_destroy(kr_app_server_client *client);
*/
int kr_app_server_map_destroy(kr_app_server_map *map);
kr_app_server_map *kr_app_server_map_create(kr_app_server *server, kr_app_server_map_setup *setup);
int kr_app_server_info_get(kr_app_server *server, kr_app_server_info *info);
int kr_app_server_disable(kr_app_server *server);
int kr_app_server_enable(kr_app_server *server);
int kr_app_server_destroy(kr_app_server *server);
kr_app_server *kr_app_server_create(kr_app_server_setup *setup);

#endif
