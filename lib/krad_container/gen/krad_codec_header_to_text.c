#include "krad_codec_header_to_text.h"

int krad_codec_t_to_text(char *text, void *st, int32_t max) {
  int res;
  krad_codec_t *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (krad_codec_t *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_krad_codec_t(*actual));

  return res;
}
int krad_container_type_t_to_text(char *text, void *st, int32_t max) {
  int res;
  krad_container_type_t *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (krad_container_type_t *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_krad_container_type_t(*actual));

  return res;
}
