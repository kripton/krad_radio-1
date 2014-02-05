#include "krad_wayland_common_to_text.h"

int kr_wayland_info_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_wayland_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_wayland_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sstate: %d \n",indent,actual->state);
  res += snprintf(&text[res],max-res,"%sdisplay_name: %s \n",indent,actual->display_name);

  return res;
}
int kr_wayland_path_info_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_wayland_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_wayland_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sdisplay_name: %s \n",indent,actual->display_name);
  res += snprintf(&text[res],max-res,"%sstate: %d \n",indent,actual->state);
  res += snprintf(&text[res],max-res,"%swidth: %d \n",indent,actual->width);
  res += snprintf(&text[res],max-res,"%sheight: %d \n",indent,actual->height);
  res += snprintf(&text[res],max-res,"%sfullscreen: %d \n",indent,actual->fullscreen);

  return res;
}
