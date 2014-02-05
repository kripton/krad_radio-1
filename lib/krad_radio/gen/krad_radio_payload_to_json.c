#include "krad_radio_payload_to_json.h"

int kr_nil_to_json(char *json, void *st, int32_t max) {
  int res;
  struct kr_nil *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_nil *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"n\" : %d",actual->n);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_payload_type_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_payload_type *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_payload_type *)st;
  type = kr_strfr_kr_payload_type(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}
int kr_payload_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_payload *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_payload *)uber_actual->actual;
  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->nil);
      uber.type = JSON_KR_NIL;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->transponder_path_info);
      uber.type = JSON_KR_TRANSPONDER_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->mixer_path_info);
      uber.type = JSON_KR_MIXER_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 3: {
      uber.actual = &(actual->compositor_path_info);
      uber.type = JSON_KR_COMPOSITOR_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
  }


  return res;
}
