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

#include "krad_radio_version.h"
#include "krad_system.h"
#include "krad_ring.h"
#include "krad_io.h"
#include "krad_pool.h"
#include "krad_radio_ipc.h"

#include "krad_radio_client.h"

#ifdef KR_LINUX
#include <ifaddrs.h>
#endif

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

#define EBML_ID_KRAD_APP_CMD 0x4444

#define MAX_REMOTES 16
#define KRAD_APP_SERVER_MAX_CLIENTS 16
#define MAX_BROADCASTS 128
#define MAX_BROADCASTERS 16

enum krad_app_shutdown {
  KRAD_APP_STARTING = -1,
  KRAD_APP_RUNNING,
  KRAD_APP_DO_SHUTDOWN,
  KRAD_APP_SHUTINGDOWN,
};

typedef struct kr_app_server kr_app_server;
typedef struct kr_app_server kr_as;
typedef struct kr_app_server_client kr_app_server_client;
typedef struct kr_app_broadcaster kr_app_broadcaster;
typedef struct kr_broadcast_msg kr_broadcast_msg;

typedef struct kr_app_server_setup kr_app_server_setup;
typedef struct kr_app_server_request kr_app_server_request;

typedef void *(kr_app_server_client_create_cb)(void *);
typedef void (kr_app_server_client_destroy_cb)(void *);
//typedef int (kr_app_server_client_handler_cb)(kr_io2_t *in, kr_io2_t *out, void *);
typedef int (kr_app_server_client_handler_cb)(kr_app_server_request *);

struct kr_app_server_request {
  kr_io2_t *in;
  kr_io2_t *out;
  void *ptr;
  kr_app_server *app;
};

typedef enum {
  KR_GET = 1,
  KR_PUT,
  KR_POST,
  KR_PATCH,
  KR_DELETE
} kr_app_method;

typedef struct kr_crate2 kr_crate2;

struct kr_crate2 {
  char address[64];
  kr_app_method method;
  void *payload;
};

struct kr_app_server_setup {
  char appname[32];
  char sysname[64];
  kr_app_server_client_create_cb *client_create;
  kr_app_server_client_destroy_cb *client_destroy;
  kr_app_server_client_handler_cb *client_handler;
  void *app;
};

struct kr_broadcast_msg {
  unsigned char *buffer;
  uint32_t size;
  kr_app_server_client *skip_client;
};

struct kr_app_broadcaster {
  kr_app_server *app;
  krad_ringbuffer_t *msg_ring;
  int sockets[2];
};

struct kr_app_server_client {
  int sd;
  void *ptr;
  int broadcasts;
  kr_io2_t *in;
  kr_io2_t *out;
};

typedef void *(kr_app_server_create_handler)(void *, void *);
typedef int (kr_app_server_patch_handler)(void *, void *);
typedef int (kr_app_server_destroy_handler)(void *);

typedef struct kr_app_address_mapping kr_app_address_mapping;
typedef struct kr_app_address_mapper kr_app_address_mapper;
typedef struct kr_app_address_sliced kr_app_address_sliced;

struct kr_app_address_sliced {
  char *slice[4];
  size_t slices;
};

struct kr_app_address_mapping {
  char name[48];
  void *ptr;
  kr_app_address_mapper *mapper;
};

struct kr_app_address_mapper {
  char prefix[32];
  void *ptr; /* for create */
  kr_app_server_create_handler *create;
  kr_app_server_patch_handler *patch;
  kr_app_server_destroy_handler *destroy;
};

int kr_app_server_route(kr_app_server *app, kr_crate2 *crate);
int kr_app_server_add_mapper(kr_app_server *app, kr_app_address_mapper *mapper);

void kr_app_server_add_client_to_broadcast(kr_app_server *app, uint32_t broadcast_ebml_id);
int kr_broadcast_msg_destroy(kr_broadcast_msg **broadcast_msg);
kr_broadcast_msg *kr_broadcast_msg_create(kr_app_broadcaster *broadcaster, unsigned char *buffer, uint32_t size);
int kr_app_server_broadcaster_broadcast(kr_app_broadcaster *broadcaster, kr_broadcast_msg **broadcast_msg);
void kr_app_server_broadcaster_register_broadcast(kr_app_broadcaster *broadcaster, uint32_t broadcast_ebml_id);
kr_app_broadcaster *kr_app_server_broadcaster_register(kr_app_server *app_server);
int kr_app_server_broadcaster_unregister(kr_app_broadcaster **broadcaster);
int kr_app_server_current_client_is_subscriber(kr_app_server *app);
int kr_app_server_recvfd(kr_app_server_client *client);
int kr_app_server_disable_remote(kr_app_server *app_server, char *interface, int port);
int kr_app_server_enable_remote(kr_app_server *app_server, char *interface, uint16_t port);
uint32_t kr_app_server_num_clients(kr_app_server *app_server);
void kr_app_server_disable(kr_app_server *kr_app_server);
void kr_app_server_destroy(kr_app_server *app_server);
void kr_app_server_run(kr_app_server *kr_app_server);
kr_app_server *kr_app_server_create(kr_app_server_setup *setup);
#endif
