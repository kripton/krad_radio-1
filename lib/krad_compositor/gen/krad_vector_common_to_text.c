#include "krad_vector_common_to_text.h"

int kr_vector_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_vector_type *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_vector_type *)st;

  res += snprintf(&text[res],max-res,"kr_vector_type : %u \n",*actual);

  return res;
}

int kr_vector_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_vector_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_vector_info *)st;

  uber.actual = &(actual->type);
  uber.type = TEXT_KR_VECTOR_TYPE;
  res += info_pack_to_text(&text[res],&uber,max-res);
  res += snprintf(&text[res],max-res,"red : %0.2f \n",actual->red);
  res += snprintf(&text[res],max-res,"green : %0.2f \n",actual->green);
  res += snprintf(&text[res],max-res,"blue : %0.2f \n",actual->blue);

  return res;
}

