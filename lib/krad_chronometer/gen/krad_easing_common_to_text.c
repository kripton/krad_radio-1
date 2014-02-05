#include "krad_easing_common_to_text.h"

int kr_easing_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_easing *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_easing *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_easing(*actual));

  return res;
}
