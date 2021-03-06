#include "krad_v4l2_common_to_json.h"

int kr_v4l2_state_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_v4l2_state *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_v4l2_state *)st;
  type = kr_strfr_kr_v4l2_state(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}
int kr_v4l2_path_info_to_json(char *json, void *st, int32_t max) {
  int res;
  kr_v4l2_path_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_v4l2_path_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"width\" : %d,",actual->width);
  res += snprintf(&json[res],max-res,"\"height\" : %d,",actual->height);
  res += snprintf(&json[res],max-res,"\"num\" : %d,",actual->num);
  res += snprintf(&json[res],max-res,"\"den\" : %d,",actual->den);
  res += snprintf(&json[res],max-res,"\"format\" : %d",actual->format);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_v4l2_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  kr_v4l2_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_v4l2_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"dev\" : %d,",actual->dev);
  res += snprintf(&json[res],max-res,"\"priority\" : %d,",actual->priority);
  res += snprintf(&json[res],max-res,"\"state\": ");
  uber.actual = &(actual->state);
  uber.type = JSON_KR_V4L2_STATE;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
