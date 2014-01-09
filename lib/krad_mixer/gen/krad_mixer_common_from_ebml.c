#include "krad_mixer_common_from_ebml.h"

int kr_mixer_channels_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_mixer_channels *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_mixer_channels *)st;

  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}

int kr_mixer_control_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_mixer_control *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_mixer_control *)st;

  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}

int kr_mixer_path_type_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_mixer_path_type *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_mixer_path_type *)st;

  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}

int kr_mixer_adv_ctl_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_mixer_adv_ctl *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_mixer_adv_ctl *)st;

  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}

int kr_mixer_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_mixer_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_mixer_info *)st;

  res += kr_ebml2_unpack_element_uint32(ebml, NULL, &actual->period_size);
  res += kr_ebml2_unpack_element_uint32(ebml, NULL, &actual->sample_rate);
  res += kr_ebml2_unpack_element_uint32(ebml, NULL, &actual->inputs);
  res += kr_ebml2_unpack_element_uint32(ebml, NULL, &actual->buses);
  res += kr_ebml2_unpack_element_uint32(ebml, NULL, &actual->outputs);
  res += kr_ebml2_unpack_element_uint64(ebml, NULL, &actual->frames);
  res += kr_ebml2_unpack_element_uint64(ebml, NULL, &actual->timecode);
  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->clock , sizeof(actual->clock));

  return res;
}

int kr_mixer_path_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int i;
  int res;
  struct kr_mixer_path_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;

  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->name , sizeof(actual->name));
  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->bus , sizeof(actual->bus));
  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->crossfade_group , sizeof(actual->crossfade_group));
  uber.actual = &(actual->channels);
  uber.type = DEBML_KR_MIXER_CHANNELS;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  uber.actual = &(actual->type);
  uber.type = DEBML_KR_MIXER_PATH_TYPE;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->fade);
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
    res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->volume[i]);
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
    res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->map[i]);
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
    res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->mixmap[i]);
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
    res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->rms[i]);
  }
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
    res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->peak[i]);
  }
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->delay);
  uber.actual = &(actual->lowpass);
  uber.type = DEBML_KR_LOWPASS_INFO;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  uber.actual = &(actual->highpass);
  uber.type = DEBML_KR_HIGHPASS_INFO;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  uber.actual = &(actual->analog);
  uber.type = DEBML_KR_ANALOG_INFO;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  uber.actual = &(actual->eq);
  uber.type = DEBML_KR_EQ_INFO;
  res += info_unpack_fr_ebml(&ebml[res],&uber);

  return res;
}

int kr_mixer_path_patch_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_mixer_path_patch *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_mixer_path_patch *)st;

  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->ctl , sizeof(actual->ctl));
  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->val);
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->ms);

  return res;
}
