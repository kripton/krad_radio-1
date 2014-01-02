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
#ifdef KR_LINUX
#include <ifaddrs.h>
#endif

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
#define KR_WEB_KRCLIENTS_MAX 64
#define KR_MAX_SDS KR_WEB_CLIENTS_MAX + KR_WEB_KRCLIENTS_MAX + MAX_REMOTES + 1
#define KR_WEBRTC_NAME_MAX 64

#define WS_MASK_BIT 0x80  // 10000000
#define WS_FIN_FRM 0x80   // 10000000
#define WS_CONT_FRM 0x00  // 00000000
#define WS_TEXT_FRM 0x01  // 00000001
#define WS_BIN_FRM 0x02   // 00000010
#define WS_CLOSE_FRM 0x08 // 00001000
#define WS_PING_FRM 0x09  // 00001001
#define WS_PONG_FRM 0x0a  // 00001010

enum krad_interweb_shutdown {
  KRAD_INTERWEB_STARTING = -1,
  KRAD_INTERWEB_RUNNING,
  KRAD_INTERWEB_DO_SHUTDOWN,
  KRAD_INTERWEB_SHUTINGDOWN,
};

typedef struct kr_web_server kr_web_server;
typedef struct kr_web_client kr_web_client;
typedef struct kr_websocket_client kr_websocket_client;

typedef struct kr_webrtc_user kr_webrtc_user;
typedef struct kr_webrtc_signal kr_webrtc_signal;

struct kr_webrtc_user {
  int active;
  char name[KR_WEBRTC_NAME_MAX];
};

enum kr_webrtc_signal_type {
  CALL = 1,
  ANSWER,
  HANGUP
};

struct kr_webrtc_signal {
  char from[KR_WEBRTC_NAME_MAX];
  char to[KR_WEBRTC_NAME_MAX];
  int32_t signal_type;
  char sdp[4096];
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
  KR_APP,
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
  uint32_t shaked;
  char key[96];
  char proto[96];
  kr_client_t *krclient;
};

struct kr_web_client {
  int32_t sd;
  kr_web_server *server;
  kr_io2_t *in;
  kr_io2_t *out;
  kr_webrtc_user webrtc_user;
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

void kr_webrtc_register(kr_web_client *client, char *name);
void kr_webrtc_unregister(kr_web_client *client);
void kr_webrtc_list_users(kr_web_client *client);
void kr_webrtc_call(kr_web_client *client, char *to, char *from, char *sdp);
void kr_webrtc_answer(kr_web_client *client, char *to, char *from, char *sdp);
void kr_webrtc_candidate(kr_web_client *client, char *to, char *from,
 char *candidate);

int32_t kr_web_server_listen_off(kr_web_server *server, char *interface, int32_t port);
int32_t kr_web_server_listen_on(kr_web_server *server, char *interface, int32_t port);

void kr_web_server_disable(kr_web_server *server);
void kr_web_server_destroy(kr_web_server **server);
void kr_web_server_run(kr_web_server *server);
kr_web_server *kr_web_server_create(char *sysname, int32_t port,
 char *headcode, char *htmlheader, char *htmlfooter);
#endif
