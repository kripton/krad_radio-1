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
typedef struct kr_app_server kr_app_server;
typedef struct kr_app_server_client kr_app_server_client;

//typedef void (kr_app_server_client_destroy_cb)(void *);

struct kr_app_server_setup {
  char appname[32];
  char sysname[64];
};

struct kr_app_server_info {
  uint64_t clients;
  uint64_t uptime;
};

/*
kr_app_server_client *kr_app_server_client_create(kr_app_server_client_setup *s);
int kr_app_server_client_destroy(kr_app_server_client *client);
*/
int kr_app_server_crate_reply(kr_app_server *server, kr_crate2 *crate);
int kr_app_server_map_destroy(kr_app_server *server, kr_router_map *map);
kr_router_map *kr_app_server_map_create(kr_app_server *server, kr_router_map_setup *setup);
kr_route *kr_app_server_route_create(kr_app_server *server, kr_route_setup *setup);
int kr_app_server_info_get(kr_app_server *server, kr_app_server_info *info);
int kr_app_server_disable(kr_app_server *server);
int kr_app_server_enable(kr_app_server *server);
int kr_app_server_destroy(kr_app_server *server);
kr_app_server *kr_app_server_create(kr_app_server_setup *setup);

#endif
