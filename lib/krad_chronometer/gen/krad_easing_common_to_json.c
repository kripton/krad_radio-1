#include "krad_easing_common_to_json.h"

int kr_easing_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_easing *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_easing *)st;
  type = kr_strfr_kr_easing(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}
