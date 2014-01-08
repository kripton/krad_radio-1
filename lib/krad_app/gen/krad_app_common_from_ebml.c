#include "krad_app_common_from_ebml.h"

int kr_app_method_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_app_method *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_app_method *)st;

  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}

int kr_crate2_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_crate2 *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_crate2 *)st;

  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->address , sizeof(actual->address));
  uber.actual = &(actual->method);
  uber.type = DEBML_KR_APP_METHOD;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  uber.actual = &(actual->payload_type);
  uber.type = DEBML_KR_RADIO_PAYLOAD_TYPE;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  index = kr_radio_payload_type_to_index(actual->payload_type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->payload);
  uber.actual = &(uber_sub);
  uber.type = DEBML_KR_RADIO_PAYLOAD;
  res += info_unpack_fr_ebml(&ebml[res],&uber);

  return res;
}

