#include "krad_crate_to_text.h"

int kr_method_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_method *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_method *)st;

  res += snprintf(&text[res],max-res,"kr_method: %s \n",kr_strfr_kr_method(*actual));

  return res;
}

int kr_crate_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_crate *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_crate *)st;

  res += snprintf(&text[res],max-res,"address: %s \n",actual->address);
  uber.actual = &(actual->method);
  uber.type = TEXT_KR_METHOD;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->payload_type);
  uber.type = TEXT_KR_PAYLOAD_TYPE;
  res += info_pack_to_text(&text[res],&uber,max-res);
  index = kr_payload_type_to_index(actual->payload_type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->payload);
  uber.actual = &(uber_sub);
  uber.type = TEXT_KR_PAYLOAD;
  res += info_pack_to_text(&text[res],&uber,max-res);

  return res;
}

