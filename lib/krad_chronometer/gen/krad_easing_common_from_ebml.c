#include "krad_easing_common_from_ebml.h"

int kr_easing_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_easing *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_easing *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}
