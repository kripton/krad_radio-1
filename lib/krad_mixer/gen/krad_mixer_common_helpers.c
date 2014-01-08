#include "krad_mixer_common_helpers.h"

int kr_mixer_channels_to_index(int val) {
  switch (val) {
    case NIL:
      return 0;
    case MONO:
      return 1;
    case STEREO:
      return 2;
    case THREE:
      return 3;
    case QUAD:
      return 4;
    case FIVE:
      return 5;
    case SIX:
      return 6;
    case SEVEN:
      return 7;
    case EIGHT:
      return 8;
  }
  return -1;
}

char *kr_strfr_kr_mixer_channels(int val) {
  switch (val) {
    case NIL:
      return "nil";
    case MONO:
      return "mono";
    case STEREO:
      return "stereo";
    case THREE:
      return "three";
    case QUAD:
      return "quad";
    case FIVE:
      return "five";
    case SIX:
      return "six";
    case SEVEN:
      return "seven";
    case EIGHT:
      return "eight";
  }
  return NULL;
}

int kr_strto_kr_mixer_channels(char *string) {
  if (!strcmp(string,"nil")) {
    return NIL;
  }
  if (!strcmp(string,"mono")) {
    return MONO;
  }
  if (!strcmp(string,"stereo")) {
    return STEREO;
  }
  if (!strcmp(string,"three")) {
    return THREE;
  }
  if (!strcmp(string,"quad")) {
    return QUAD;
  }
  if (!strcmp(string,"five")) {
    return FIVE;
  }
  if (!strcmp(string,"six")) {
    return SIX;
  }
  if (!strcmp(string,"seven")) {
    return SEVEN;
  }
  if (!strcmp(string,"eight")) {
    return EIGHT;
  }

  return -1;
}

int kr_mixer_control_to_index(int val) {
  switch (val) {
    case KR_VOLUME:
      return 0;
    case KR_CROSSFADE:
      return 1;
    case KR_PEAK:
      return 2;
    case KR_CROSSFADE_GROUP:
      return 3;
  }
  return -1;
}

char *kr_strfr_kr_mixer_control(int val) {
  switch (val) {
    case KR_VOLUME:
      return "kr_volume";
    case KR_CROSSFADE:
      return "kr_crossfade";
    case KR_PEAK:
      return "kr_peak";
    case KR_CROSSFADE_GROUP:
      return "kr_crossfade_group";
  }
  return NULL;
}

int kr_strto_kr_mixer_control(char *string) {
  if (!strcmp(string,"kr_volume")) {
    return KR_VOLUME;
  }
  if (!strcmp(string,"kr_crossfade")) {
    return KR_CROSSFADE;
  }
  if (!strcmp(string,"kr_peak")) {
    return KR_PEAK;
  }
  if (!strcmp(string,"kr_crossfade_group")) {
    return KR_CROSSFADE_GROUP;
  }

  return -1;
}

int kr_mixer_path_type_to_index(int val) {
  switch (val) {
    case KR_MXR_INPUT:
      return 0;
    case KR_MXR_BUS:
      return 1;
    case KR_MXR_OUTPUT:
      return 2;
  }
  return -1;
}

char *kr_strfr_kr_mixer_path_type(int val) {
  switch (val) {
    case KR_MXR_INPUT:
      return "kr_mxr_input";
    case KR_MXR_BUS:
      return "kr_mxr_bus";
    case KR_MXR_OUTPUT:
      return "kr_mxr_output";
  }
  return NULL;
}

int kr_strto_kr_mixer_path_type(char *string) {
  if (!strcmp(string,"kr_mxr_input")) {
    return KR_MXR_INPUT;
  }
  if (!strcmp(string,"kr_mxr_bus")) {
    return KR_MXR_BUS;
  }
  if (!strcmp(string,"kr_mxr_output")) {
    return KR_MXR_OUTPUT;
  }

  return -1;
}

int kr_mixer_adv_ctl_to_index(int val) {
  switch (val) {
    case KR_MXR_SAMPLERATE_SET:
      return 0;
    case KR_MXR_PERIOD_SIZE_SET:
      return 1;
    case KR_MXR_CLOCK_SET:
      return 2;
  }
  return -1;
}

char *kr_strfr_kr_mixer_adv_ctl(int val) {
  switch (val) {
    case KR_MXR_SAMPLERATE_SET:
      return "kr_mxr_samplerate_set";
    case KR_MXR_PERIOD_SIZE_SET:
      return "kr_mxr_period_size_set";
    case KR_MXR_CLOCK_SET:
      return "kr_mxr_clock_set";
  }
  return NULL;
}

