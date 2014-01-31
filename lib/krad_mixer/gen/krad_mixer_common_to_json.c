#include "krad_mixer_common_to_json.h"

int kr_mixer_channels_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_mixer_channels *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_mixer_channels *)st;

  type = kr_strfr_kr_mixer_channels(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}

int kr_mixer_path_type_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_mixer_path_type *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_mixer_path_type *)st;

  type = kr_strfr_kr_mixer_path_type(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}

int kr_mixer_path_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_mixer_path_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;

  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"type\": ");
  uber.actual = &(actual->type);
  uber.type = JSON_KR_MIXER_PATH_TYPE;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"channels\": ");
  uber.actual = &(actual->channels);
  uber.type = JSON_KR_MIXER_CHANNELS;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"lowpass\": ");
  uber.actual = &(actual->lowpass);
  uber.type = JSON_KR_LOWPASS_INFO;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"highpass\": ");
  uber.actual = &(actual->highpass);
  uber.type = JSON_KR_HIGHPASS_INFO;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"analog\": ");
  uber.actual = &(actual->analog);
  uber.type = JSON_KR_ANALOG_INFO;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"eq\": ");
  uber.actual = &(actual->eq);
  uber.type = JSON_KR_EQ_INFO;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"volume\": ");
  uber.actual = &(actual->volume);
  uber.type = JSON_KR_VOLUME_INFO;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}

