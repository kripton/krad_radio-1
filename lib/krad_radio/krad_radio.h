#ifndef KRAD_RADIO_H
#define KRAD_RADIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>
#include <sys/stat.h>
#include <stddef.h>

#include "krad_radio_version.h"
#include "krad_system.h"
#include "krad_timer.h"
#include "krad_ticker.h"
#include "krad_tags.h"
#include "krad_ebml.h"
#include "krad_app_server.h"
#include "krad_radio_ipc.h"
#include "krad_mixer.h"
#include "krad_compositor.h"
#include "krad_transponder.h"
#include "krad_interweb.h"

typedef struct kr_radio kr_radio;

kr_radio *kr_radio_create(char *sysname);
int kr_radio_destroy(kr_radio *radio);

#endif
