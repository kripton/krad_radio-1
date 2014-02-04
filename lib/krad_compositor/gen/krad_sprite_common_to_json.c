#include "krad_sprite_common_to_json.h"

int kr_sprite_info_to_json(char *json, void *st, int32_t max) {
  int res;
  struct kr_sprite_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_sprite_info *)st;

  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"filename\" : \"%s\",",actual->filename);
  res += snprintf(&json[res],max-res,"\"rate\" : %d",actual->rate);
  res += snprintf(&json[res],max-res,"}");

  return res;
}

