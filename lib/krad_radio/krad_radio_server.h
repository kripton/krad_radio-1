#include "krad_radio.h"
#include "krad_radio_client.h"
#include "krad_compositor_server.h"
#include "krad_transponder_server.h"
#include "krad_mixer_server.h"

#ifndef KRAD_RADIO_SERVER_H
#define KRAD_RADIO_SERVER_H

typedef struct kr_radio_client kr_radio_client;

struct kr_radio_client {
  kr_radio *radio;
  int valid;
};

void krad_radio_pack_shipment_terminator(kr_ebml2_t *ebml);

void *kr_radio_client_create(void *ptr);
void kr_radio_client_destroy(void *ptr);

struct kr_radio_server_cmd {
  kr_io2_t *in;
  kr_io2_t *out;
  kr_radio_client *client;
};

int kr_radio_server_handle(kr_app_server_request *request);

#endif
