#include "krad_vector_common_to_json.h"

int kr_vector_type_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_vector_type *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_vector_type *)st;
  type = kr_strfr_kr_vector_type(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}
int kr_vector_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_vector_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_vector_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"type\": ");
  uber.actual = &(actual->type);
  uber.type = JSON_KR_VECTOR_TYPE;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"red\" : %0.2f,",actual->red);
  res += snprintf(&json[res],max-res,"\"green\" : %0.2f,",actual->green);
  res += snprintf(&json[res],max-res,"\"blue\" : %0.2f",actual->blue);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
