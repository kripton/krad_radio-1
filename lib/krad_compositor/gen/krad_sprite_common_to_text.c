#include "krad_sprite_common_to_text.h"

int kr_sprite_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_sprite_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_sprite_info *)st;

  res += snprintf(&text[res],max-res,"filename: %s \n",actual->filename);
  res += snprintf(&text[res],max-res,"rate: %d \n",actual->rate);

  return res;
}

