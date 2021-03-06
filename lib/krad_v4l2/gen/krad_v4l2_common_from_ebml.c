#include "krad_v4l2_common_from_ebml.h"

int kr_v4l2_state_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_v4l2_state *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_v4l2_state *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}
int kr_v4l2_path_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_v4l2_path_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_v4l2_path_info *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->width);
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->height);
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->num);
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->den);
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->format);

  return res;
}
int kr_v4l2_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  kr_v4l2_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_v4l2_info *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->dev);
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->priority);
  uber.actual = &(actual->state);
  uber.type = DEBML_KR_V4L2_STATE;
  res += info_unpack_fr_ebml(&ebml[res],&uber);

  return res;
}
