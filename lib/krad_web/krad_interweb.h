#include "krad_radio_version.h"
#include "krad_system.h"
#include "krad_ring.h"
#include "krad_io.h"
#include "krad_radio_client.h"
#include "krad_sha1.h"
#include "krad_base64.h"
#include "kr_client.h"

#ifndef KRAD_INTERWEB_SERVER_H
#define KRAD_INTERWEB_SERVER_H

/*
 * Here we are dealing with the interweb wild and free
 * as it may be, its various protocols and endless surprises
 */

#define MAX_REMOTES 16
#define KR_WEB_CLIENTS_MAX 64
#define KR_MAX_SDS KR_WEB_CLIENTS_MAX + MAX_REMOTES + 1
#define KR_WEBRTC_NAME_MAX 64

enum krad_interweb_shutdown {
  KRAD_INTERWEB_STARTING = -1,
  KRAD_INTERWEB_RUNNING,
  KRAD_INTERWEB_DO_SHUTDOWN,
  KRAD_INTERWEB_SHUTINGDOWN,
};

typedef struct kr_web_server kr_web_server;
typedef struct kr_web_server_setup kr_web_server_setup;
typedef struct kr_web_client kr_web_client;
typedef struct kr_websocket_client kr_websocket_client;

typedef struct kr_web_event kr_web_event;
typedef void (kr_web_event_cb)(kr_web_event *);
typedef int (kr_web_output_cb)(kr_io2_t *, uint8_t *buffer, size_t len);

typedef enum {
  KR_WEB_CLIENT_CREATE = 1,
  KR_WEB_STREAM_MONKEY,
  KR_WEB_BONGO_COCONUT
} kr_web_event_type;

struct kr_web_event {
  kr_web_event_type type;
  void *user;
  int fd;
  kr_web_output_cb *output_cb;
  kr_io2_t *in;
  kr_io2_t *out;
  void *in_state_tracker;
  size_t in_state_tracker_sz;
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

enum kr_web_client_type {
  INTERWEB_UNKNOWN = 0,
  KR_IWS_WS,
  KR_IWS_FILE,
  KR_IWS_STREAM_IN,
  KR_IWS_STREAM_OUT,
  KR_IWS_API,
  KR_REMOTE_LISTEN,
};

enum kr_web_verb {
  KR_IWS_INVALID = 0,
  KR_IWS_GET,
  KR_IWS_PUT,
  KR_IWS_SOURCE,
  KR_IWS_POST,
  KR_IWS_HEAD,
  KR_IWS_PATCH,
  KR_IWS_OPTIONS,
};

struct kr_websocket_client {
  uint8_t mask[4];
  uint32_t pos;
  uint64_t len;
  uint8_t *input;
  uint32_t input_len;
  uint8_t *output;
  uint32_t output_len;
  uint64_t frames;
  char key[96];
  char proto[96];
};

struct kr_web_client {
  int32_t sd;
  kr_web_server *server;
  kr_io2_t *in;
  kr_io2_t *out;
/*  kr_webrtc_user webrtc_user;*/
  int32_t drop_after_sync;
  int32_t type;
  uint32_t hdr_le;
  uint32_t hdr_pos;
  uint32_t hdrs_recvd;
  int32_t verb;
  char get[96];
  char mount[128];
  kr_websocket_client ws;
};

int32_t kr_web_server_listen_off(kr_web_server *server, char *interface, int32_t port);
int32_t kr_web_server_listen_on(kr_web_server *server, char *interface, int32_t port);
void kr_web_server_disable(kr_web_server *server);
void kr_web_server_run(kr_web_server *server);
void kr_web_server_destroy(kr_web_server **server);
kr_web_server *kr_web_server_create(kr_web_server_setup *setup);

#endif
