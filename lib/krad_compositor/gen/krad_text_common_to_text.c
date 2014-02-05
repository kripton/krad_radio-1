#include "krad_text_common_to_text.h"

int kr_text_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_text_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_text_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sstring: %s \n",indent,actual->string);
  res += snprintf(&text[res],max-res,"%sfont: %s \n",indent,actual->font);
  res += snprintf(&text[res],max-res,"%sred: %0.2f \n",indent,actual->red);
  res += snprintf(&text[res],max-res,"%sgreen: %0.2f \n",indent,actual->green);
  res += snprintf(&text[res],max-res,"%sblue: %0.2f \n",indent,actual->blue);

  return res;
}
