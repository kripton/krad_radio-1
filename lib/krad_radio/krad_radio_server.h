#include "krad_radio.h"
#include "krad_radio_client.h"
#include "krad_mixer_server.h"
#include "krad_compositor_server.h"
#include "krad_transponder_server.h"

#ifndef KRAD_RADIO_SERVER_H
#define KRAD_RADIO_SERVER_H

typedef struct kr_radio_client kr_radio_client;

struct kr_radio_client {
  kr_radio *radio;
  int valid;
};

typedef union {
  kr_mixer_path_info *portgroup;
  kr_remote_t *remote;
} krad_subunit_ptr_t;

int krad_radio_broadcast_subunit_created(kr_app_broadcaster *broadcaster, kr_address_t *address, void *subunit_in);
int krad_radio_broadcast_subunit_control(kr_app_broadcaster *broadcaster, kr_address_t *address_in, int control, float value, void *client);
int krad_radio_broadcast_subunit_update(kr_app_broadcaster *broadcaster, kr_address_t *address_in, int control, int type, void *value, void *client);
int krad_radio_broadcast_subunit_destroyed(kr_app_broadcaster *broadcaster, kr_address_t *address);

void krad_radio_pack_shipment_terminator(kr_ebml2_t *ebml);

void *kr_radio_client_create(void *ptr);
void kr_radio_client_destroy(void *ptr);
int kr_radio_client_handler(kr_io2_t *in, kr_io2_t *out, void *ptr);

#endif
