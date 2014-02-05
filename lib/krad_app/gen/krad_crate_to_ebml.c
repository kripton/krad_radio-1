#include "krad_crate_to_ebml.h"

int kr_method_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_method *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_method *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}
int kr_crate_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_crate *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_crate *)st;
  res += kr_ebml_pack_string(ebml, 0xe1, actual->address);
  uber.actual = &(actual->method);
  uber.type = EBML_KR_METHOD;
  res += info_pack_to_ebml(&ebml[res],&uber);
  uber.actual = &(actual->payload_type);
  uber.type = EBML_KR_PAYLOAD_TYPE;
  res += info_pack_to_ebml(&ebml[res],&uber);
  index = kr_payload_type_to_index(actual->payload_type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->payload);
  uber.actual = &(uber_sub);
  uber.type = EBML_KR_PAYLOAD;
  res += info_pack_to_ebml(&ebml[res],&uber);

  return res;
}
