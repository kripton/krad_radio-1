#include "krad_codec_header_to_json.h"

int krad_codec_t_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  krad_codec_t *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (krad_codec_t *)st;
  type = kr_strfr_krad_codec_t(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}
int krad_container_type_t_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  krad_container_type_t *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (krad_container_type_t *)st;
  type = kr_strfr_krad_container_type_t(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}
