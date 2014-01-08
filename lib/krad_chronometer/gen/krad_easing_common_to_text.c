#include "krad_easing_common_to_text.h"

int kr_easing_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_easing *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_easing *)st;

  res += snprintf(&text[res],max-res,"kr_easing : %u \n",*actual);

  return res;
}

