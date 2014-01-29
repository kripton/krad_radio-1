#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>

#include "krad_system.h"
#include "krad_easing.h"
#include "krad_sfx_common.h"
#include "krad_mixer.h"

#ifndef KR_VOLUME_H
#define KR_VOLUME_H

typedef struct kr_volume kr_volume;

kr_volume *kr_volume_create(int sample_rate);
void kr_volume_destroy(kr_volume *volume);

void kr_volume_set_sample_rate(kr_volume *volume, int sample_rate);
//void kr_volume_process(kr_volume_t *kr_volume, float *input, float *output,
//int nsamples);
void kr_volume_process2(kr_volume *volume, float *input, float *output,
 int nsamples, int broadcast);

/* Controls */
void kr_volume_set_level(kr_volume *volume, float level, int duration,
 kr_easing easing, void *user);

int kr_volume_info_get(kr_volume *volume, kr_volume_info *info);

#endif
