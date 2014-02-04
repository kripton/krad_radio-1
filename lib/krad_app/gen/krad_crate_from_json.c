#include "krad_crate_from_json.h"

int kr_method_fr_json(char *json, void *st) {
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

int kr_crate_fr_json(char *json, void *st) {
  uber_St uber;
  int type;
  uber_St uber_sub;
  int index;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  struct kr_crate *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_crate *)st;

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
  if (strncmp(&json[tokens[k].start],"address",7)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -1;
  }

  json[tokens[k].end] = '\0';
  snprintf(actual->address, sizeof(actual->address), "%s", &json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"method",6)) {
    return -2;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }
  json[tokens[k].end] = '\0';
  type = kr_strto_kr_method(&json[tokens[k].start]);
  if (type < 0) {
    return -2;
  }
  actual->method = type;
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -3;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"payload_type",12)) {
    return -3;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -3;
  }
  json[tokens[k].end] = '\0';
  type = kr_strto_kr_payload_type(&json[tokens[k].start]);
  if (type < 0) {
    return -3;
  }
  actual->payload_type = type;
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -4;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"payload",7)) {
    return -4;
  }

  k++;
  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -4;
  }

  index = kr_payload_type_to_index(actual->payload_type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->payload);
  uber.actual = &(uber_sub);
  uber.type = DEJSON_KR_PAYLOAD;
  json[tokens[k].end] = '\0';
  res += info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -4;
  }

  k += res;

  res = k;

  return res;
}

