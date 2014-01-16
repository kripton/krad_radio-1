#include "krad_app_common_to_json.h"

int kr_app_method_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_app_method *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_app_method *)st;

  type = kr_strfr_kr_app_method(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}

int kr_crate2_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_crate2 *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_crate2 *)st;

  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"address\" : \"%s\",",actual->address);
  res += snprintf(&json[res],max-res,"\"method\": ");
  uber.actual = &(actual->method);
  uber.type = JSON_KR_APP_METHOD;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"payload_type\": ");
  uber.actual = &(actual->payload_type);
  uber.type = JSON_KR_RADIO_PAYLOAD_TYPE;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  index = kr_radio_payload_type_to_index(actual->payload_type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->payload);
  uber.actual = &(uber_sub);
  uber.type = JSON_KR_RADIO_PAYLOAD;
  res += snprintf(&json[res],max-res,"\"payload\": ");
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}

