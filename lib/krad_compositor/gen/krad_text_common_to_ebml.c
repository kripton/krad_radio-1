#include "krad_text_common_to_ebml.h"

int kr_text_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_text_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_text_info *)st;

  res += kr_ebml_pack_string(ebml, 0xe1, actual->string);
  res += kr_ebml_pack_string(ebml, 0xe1, actual->font);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->red);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->green);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->blue);

  return res;
}

