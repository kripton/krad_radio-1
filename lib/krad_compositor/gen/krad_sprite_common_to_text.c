#include "krad_sprite_common_to_text.h"

int kr_sprite_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_sprite_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_sprite_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sfilename: %s \n",indent,actual->filename);
  res += snprintf(&text[res],max-res,"%srate: %d \n",indent,actual->rate);

  return res;
}
