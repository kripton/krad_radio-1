#include "krad_sfx_common_to_text.h"

int kr_sfx_control_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_sfx_control *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_sfx_control *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_sfx_control(*actual));

  return res;
}
int kr_sfx_effect_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_sfx_effect_type *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_sfx_effect_type *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_sfx_effect_type(*actual));

  return res;
}
int kr_sfx_effect_control_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_sfx_effect_control *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_sfx_effect_control *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_sfx_effect_control(*actual));

  return res;
}
int kr_eq_band_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_eq_band_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_eq_band_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sdb: %0.2f \n",indent,actual->db);
  res += snprintf(&text[res],max-res,"%sbw: %0.2f \n",indent,actual->bw);
  res += snprintf(&text[res],max-res,"%shz: %0.2f \n",indent,actual->hz);

  return res;
}
int kr_eq_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int i;
  int res;
  struct kr_eq_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_eq_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sband:\n",indent);
  for (i = 0; i < KR_EQ_MAX_BANDS; i++) {
    uber.actual = &(actual->band[i]);
    uber.type = TEXT_KR_EQ_BAND_INFO;
  depth_state++;
    res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  }

  return res;
}
int kr_lowpass_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_lowpass_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_lowpass_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sbw: %0.2f \n",indent,actual->bw);
  res += snprintf(&text[res],max-res,"%shz: %0.2f \n",indent,actual->hz);

  return res;
}
int kr_highpass_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_highpass_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_highpass_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sbw: %0.2f \n",indent,actual->bw);
  res += snprintf(&text[res],max-res,"%shz: %0.2f \n",indent,actual->hz);

  return res;
}
int kr_volume_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_volume_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_volume_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%slevel: %0.2f \n",indent,actual->level);

  return res;
}
int kr_analog_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_analog_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_analog_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sdrive: %0.2f \n",indent,actual->drive);
  res += snprintf(&text[res],max-res,"%sblend: %0.2f \n",indent,actual->blend);

  return res;
}
