#ifndef KRAD_MIXER_SERVER_H
#define KRAD_MIXER_SERVER_H

#include "krad_mixer.h"
#include "krad_radio_server.h"

int kr_mixer_command(kr_io2_t *in, kr_io2_t *out, kr_radio_client *client);

#endif
