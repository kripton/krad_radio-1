#include "krad_perspective_common_to_json.h"

int kr_pos_to_json(char *json, void *st, int32_t max) {
  int res;
  struct kr_pos *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_pos *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"x\" : %u,",actual->x);
  res += snprintf(&json[res],max-res,"\"y\" : %u",actual->y);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_perspective_view_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_perspective_view *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_perspective_view *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"top_left\": ");
  uber.actual = &(actual->top_left);
  uber.type = JSON_KR_POS;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"top_right\": ");
  uber.actual = &(actual->top_right);
  uber.type = JSON_KR_POS;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"bottom_left\": ");
  uber.actual = &(actual->bottom_left);
  uber.type = JSON_KR_POS;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"bottom_right\": ");
  uber.actual = &(actual->bottom_right);
  uber.type = JSON_KR_POS;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
