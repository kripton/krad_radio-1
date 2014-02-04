#include "krad_text_common_to_text.h"

int kr_text_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_text_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_text_info *)st;

  res += snprintf(&text[res],max-res,"string : %s \n",actual->string);
  res += snprintf(&text[res],max-res,"font : %s \n",actual->font);
  res += snprintf(&text[res],max-res,"red : %0.2f \n",actual->red);
  res += snprintf(&text[res],max-res,"green : %0.2f \n",actual->green);
  res += snprintf(&text[res],max-res,"blue : %0.2f \n",actual->blue);

  return res;
}

