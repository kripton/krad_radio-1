#include "krad_radio_version.h"
#include "krad_system.h"
#include "krad_ring.h"
#include "krad_io.h"
#include "krad_radio_client.h"
#include "krad_sha1.h"
#include "krad_base64.h"
#include "kr_client.h"

#ifndef KRAD_WEB_SERVER_SERVER_H
#define KRAD_WEB_SERVER_SERVER_H

/*
 * Here we are dealing with the interweb wild and free
 * as it may be, its various protocols and endless surprises
 */

#define MAX_REMOTES 16
#define KR_WEB_CLIENTS_MAX 64
#define KR_MAX_SDS KR_WEB_CLIENTS_MAX + MAX_REMOTES + 1
#define KR_WEBRTC_NAME_MAX 64

typedef enum {
  KR_WEB_CLIENT_CREATE = 1,
  KR_WEB_STREAM_MONKEY,
  KR_WEB_BONGO_COCONUT
} kr_web_event_type;

typedef struct kr_web_server kr_web_server;
typedef struct kr_web_server_setup kr_web_server_setup;
typedef struct kr_web_event kr_web_event;
typedef void (kr_web_event_cb)(kr_web_event *);
typedef ssize_t (kr_web_io_cb)(void *ctx, void *out, size_t max, void *in, size_t len);

struct kr_web_event {
  kr_web_event_type type;
  void *user;
  int fd;
  void *state_tracker;
  size_t state_tracker_sz;
  kr_io2_t *in;
  kr_io2_t *out;
  kr_web_io_cb *input_cb;
  kr_web_io_cb *output_cb;
};

struct kr_web_server_setup {
  char *sysname;
  int32_t port;
  char *headcode;
  char *htmlheader;
  char *htmlfooter;
  void *user;
  kr_web_event_cb *event_cb;
};

int kr_web_server_listen_off(kr_web_server *server, char *interface, int32_t port);
int kr_web_server_listen_on(kr_web_server *server, char *interface, int32_t port);
int kr_web_server_destroy(kr_web_server **server);
kr_web_server *kr_web_server_create(kr_web_server_setup *setup);

#endif
