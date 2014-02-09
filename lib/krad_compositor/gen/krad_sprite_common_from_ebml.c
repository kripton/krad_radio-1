#include "krad_sprite_common_from_ebml.h"

int kr_sprite_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_sprite_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_sprite_info *)st;
  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->filename , sizeof(actual->filename));
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->rate);
  res += kr_ebml2_unpack_element_int16(ebml, NULL, &actual->w);
  res += kr_ebml2_unpack_element_int16(ebml, NULL, &actual->h);

  return res;
}
