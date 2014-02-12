#include "krad_wayland_common_to_text.h"

int kr_wayland_state_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_wayland_state *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_wayland_state *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_wayland_state(*actual));

  return res;
}
int kr_wayland_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
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
  res += snprintf(&text[res],max-res,"%sdisplay_name: %s \n",indent,actual->display_name);
  res += snprintf(&text[res],max-res,"%sstate:",indent);
  uber.actual = &(actual->state);
  uber.type = TEXT_KR_WAYLAND_STATE;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

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
  res += snprintf(&text[res],max-res,"%swidth: %d \n",indent,actual->width);
  res += snprintf(&text[res],max-res,"%sheight: %d \n",indent,actual->height);
  res += snprintf(&text[res],max-res,"%sfullscreen: %d \n",indent,actual->fullscreen);

  return res;
}
