#include "krad_jack_common_from_json.h"

int kr_jack_direction_fr_json(char *json, void *st) {
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

int kr_jack_state_fr_json(char *json, void *st) {
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

int kr_jack_setup_info_fr_json(char *json, void *st) {
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  kr_jack_setup_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_jack_setup_info *)st;

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
  if (strncmp(&json[tokens[k].start],"client_name",11)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -1;
  }

  json[tokens[k].end] = '\0';
  snprintf(actual->client_name, sizeof(actual->client_name), "%s", &json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"server_name",11)) {
    return -2;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }

  json[tokens[k].end] = '\0';
  snprintf(actual->server_name, sizeof(actual->server_name), "%s", &json[tokens[k].start]);
  k++;

  res = k;

  return res;
}

int kr_jack_info_fr_json(char *json, void *st) {
  int type;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  kr_jack_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_jack_info *)st;

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
  if (strncmp(&json[tokens[k].start],"client_name",11)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -1;
  }

  json[tokens[k].end] = '\0';
  snprintf(actual->client_name, sizeof(actual->client_name), "%s", &json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"server_name",11)) {
    return -2;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }

  json[tokens[k].end] = '\0';
  snprintf(actual->server_name, sizeof(actual->server_name), "%s", &json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -3;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"state",5)) {
    return -3;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -3;
  }
  json[tokens[k].end] = '\0';
  type = kr_strto_kr_jack_state(&json[tokens[k].start]);
  if (type < 0) {
    return -3;
  }
  actual->state = type;
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -4;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"inputs",6)) {
    return -4;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -4;
  }

  json[tokens[k].end] = '\0';
  actual->inputs = atoi(&json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -5;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"outputs",7)) {
    return -5;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -5;
  }

  json[tokens[k].end] = '\0';
  actual->outputs = atoi(&json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -6;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"sample_rate",11)) {
    return -6;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -6;
  }

  json[tokens[k].end] = '\0';
  actual->sample_rate = atoi(&json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -7;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"period_size",11)) {
    return -7;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -7;
  }

  json[tokens[k].end] = '\0';
  actual->period_size = atoi(&json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -8;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"xruns",5)) {
    return -8;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -8;
  }

  json[tokens[k].end] = '\0';
  actual->xruns = atoi(&json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -9;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"frames",6)) {
    return -9;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -9;
  }

  json[tokens[k].end] = '\0';
  actual->frames = atoi(&json[tokens[k].start]);
  k++;

  res = k;

  return res;
}

int kr_jack_path_info_fr_json(char *json, void *st) {
  int type;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  kr_jack_path_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_jack_path_info *)st;

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
  if (strncmp(&json[tokens[k].start],"name",4)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -1;
  }

  json[tokens[k].end] = '\0';
  snprintf(actual->name, sizeof(actual->name), "%s", &json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"channels",8)) {
    return -2;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -2;
  }

  json[tokens[k].end] = '\0';
  actual->channels = atoi(&json[tokens[k].start]);
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -3;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"direction",9)) {
    return -3;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -3;
  }
  json[tokens[k].end] = '\0';
  type = kr_strto_kr_jack_direction(&json[tokens[k].start]);
  if (type < 0) {
    return -3;
  }
  actual->direction = type;
  k++;

  res = k;

  return res;
}

