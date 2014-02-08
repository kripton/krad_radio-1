#include "kr_aux_common_from_json.h"

int kr_aux_info_fr_json(char *json, void *st) {
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  struct kr_aux_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_aux_info *)st;
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
  if (strncmp(&json[tokens[k].start],"monkeyname",10)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -1;
  }

  json[tokens[k].end] = '\0';
  snprintf(actual->monkeyname, sizeof(actual->monkeyname), "%s", &json[tokens[k].start]);
  k++;

  res = k;

  return res;
}
int kr_aux_path_info_fr_json(char *json, void *st) {
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  struct kr_aux_path_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_aux_path_info *)st;
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
  if (strncmp(&json[tokens[k].start],"ncoconuts",9)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -1;
  }

  json[tokens[k].end] = '\0';
  actual->ncoconuts = atoi(&json[tokens[k].start]);
  k++;

  res = k;

  return res;
}
