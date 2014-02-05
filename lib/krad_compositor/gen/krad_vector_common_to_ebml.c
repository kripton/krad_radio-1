#include "krad_vector_common_to_ebml.h"

int kr_vector_type_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_vector_type *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_vector_type *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}
int kr_vector_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  struct kr_vector_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_vector_info *)st;
  uber.actual = &(actual->type);
  uber.type = EBML_KR_VECTOR_TYPE;
  res += info_pack_to_ebml(&ebml[res],&uber);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->red);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->green);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->blue);

  return res;
}
