#include "krad_perspective_common_from_json.h"

int kr_pos_fr_json(char *json, void *st) {
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  struct kr_pos *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_pos *)st;

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
  if (strncmp(&json[tokens[k].start],"x",1)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -1;
  }

  json[tokens[k].end] = '\0';
  actual->x = atoi(&json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"y",1)) {
    return -2;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -2;
  }

  json[tokens[k].end] = '\0';
  actual->y = atoi(&json[tokens[k].start]);
  k++;

  res = k;

  return res;
}

int kr_perspective_view_fr_json(char *json, void *st) {
  uber_St uber;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  struct kr_perspective_view *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_perspective_view *)st;

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
  if (strncmp(&json[tokens[k].start],"top_left",8)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -1;
  }

  uber.actual = &(actual->top_left);
  uber.type = DEJSON_KR_POS;
  json[tokens[k].end] = '\0';
  res = info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -1;
  }

  k += res;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"top_right",9)) {
    return -2;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -2;
  }

  uber.actual = &(actual->top_right);
  uber.type = DEJSON_KR_POS;
  json[tokens[k].end] = '\0';
  res = info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -2;
  }

  k += res;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -3;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"bottom_left",11)) {
    return -3;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -3;
  }

  uber.actual = &(actual->bottom_left);
  uber.type = DEJSON_KR_POS;
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
  if (strncmp(&json[tokens[k].start],"bottom_right",12)) {
    return -4;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -4;
  }

  uber.actual = &(actual->bottom_right);
  uber.type = DEJSON_KR_POS;
  json[tokens[k].end] = '\0';
  res = info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -4;
  }

  k += res;

  res = k;

  return res;
}

