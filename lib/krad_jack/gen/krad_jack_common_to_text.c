#include "krad_jack_common_to_text.h"

int kr_jack_direction_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_jack_direction *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_jack_direction *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_jack_direction(*actual));

  return res;
}
int kr_jack_state_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_jack_state *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_jack_state *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_jack_state(*actual));

  return res;
}
int kr_jack_setup_info_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_jack_setup_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_jack_setup_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sclient_name: %s \n",indent,actual->client_name);
  res += snprintf(&text[res],max-res,"%sserver_name: %s \n",indent,actual->server_name);

  return res;
}
int kr_jack_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  kr_jack_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_jack_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sclient_name: %s \n",indent,actual->client_name);
  res += snprintf(&text[res],max-res,"%sserver_name: %s \n",indent,actual->server_name);
  res += snprintf(&text[res],max-res,"%sstate:",indent);
  uber.actual = &(actual->state);
  uber.type = TEXT_KR_JACK_STATE;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%sinputs: %u \n",indent,actual->inputs);
  res += snprintf(&text[res],max-res,"%soutputs: %u \n",indent,actual->outputs);
  res += snprintf(&text[res],max-res,"%ssample_rate: %u \n",indent,actual->sample_rate);
  res += snprintf(&text[res],max-res,"%speriod_size: %u \n",indent,actual->period_size);
  res += snprintf(&text[res],max-res,"%sxruns: %u \n",indent,actual->xruns);
  res += snprintf(&text[res],max-res,"%sframes: %ju \n",indent,actual->frames);

  return res;
}
int kr_jack_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  kr_jack_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_jack_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sname: %s \n",indent,actual->name);
  res += snprintf(&text[res],max-res,"%schannels: %d \n",indent,actual->channels);
  res += snprintf(&text[res],max-res,"%sdirection:",indent);
  uber.actual = &(actual->direction);
  uber.type = TEXT_KR_JACK_DIRECTION;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
