#include "krad_alsa_common_to_text.h"

int kr_alsa_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_alsa_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_alsa_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%scard: %d \n",indent,actual->card);
  res += snprintf(&text[res],max-res,"%sname: %s \n",indent,actual->name);

  return res;
}
int kr_alsa_path_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_alsa_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_alsa_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%scard_num: %d \n",indent,actual->card_num);

  return res;
}