int kr_strto_kr_mixer_adv_ctl(char *string) {
  if (!strcmp(string,"kr_mxr_samplerate_set")) {
    return KR_MXR_SAMPLERATE_SET;
  }
  if (!strcmp(string,"kr_mxr_period_size_set")) {
    return KR_MXR_PERIOD_SIZE_SET;
  }
  if (!strcmp(string,"kr_mxr_clock_set")) {
    return KR_MXR_CLOCK_SET;
  }

  return -1;
}

int kr_mixer_info_init(void *st) {
  struct kr_mixer_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_info *)st;
  memset(actual, 0, sizeof(struct kr_mixer_info));
  actual->period_size = 1024;
  actual->sample_rate = 44100;
  actual->inputs = 0;
  actual->buses = 0;
  actual->outputs = 0;

  return 0;
}

int kr_mixer_info_valid(void *st) {
  struct kr_mixer_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_info *)st;
  if ( (actual->period_size < 32) || (actual->period_size > 4096) ) {
    return -2;
  }

  if ( (actual->sample_rate < 8000) || (actual->sample_rate > 192000) ) {
    return -3;
  }

  if ( (actual->inputs < 0) || (actual->inputs > 32) ) {
    return -4;
  }

  if ( (actual->buses < 0) || (actual->buses > 64) ) {
    return -5;
  }

  if ( (actual->outputs < 0) || (actual->outputs > 32) ) {
    return -6;
  }

  for (i = 0; i < 32; i++) {
    if (!actual->clock[i]) {
      break;
    }
    if (i == 31 && actual->clock[i]) {
      return -9;
    }
  }

  return 0;
}

int kr_mixer_info_random(void *st) {
  struct kr_mixer_info *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_info *)st;
  memset(actual, 0, sizeof(struct kr_mixer_info));
  scale = (double)4064 / RAND_MAX;
  actual->period_size = 32 + floor(rand() * scale);
  scale = (double)184000 / RAND_MAX;
  actual->sample_rate = 8000 + floor(rand() * scale);
  scale = (double)32 / RAND_MAX;
  actual->inputs = 0 + floor(rand() * scale);
  scale = (double)64 / RAND_MAX;
  actual->buses = 0 + floor(rand() * scale);
  scale = (double)32 / RAND_MAX;
  actual->outputs = 0 + floor(rand() * scale);
  for (i = 0; i < 32; i++) {
    scale = (double)25 / RAND_MAX;
    actual->clock[i] = 97 + floor(rand() * scale);
    if (i == 31) {
      actual->clock[31] = '\0';
    }
  }

  return 0;
}

int kr_mixer_path_info_init(void *st) {
  struct kr_mixer_path_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;
  memset(actual, 0, sizeof(struct kr_mixer_path_info));
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  kr_lowpass_info_init(&actual->lowpass);
  kr_highpass_info_init(&actual->highpass);
  kr_analog_info_init(&actual->analog);
  kr_eq_info_init(&actual->eq);

  return 0;
}

int kr_mixer_path_info_valid(void *st) {
  struct kr_mixer_path_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;
  for (i = 0; i < 64; i++) {
    if (!actual->name[i]) {
      break;
    }
    if (i == 63 && actual->name[i]) {
      return -2;
    }
  }
  for (i = 0; i < 64; i++) {
    if (!actual->bus[i]) {
      break;
    }
    if (i == 63 && actual->bus[i]) {
      return -3;
    }
  }
  for (i = 0; i < 64; i++) {
    if (!actual->crossfade_group[i]) {
      break;
    }
    if (i == 63 && actual->crossfade_group[i]) {
      return -4;
    }
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  kr_lowpass_info_valid(&actual->lowpass);
  kr_highpass_info_valid(&actual->highpass);
  kr_analog_info_valid(&actual->analog);
  kr_eq_info_valid(&actual->eq);

  return 0;
}

int kr_mixer_path_info_random(void *st) {
  struct kr_mixer_path_info *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;
  memset(actual, 0, sizeof(struct kr_mixer_path_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    actual->name[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      actual->name[63] = '\0';
    }
  }
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    actual->bus[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      actual->bus[63] = '\0';
    }
  }
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    actual->crossfade_group[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      actual->crossfade_group[63] = '\0';
    }
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  kr_lowpass_info_random(&actual->lowpass);
  kr_highpass_info_random(&actual->highpass);
  kr_analog_info_random(&actual->analog);
  kr_eq_info_random(&actual->eq);

  return 0;
}

