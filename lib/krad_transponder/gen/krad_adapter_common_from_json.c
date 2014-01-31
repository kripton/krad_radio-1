#include "krad_adapter_common_from_json.h"

int kr_adapter_path_direction_fr_json(char *json, void *st) {
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

int kr_adapter_api_fr_json(char *json, void *st) {
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

int kr_adapter_api_info_fr_json(char *json, void *st) {
  uber_St uber;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  uber_St *uber_actual;

  kr_adapter_api_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  uber_actual = (uber_St *)st;

  if (uber_actual->actual == NULL) {
    return -1;
  }

  actual = (kr_adapter_api_info *)uber_actual->actual;

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
      uber.actual = &(actual->jack);
      uber.type = DEJSON_KR_JACK_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -1;
      }
      break;
    }
    case 1: {
      uber.actual = &(actual->wayland);
      uber.type = DEJSON_KR_WAYLAND_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -2;
      }
      break;
    }
    case 2: {
      uber.actual = &(actual->v4l2);
      uber.type = DEJSON_KR_V4L2_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -3;
      }
      break;
    }
    case 3: {
      uber.actual = &(actual->decklink);
      uber.type = DEJSON_KR_DECKLINK_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -4;
      }
      break;
    }
    case 4: {
      uber.actual = &(actual->x11);
      uber.type = DEJSON_KR_X11_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -5;
      }
      break;
    }
    case 5: {
      uber.actual = &(actual->alsa);
      uber.type = DEJSON_KR_ALSA_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -6;
      }
      break;
    }
  }


  return res;
}

int kr_adapter_api_path_info_fr_json(char *json, void *st) {
  uber_St uber;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  uber_St *uber_actual;

  kr_adapter_api_path_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  uber_actual = (uber_St *)st;

  if (uber_actual->actual == NULL) {
    return -1;
  }

  actual = (kr_adapter_api_path_info *)uber_actual->actual;

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
      uber.actual = &(actual->jack);
      uber.type = DEJSON_KR_JACK_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -1;
      }
      break;
    }
    case 1: {
      uber.actual = &(actual->wayland);
      uber.type = DEJSON_KR_WAYLAND_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -2;
      }
      break;
    }
    case 2: {
      uber.actual = &(actual->v4l2);
      uber.type = DEJSON_KR_V4L2_OPEN_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -3;
      }
      break;
    }
    case 3: {
      uber.actual = &(actual->decklink);
      uber.type = DEJSON_KR_DECKLINK_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -4;
      }
      break;
    }
    case 4: {
      uber.actual = &(actual->x11);
      uber.type = DEJSON_KR_X11_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -5;
      }
      break;
    }
    case 5: {
      uber.actual = &(actual->alsa);
      uber.type = DEJSON_KR_ALSA_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -6;
      }
      break;
    }
  }


  return res;
}

int kr_adapter_info_fr_json(char *json, void *st) {
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
  struct kr_adapter_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_adapter_info *)st;

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
  if (strncmp(&json[tokens[k].start],"api",3)) {
    return -1;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -1;
  }
  json[tokens[k].end] = '\0';
  type = kr_strto_kr_adapter_api(&json[tokens[k].start]);
  if (type < 0) {
    return -1;
  }
  actual->api = type;
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"api_info",8)) {
    return -2;
  }

  k++;
  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -2;
  }

  index = kr_adapter_api_to_index(actual->api);
  uber_sub.type = index;
  uber_sub.actual = &(actual->api_info);
  uber.actual = &(uber_sub);
  uber.type = DEJSON_KR_ADAPTER_API_INFO;
  json[tokens[k].end] = '\0';
  res += info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -2;
  }

  k += res;

  res = k;

  return res;
}

int kr_adapter_path_info_fr_json(char *json, void *st) {
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
  struct kr_adapter_path_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_adapter_path_info *)st;

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
  if (strncmp(&json[tokens[k].start],"dir",3)) {
    return -2;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }
  json[tokens[k].end] = '\0';
  type = kr_strto_kr_adapter_path_direction(&json[tokens[k].start]);
  if (type < 0) {
    return -2;
  }
  actual->dir = type;
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -3;
  }
  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"api",3)) {
    return -3;
  }

  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -3;
  }
  json[tokens[k].end] = '\0';
  type = kr_strto_kr_adapter_api(&json[tokens[k].start]);
  if (type < 0) {
    return -3;
  }
  actual->api = type;
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -4;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"info",4)) {
    return -4;
  }

  k++;
  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -4;
  }

  index = kr_adapter_api_to_index(actual->api);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = DEJSON_KR_ADAPTER_API_PATH_INFO;
  json[tokens[k].end] = '\0';
  res += info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -4;
  }

  k += res;

  res = k;

  return res;
}

