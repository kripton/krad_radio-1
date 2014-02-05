#include "krad_vector_common_to_text.h"

int kr_vector_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_vector_type *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_vector_type *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_vector_type(*actual));

  return res;
}
int kr_vector_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_vector_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_vector_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%stype:",indent);
  uber.actual = &(actual->type);
  uber.type = TEXT_KR_VECTOR_TYPE;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%sred: %0.2f \n",indent,actual->red);
  res += snprintf(&text[res],max-res,"%sgreen: %0.2f \n",indent,actual->green);
  res += snprintf(&text[res],max-res,"%sblue: %0.2f \n",indent,actual->blue);

  return res;
}
