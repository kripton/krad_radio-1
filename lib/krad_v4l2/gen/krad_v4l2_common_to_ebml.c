#include "krad_v4l2_common_to_ebml.h"

int kr_v4l2_state_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_v4l2_state *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_v4l2_state *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}
int kr_v4l2_path_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_v4l2_path_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_v4l2_path_info *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->width);
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->height);
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->num);
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->den);
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->format);

  return res;
}
int kr_v4l2_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  kr_v4l2_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_v4l2_info *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->dev);
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->priority);
  uber.actual = &(actual->state);
  uber.type = EBML_KR_V4L2_STATE;
  res += info_pack_to_ebml(&ebml[res],&uber);

  return res;
}
