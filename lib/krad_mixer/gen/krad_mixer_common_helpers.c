#include "krad_mixer_common_helpers.h"

kr_mixer_path_info_member kr_mixer_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"kr_mixer_path_info_type",len)) {
    return KR_MIXER_PATH_INFO_TYPE;
  }
  if (!strncmp(string,"kr_mixer_path_info_channels",len)) {
    return KR_MIXER_PATH_INFO_CHANNELS;
  }
  if (!strncmp(string,"kr_mixer_path_info_lowpass",len)) {
    return KR_MIXER_PATH_INFO_LOWPASS;
  }
  if (!strncmp(string,"kr_mixer_path_info_highpass",len)) {
    return KR_MIXER_PATH_INFO_HIGHPASS;
  }
  if (!strncmp(string,"kr_mixer_path_info_analog",len)) {
    return KR_MIXER_PATH_INFO_ANALOG;
  }
  if (!strncmp(string,"kr_mixer_path_info_eq",len)) {
    return KR_MIXER_PATH_INFO_EQ;
  }
  if (!strncmp(string,"kr_mixer_path_info_volume",len)) {
    return KR_MIXER_PATH_INFO_VOLUME;
  }
  return -1;
}

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

int kr_mixer_path_info_patch_apply(struct kr_mixer_path_info *info, kr_mixer_path_info_patch *patch) {
  const ptrdiff_t off[7] = { offsetof(struct kr_mixer_path_info, type), 
    offsetof(struct kr_mixer_path_info, channels), offsetof(struct kr_mixer_path_info, lowpass), 
    offsetof(struct kr_mixer_path_info, highpass), offsetof(struct kr_mixer_path_info, analog), 
    offsetof(struct kr_mixer_path_info, eq), offsetof(struct kr_mixer_path_info, volume)
  };
  const size_t sz[7] = { sizeof(info->type), 
    sizeof(info->channels), sizeof(info->lowpass), 
    sizeof(info->highpass), sizeof(info->analog), 
    sizeof(info->eq), sizeof(info->volume)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_mixer_path_info_init(struct kr_mixer_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_mixer_path_info));
  kr_lowpass_info_init(&st->lowpass);
  kr_highpass_info_init(&st->highpass);
  kr_analog_info_init(&st->analog);
  kr_eq_info_init(&st->eq);
  kr_volume_info_init(&st->volume);

  return 0;
}

int kr_mixer_path_info_valid(struct kr_mixer_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_lowpass_info_valid(&st->lowpass);
  kr_highpass_info_valid(&st->highpass);
  kr_analog_info_valid(&st->analog);
  kr_eq_info_valid(&st->eq);
  kr_volume_info_valid(&st->volume);

  return 0;
}

int kr_mixer_path_info_random(struct kr_mixer_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_mixer_path_info));
  kr_lowpass_info_random(&st->lowpass);
  kr_highpass_info_random(&st->highpass);
  kr_analog_info_random(&st->analog);
  kr_eq_info_random(&st->eq);
  kr_volume_info_random(&st->volume);

  return 0;
}

int kr_mixer_path_patch_init(struct kr_mixer_path_patch *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_mixer_path_patch));

  return 0;
}

int kr_mixer_path_patch_valid(struct kr_mixer_path_patch *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 16; i++) {
    if (!st->ctl[i]) {
      break;
    }
    if (i == 15 && st->ctl[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_mixer_path_patch_random(struct kr_mixer_path_patch *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_mixer_path_patch));
  for (i = 0; i < 16; i++) {
    scale = (double)25 / RAND_MAX;
    st->ctl[i] = 97 + floor(rand() * scale);
    if (i == 15) {
      st->ctl[15] = '\0';
    }
  }

  return 0;
}

