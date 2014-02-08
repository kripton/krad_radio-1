#include "kr_aux_common_from_ebml.h"

int kr_aux_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_aux_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_aux_info *)st;
  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->monkeyname , sizeof(actual->monkeyname));

  return res;
}
int kr_aux_path_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_aux_path_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_aux_path_info *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->ncoconuts);

  return res;
}
