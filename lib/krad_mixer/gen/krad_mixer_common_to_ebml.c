#include "krad_mixer_common_to_ebml.h"

int kr_mixer_channels_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_mixer_channels *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_mixer_channels *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}
int kr_mixer_path_type_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_mixer_path_type *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_mixer_path_type *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}
int kr_mixer_path_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  struct kr_mixer_path_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;
  uber.actual = &(actual->type);
  uber.type = EBML_KR_MIXER_PATH_TYPE;
  res += info_pack_to_ebml(&ebml[res],&uber);
  uber.actual = &(actual->channels);
  uber.type = EBML_KR_MIXER_CHANNELS;
  res += info_pack_to_ebml(&ebml[res],&uber);
  uber.actual = &(actual->lowpass);
  uber.type = EBML_KR_LOWPASS_INFO;
  res += info_pack_to_ebml(&ebml[res],&uber);
  uber.actual = &(actual->highpass);
  uber.type = EBML_KR_HIGHPASS_INFO;
  res += info_pack_to_ebml(&ebml[res],&uber);
  uber.actual = &(actual->analog);
  uber.type = EBML_KR_ANALOG_INFO;
  res += info_pack_to_ebml(&ebml[res],&uber);
  uber.actual = &(actual->eq);
  uber.type = EBML_KR_EQ_INFO;
  res += info_pack_to_ebml(&ebml[res],&uber);
  uber.actual = &(actual->volume);
  uber.type = EBML_KR_VOLUME_INFO;
  res += info_pack_to_ebml(&ebml[res],&uber);

  return res;
}
