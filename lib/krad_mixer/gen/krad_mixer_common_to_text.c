#include "krad_mixer_common_to_text.h"

int kr_mixer_channels_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_mixer_channels *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_mixer_channels *)st;

  res += snprintf(&text[res],max-res,"kr_mixer_channels : %u \n",*actual);

  return res;
}

int kr_mixer_path_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_mixer_path_type *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_mixer_path_type *)st;

  res += snprintf(&text[res],max-res,"kr_mixer_path_type : %u \n",*actual);

  return res;
}

int kr_mixer_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_mixer_path_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;

  uber.actual = &(actual->type);
  uber.type = TEXT_KR_MIXER_PATH_TYPE;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->channels);
  uber.type = TEXT_KR_MIXER_CHANNELS;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->lowpass);
  uber.type = TEXT_KR_LOWPASS_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->highpass);
  uber.type = TEXT_KR_HIGHPASS_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->analog);
  uber.type = TEXT_KR_ANALOG_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->eq);
  uber.type = TEXT_KR_EQ_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->volume);
  uber.type = TEXT_KR_VOLUME_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);

  return res;
}

