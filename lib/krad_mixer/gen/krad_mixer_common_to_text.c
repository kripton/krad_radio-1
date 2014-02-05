#include "krad_mixer_common_to_text.h"

int kr_mixer_channels_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_mixer_channels *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_mixer_channels *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_mixer_channels(*actual));

  return res;
}
int kr_mixer_path_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_mixer_path_type *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_mixer_path_type *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_mixer_path_type(*actual));

  return res;
}
int kr_mixer_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_mixer_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%stype:",indent);
  uber.actual = &(actual->type);
  uber.type = TEXT_KR_MIXER_PATH_TYPE;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%schannels:",indent);
  uber.actual = &(actual->channels);
  uber.type = TEXT_KR_MIXER_CHANNELS;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%slowpass:\n",indent);
  uber.actual = &(actual->lowpass);
  uber.type = TEXT_KR_LOWPASS_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%shighpass:\n",indent);
  uber.actual = &(actual->highpass);
  uber.type = TEXT_KR_HIGHPASS_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%sanalog:\n",indent);
  uber.actual = &(actual->analog);
  uber.type = TEXT_KR_ANALOG_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%seq:\n",indent);
  uber.actual = &(actual->eq);
  uber.type = TEXT_KR_EQ_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%svolume:\n",indent);
  uber.actual = &(actual->volume);
  uber.type = TEXT_KR_VOLUME_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
