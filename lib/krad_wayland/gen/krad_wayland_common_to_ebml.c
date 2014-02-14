#include "krad_wayland_common_to_ebml.h"

int kr_wayland_state_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_wayland_state *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_wayland_state *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}
int kr_wayland_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  kr_wayland_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_wayland_info *)st;
  res += kr_ebml_pack_string(ebml, 0xe1, actual->display_name);
  uber.actual = &(actual->state);
  uber.type = EBML_KR_WAYLAND_STATE;
  res += info_pack_to_ebml(&ebml[res],&uber);

  return res;
}
int kr_wayland_path_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_wayland_path_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_wayland_path_info *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->width);
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->height);
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->fullscreen);

  return res;
}
