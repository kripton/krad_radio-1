#include "krad_vector_common_from_ebml.h"

int kr_vector_type_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_vector_type *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_vector_type *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}
int kr_vector_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  struct kr_vector_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_vector_info *)st;
  uber.actual = &(actual->type);
  uber.type = DEBML_KR_VECTOR_TYPE;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->red);
  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->green);
  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->blue);

  return res;
}
