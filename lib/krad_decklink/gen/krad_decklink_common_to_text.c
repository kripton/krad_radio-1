#include "krad_decklink_common_to_text.h"

int kr_decklink_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_decklink_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_decklink_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sdevice: %s \n",indent,actual->device);

  return res;
}
int kr_decklink_path_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_decklink_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_decklink_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sdevice: %s \n",indent,actual->device);
  res += snprintf(&text[res],max-res,"%swidth: %u \n",indent,actual->width);
  res += snprintf(&text[res],max-res,"%sheight: %u \n",indent,actual->height);
  res += snprintf(&text[res],max-res,"%snum: %u \n",indent,actual->num);
  res += snprintf(&text[res],max-res,"%sden: %u \n",indent,actual->den);
  res += snprintf(&text[res],max-res,"%svideo_connector: %s \n",indent,actual->video_connector);
  res += snprintf(&text[res],max-res,"%saudio_connector: %s \n",indent,actual->audio_connector);

  return res;
}
