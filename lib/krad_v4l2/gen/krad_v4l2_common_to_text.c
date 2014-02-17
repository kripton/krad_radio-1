#include "krad_v4l2_common_to_text.h"

int kr_v4l2_state_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_v4l2_state *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_v4l2_state *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_v4l2_state(*actual));

  return res;
}
int kr_v4l2_path_info_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_v4l2_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_v4l2_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%swidth: %d \n",indent,actual->width);
  res += snprintf(&text[res],max-res,"%sheight: %d \n",indent,actual->height);
  res += snprintf(&text[res],max-res,"%snum: %d \n",indent,actual->num);
  res += snprintf(&text[res],max-res,"%sden: %d \n",indent,actual->den);
  res += snprintf(&text[res],max-res,"%sformat: %d \n",indent,actual->format);

  return res;
}
int kr_v4l2_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  kr_v4l2_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_v4l2_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sdev: %d \n",indent,actual->dev);
  res += snprintf(&text[res],max-res,"%spriority: %d \n",indent,actual->priority);
  res += snprintf(&text[res],max-res,"%sstate:",indent);
  uber.actual = &(actual->state);
  uber.type = TEXT_KR_V4L2_STATE;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
