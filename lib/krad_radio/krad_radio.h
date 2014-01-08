#ifndef KRAD_RADIO_H
#define KRAD_RADIO_H

#include "krad_system.h"

typedef struct kr_radio kr_radio;

kr_radio *kr_radio_create(char *sysname);
int kr_radio_destroy(kr_radio *radio);

#endif
