#include "krad_easing_common_to_ebml.h"

int kr_easing_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_easing *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_easing *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}
