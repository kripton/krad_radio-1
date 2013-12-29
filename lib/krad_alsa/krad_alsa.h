#ifndef KRAD_ALSA_H
#define KRAD_ALSA_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <stdint.h>
#include <limits.h>
#include <endian.h>

#include <alsa/asoundlib.h>

#include "krad_system.h"

typedef struct kr_alsa kr_alsa;

int kr_alsa_destroy(kr_alsa *alsa);
kr_alsa *kr_alsa_create(int card_num);

#endif
