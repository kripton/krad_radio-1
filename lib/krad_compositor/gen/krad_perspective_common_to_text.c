#include "krad_perspective_common_to_text.h"

int kr_pos_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_pos *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_pos *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sx: %u \n",indent,actual->x);
  res += snprintf(&text[res],max-res,"%sy: %u \n",indent,actual->y);

  return res;
}
int kr_perspective_view_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_perspective_view *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_perspective_view *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%stop_left:\n",indent);
  uber.actual = &(actual->top_left);
  uber.type = TEXT_KR_POS;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%stop_right:\n",indent);
  uber.actual = &(actual->top_right);
  uber.type = TEXT_KR_POS;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%sbottom_left:\n",indent);
  uber.actual = &(actual->bottom_left);
  uber.type = TEXT_KR_POS;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%sbottom_right:\n",indent);
  uber.actual = &(actual->bottom_right);
  uber.type = TEXT_KR_POS;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
