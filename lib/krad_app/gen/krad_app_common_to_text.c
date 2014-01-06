#include "krad_app_common_to_text.h"

int kr_app_method_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_app_method *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_app_method *)st;

  res += snprintf(&text[res],max-res,"kr_app_method : %u \n",*actual);

  return res;
}

int kr_crate2_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_crate2 *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_crate2 *)st;

  res += snprintf(&text[res],max-res,"address : %s \n",actual->address);
  uber.actual = &(actual->method);
  uber.type = TEXT_KR_APP_METHOD;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->info);
  uber.type = TEXT_KR_TRANSPONDER_PATH_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);

  return res;
}

