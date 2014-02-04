#ifndef KRAD_CLIENT_H
#define KRAD_CLIENT_H

#include <inttypes.h>

typedef struct kr_client kr_client;

#include "krad_app_common.h"
#include "krad_easing_common.h"
#include "krad_mixer_common.h"
#include "krad_compositor_common.h"
#include "krad_radio_common.h"
#include "krad_tags.h"

kr_client *kr_client_create (char *client_name);
void kr_delivery_recv (kr_client *client);
int kr_connect (kr_client *client, char *sysname);
int kr_connect_full (kr_client *client, char *sysname, int timeout_ms);
int kr_connect_remote (kr_client *client, char *host, int port, int timeout_ms);
int kr_connected (kr_client *client);
int kr_disconnect (kr_client *client);
int kr_client_destroy (kr_client **client);
int kr_client_local (kr_client *client);
int kr_client_get_fd (kr_client *client);
int kr_client_sync (kr_client *client);
int kr_client_push (kr_client *client);
int kr_client_want_out (kr_client *client);
void kr_subscribe (kr_client *client, uint32_t broadcast_id);
void kr_subscribe_all (kr_client *client);
int kr_poll (kr_client *client, uint32_t timeout_ms);
int kr_crate_send(kr_client *client, kr_crate *crate);
int kr_get(kr_client *client, char *address);
int kr_delete(kr_client *client, char *address);
int kr_streamer45(kr_client *client);
#endif
