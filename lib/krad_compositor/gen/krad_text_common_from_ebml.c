#include "krad_text_common_from_ebml.h"

int kr_text_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_text_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_text_info *)st;

  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->string , sizeof(actual->string));
  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->font , sizeof(actual->font));
  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->red);
  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->green);
  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->blue);

  return res;
}

