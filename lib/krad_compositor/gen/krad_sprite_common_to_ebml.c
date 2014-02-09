#include "krad_sprite_common_to_ebml.h"

int kr_sprite_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_sprite_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_sprite_info *)st;
  res += kr_ebml_pack_string(ebml, 0xe1, actual->filename);
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->rate);
  res += kr_ebml_pack_int16(ebml, 0xe1, actual->w);
  res += kr_ebml_pack_int16(ebml, 0xe1, actual->h);

  return res;
}
