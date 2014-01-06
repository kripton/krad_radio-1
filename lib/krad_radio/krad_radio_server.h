#include "krad_radio.h"
#include "krad_radio_client.h"
#include "krad_compositor_server.h"
#include "krad_transponder_server.h"
#include "krad_mixer_server.h"

#ifndef KRAD_RADIO_SERVER_H
#define KRAD_RADIO_SERVER_H

void *kr_radio_client_create(void *ptr);
void kr_radio_client_destroy(void *ptr);
int kr_radio_server_handle(kr_app_server_request *request);

#endif
