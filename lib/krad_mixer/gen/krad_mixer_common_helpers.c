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

int kr_mixer_path_type_to_index(int val) {
  switch (val) {
    case KR_MXR_SOURCE:
      return 0;
    case KR_MXR_INPUT:
      return 1;
    case KR_MXR_BUS:
      return 2;
    case KR_MXR_OUTPUT:
      return 3;
  }
  return -1;
}

char *kr_strfr_kr_mixer_path_type(int val) {
  switch (val) {
    case KR_MXR_SOURCE:
      return "kr_mxr_source";
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
  if (!strcmp(string,"kr_mxr_source")) {
    return KR_MXR_SOURCE;
  }
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

int kr_mixer_path_info_init(void *st) {
  struct kr_mixer_path_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;
  memset(actual, 0, sizeof(struct kr_mixer_path_info));
  kr_lowpass_info_init(&actual->lowpass);
  kr_highpass_info_init(&actual->highpass);
  kr_analog_info_init(&actual->analog);
  kr_eq_info_init(&actual->eq);
  kr_volume_info_init(&actual->volume);

  return 0;
}

int kr_mixer_path_info_valid(void *st) {
  struct kr_mixer_path_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;
  kr_lowpass_info_valid(&actual->lowpass);
  kr_highpass_info_valid(&actual->highpass);
  kr_analog_info_valid(&actual->analog);
  kr_eq_info_valid(&actual->eq);
  kr_volume_info_valid(&actual->volume);

  return 0;
}

int kr_mixer_path_info_random(void *st) {
  struct kr_mixer_path_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;
  memset(actual, 0, sizeof(struct kr_mixer_path_info));
  kr_lowpass_info_random(&actual->lowpass);
  kr_highpass_info_random(&actual->highpass);
  kr_analog_info_random(&actual->analog);
  kr_eq_info_random(&actual->eq);
  kr_volume_info_random(&actual->volume);

  return 0;
}

int kr_mixer_path_patch_init(void *st) {
  struct kr_mixer_path_patch *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_path_patch *)st;
  memset(actual, 0, sizeof(struct kr_mixer_path_patch));

  return 0;
}

int kr_mixer_path_patch_valid(void *st) {
  struct kr_mixer_path_patch *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_path_patch *)st;
  for (i = 0; i < 16; i++) {
    if (!actual->ctl[i]) {
      break;
    }
    if (i == 15 && actual->ctl[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_mixer_path_patch_random(void *st) {
  struct kr_mixer_path_patch *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_mixer_path_patch *)st;
  memset(actual, 0, sizeof(struct kr_mixer_path_patch));
  for (i = 0; i < 16; i++) {
    scale = (double)25 / RAND_MAX;
    actual->ctl[i] = 97 + floor(rand() * scale);
    if (i == 15) {
      actual->ctl[15] = '\0';
    }
  }

  return 0;
}

