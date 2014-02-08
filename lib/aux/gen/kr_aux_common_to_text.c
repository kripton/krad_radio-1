#include "kr_aux_common_to_text.h"

int kr_aux_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_aux_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_aux_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%smonkeyname: %s \n",indent,actual->monkeyname);

  return res;
}
int kr_aux_path_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_aux_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_aux_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sncoconuts: %d \n",indent,actual->ncoconuts);

  return res;
}
