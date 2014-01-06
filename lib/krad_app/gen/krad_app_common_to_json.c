#include "krad_app_common_to_json.h"

int kr_app_method_to_json(char *json, void *st, int32_t max) {
  int res;
  kr_app_method *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_app_method *)st;

  res += snprintf(&json[res],max-res,"\"%u\"",*actual);

  return res;
}

int kr_crate2_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
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
  res += snprintf(&json[res],max-res,"}");

  return res;
}

