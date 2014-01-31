#include "krad_mixer_common_from_json.h"

int kr_mixer_channels_fr_json(char *json, void *st) {
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  jsmn_init(&parser);
  err = jsmn_parse(&parser,json,tokens,512);
  ntokens = parser.toknext;

  k = 0;

  if (err != JSMN_SUCCESS || ntokens < 3) {
    return -1;
  }

  if (tokens[k].type != JSMN_OBJECT) {
    return -1;
  }

  k++;


  return res;
}

int kr_mixer_path_type_fr_json(char *json, void *st) {
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  jsmn_init(&parser);
  err = jsmn_parse(&parser,json,tokens,512);
  ntokens = parser.toknext;

  k = 0;

  if (err != JSMN_SUCCESS || ntokens < 3) {
    return -1;
  }

  if (tokens[k].type != JSMN_OBJECT) {
    return -1;
  }

  k++;


  return res;
}

int kr_mixer_path_info_fr_json(char *json, void *st) {
  uber_St uber;
  int type;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  struct kr_mixer_path_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_mixer_path_info *)st;

  jsmn_init(&parser);
  err = jsmn_parse(&parser,json,tokens,512);
  ntokens = parser.toknext;

  k = 0;

  if (err != JSMN_SUCCESS || ntokens < 3) {
    return -1;
  }

  if (tokens[k].type != JSMN_OBJECT) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -1;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"type",4)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -1;
  }
  json[tokens[k].end] = '\0';
  type = kr_strto_kr_mixer_path_type(&json[tokens[k].start]);
  if (type < 0) {
    return -1;
  }
  actual->type = type;
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"channels",8)) {
    return -2;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }
  json[tokens[k].end] = '\0';
  type = kr_strto_kr_mixer_channels(&json[tokens[k].start]);
  if (type < 0) {
    return -2;
  }
  actual->channels = type;
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -3;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"lowpass",7)) {
    return -3;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -3;
  }

  uber.actual = &(actual->lowpass);
  uber.type = DEJSON_KR_LOWPASS_INFO;
  json[tokens[k].end] = '\0';
  res = info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -3;
  }

  k += res;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -4;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"highpass",8)) {
    return -4;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -4;
  }

  uber.actual = &(actual->highpass);
  uber.type = DEJSON_KR_HIGHPASS_INFO;
  json[tokens[k].end] = '\0';
  res = info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -4;
  }

  k += res;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -5;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"analog",6)) {
    return -5;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -5;
  }

  uber.actual = &(actual->analog);
  uber.type = DEJSON_KR_ANALOG_INFO;
  json[tokens[k].end] = '\0';
  res = info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -5;
  }

  k += res;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -6;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"eq",2)) {
    return -6;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -6;
  }

  uber.actual = &(actual->eq);
  uber.type = DEJSON_KR_EQ_INFO;
  json[tokens[k].end] = '\0';
  res = info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -6;
  }

  k += res;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -7;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"volume",6)) {
    return -7;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -7;
  }

  uber.actual = &(actual->volume);
  uber.type = DEJSON_KR_VOLUME_INFO;
  json[tokens[k].end] = '\0';
  res = info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -7;
  }

  k += res;

  res = k;

  return res;
}

