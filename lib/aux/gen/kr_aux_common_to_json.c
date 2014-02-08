#include "kr_aux_common_to_json.h"

int kr_aux_info_to_json(char *json, void *st, int32_t max) {
  int res;
  struct kr_aux_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_aux_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"monkeyname\" : \"%s\"",actual->monkeyname);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_aux_path_info_to_json(char *json, void *st, int32_t max) {
  int res;
  struct kr_aux_path_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_aux_path_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"ncoconuts\" : %d",actual->ncoconuts);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
