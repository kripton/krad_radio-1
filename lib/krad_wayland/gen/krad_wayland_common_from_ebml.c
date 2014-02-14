#include "krad_wayland_common_from_ebml.h"

int kr_wayland_state_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_wayland_state *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_wayland_state *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}
int kr_wayland_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  kr_wayland_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_wayland_info *)st;
  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->display_name , sizeof(actual->display_name));
  uber.actual = &(actual->state);
  uber.type = DEBML_KR_WAYLAND_STATE;
  res += info_unpack_fr_ebml(&ebml[res],&uber);

  return res;
}
int kr_wayland_path_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_wayland_path_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_wayland_path_info *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->width);
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->height);
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->fullscreen);

  return res;
}
