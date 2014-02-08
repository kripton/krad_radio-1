#include "krad_radio_payload_from_json.h"

int kr_nil_fr_json(char *json, void *st) {
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  struct kr_nil *actual;
  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_nil *)st;
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
  if (strncmp(&json[tokens[k].start],"n",1)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -1;
  }

  json[tokens[k].end] = '\0';
  actual->n = atoi(&json[tokens[k].start]);
  k++;

  res = k;

  return res;
}
int kr_payload_type_fr_json(char *json, void *st) {
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
int kr_payload_fr_json(char *json, void *st) {
  uber_St uber;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  uber_St *uber_actual;
  kr_payload *actual;
  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_payload *)uber_actual->actual;
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

    switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->nil);
      uber.type = DEJSON_KR_NIL;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -1;
      }
      break;
    }
    case 1: {
      uber.actual = &(actual->xpdr_path_info);
      uber.type = DEJSON_KR_XPDR_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -2;
      }
      break;
    }
    case 2: {
      uber.actual = &(actual->mixer_path_info);
      uber.type = DEJSON_KR_MIXER_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -3;
      }
      break;
    }
    case 3: {
      uber.actual = &(actual->compositor_path_info);
      uber.type = DEJSON_KR_COMPOSITOR_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -4;
      }
      break;
    }
  }


  return res;
}
