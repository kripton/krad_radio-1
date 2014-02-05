#include "krad_x11_common_to_text.h"

int kr_x11_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_x11_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_x11_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sdisplay: %s \n",indent,actual->display);

  return res;
}
int kr_x11_path_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_x11_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_x11_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sdisplay: %s \n",indent,actual->display);
  res += snprintf(&text[res],max-res,"%swidth: %u \n",indent,actual->width);
  res += snprintf(&text[res],max-res,"%sheight: %u \n",indent,actual->height);
  res += snprintf(&text[res],max-res,"%snum: %u \n",indent,actual->num);
  res += snprintf(&text[res],max-res,"%sden: %u \n",indent,actual->den);
  res += snprintf(&text[res],max-res,"%sx: %d \n",indent,actual->x);
  res += snprintf(&text[res],max-res,"%sy: %d \n",indent,actual->y);

  return res;
}
