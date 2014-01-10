#include "krad_transponder_common_from_json.h"

int krad_link_av_mode_t_fr_json(char *json, void *st) {
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

int kr_txpdr_su_type_t_fr_json(char *json, void *st) {
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

int krad_link_transport_mode_t_fr_json(char *json, void *st) {
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

int kr_transponder_path_io_type_fr_json(char *json, void *st) {
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

int kr_transponder_info_fr_json(char *json, void *st) {
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  struct kr_transponder_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_transponder_info *)st;

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
  if (strncmp(&json[tokens[k].start],"active_paths",12)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -1;
  }

  json[tokens[k].end] = '\0';
  actual->active_paths = atoi(&json[tokens[k].start]);
  k++;

  res = k;

  return res;
}

int kr_transponder_path_io_path_info_fr_json(char *json, void *st) {
  uber_St uber;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  uber_St *uber_actual;

  kr_transponder_path_io_path_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  uber_actual = (uber_St *)st;

  if (uber_actual->actual == NULL) {
    return -1;
  }

  actual = (kr_transponder_path_io_path_info *)uber_actual->actual;

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
      uber.actual = &(actual->mixer_path_info);
      uber.type = DEJSON_KR_MIXER_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -1;
      }
      break;
    }
    case 1: {
      uber.actual = &(actual->compositor_path_info);
      uber.type = DEJSON_KR_COMPOSITOR_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -2;
      }
      break;
    }
    case 2: {
      uber.actual = &(actual->adapter_path_info);
      uber.type = DEJSON_KR_ADAPTER_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -3;
      }
      break;
    }
  }


  return res;
}

int kr_transponder_path_io_info_fr_json(char *json, void *st) {
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
  struct kr_transponder_path_io_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_transponder_path_io_info *)st;

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
  type = kr_strto_kr_transponder_path_io_type(&json[tokens[k].start]);
  if (type < 0) {
    return -1;
  }
  actual->type = type;
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"info",4)) {
    return -2;
  }

  k++;
  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -2;
  }

  index = kr_transponder_path_io_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = DEJSON_KR_TRANSPONDER_PATH_IO_PATH_INFO;
  json[tokens[k].end] = '\0';
  res += info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -2;
  }

  k += res;

  res = k;

  return res;
}

int kr_transponder_path_info_fr_json(char *json, void *st) {
  uber_St uber;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  struct kr_transponder_path_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_transponder_path_info *)st;

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
  if (strncmp(&json[tokens[k].start],"input",5)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -1;
  }

  uber.actual = &(actual->input);
  uber.type = DEJSON_KR_TRANSPONDER_PATH_IO_INFO;
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
  if (strncmp(&json[tokens[k].start],"output",6)) {
    return -2;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -2;
  }

  uber.actual = &(actual->output);
  uber.type = DEJSON_KR_TRANSPONDER_PATH_IO_INFO;
  json[tokens[k].end] = '\0';
  res = info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -2;
  }

  k += res;

  res = k;

  return res;
}

int kr_transponder_path_patch_fr_json(char *json, void *st) {
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  struct kr_transponder_path_patch *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_transponder_path_patch *)st;

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
  if (strncmp(&json[tokens[k].start],"coconut",7)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_PRIMITIVE) {
    return -1;
  }

  json[tokens[k].end] = '\0';
  actual->coconut = atoi(&json[tokens[k].start]);
  k++;

  res = k;

  return res;
}

