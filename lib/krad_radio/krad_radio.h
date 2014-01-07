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

#include "krad_system.h"

typedef struct kr_radio kr_radio;

kr_radio *kr_radio_create(char *sysname);
int kr_radio_destroy(kr_radio *radio);

#endif
