#include "krad_text_common_to_json.h"

int kr_text_info_to_json(char *json, void *st, int32_t max) {
  int res;
  struct kr_text_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_text_info *)st;

  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"string\" : \"%s\",",actual->string);
  res += snprintf(&json[res],max-res,"\"font\" : \"%s\",",actual->font);
  res += snprintf(&json[res],max-res,"\"red\" : %0.2f,",actual->red);
  res += snprintf(&json[res],max-res,"\"green\" : %0.2f,",actual->green);
  res += snprintf(&json[res],max-res,"\"blue\" : %0.2f",actual->blue);
  res += snprintf(&json[res],max-res,"}");

  return res;
}

