#include "kr_xpdr_common_from_json.h"

int kr_xpdr_type_fr_json(char *json, void *st) {
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
int kr_xpdr_type_info_fr_json(char *json, void *st) {
  uber_St uber;
  int res;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  int k;
  uber_St *uber_actual;
  kr_xpdr_type_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_xpdr_type_info *)uber_actual->actual;
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
      uber.actual = &(actual->wl);
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
      uber.actual = &(actual->dl);
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
    case 6: {
      uber.actual = &(actual->aux);
      uber.type = DEJSON_KR_AUX_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -7;
      }
      break;
    }
    case 7: {
      uber.actual = &(actual->jack_in);
      uber.type = DEJSON_KR_JACK_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -8;
      }
      break;
    }
    case 8: {
      uber.actual = &(actual->jack_out);
      uber.type = DEJSON_KR_JACK_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -9;
      }
      break;
    }
    case 9: {
      uber.actual = &(actual->wl_out);
      uber.type = DEJSON_KR_WAYLAND_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -10;
      }
      break;
    }
    case 10: {
      uber.actual = &(actual->v4l2_in);
      uber.type = DEJSON_KR_V4L2_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -11;
      }
      break;
    }
    case 11: {
      uber.actual = &(actual->dl_in);
      uber.type = DEJSON_KR_DECKLINK_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -12;
      }
      break;
    }
    case 12: {
      uber.actual = &(actual->x11_in);
      uber.type = DEJSON_KR_X11_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -13;
      }
      break;
    }
    case 13: {
      uber.actual = &(actual->alsa_in);
      uber.type = DEJSON_KR_ALSA_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -14;
      }
      break;
    }
    case 14: {
      uber.actual = &(actual->alsa_out);
      uber.type = DEJSON_KR_ALSA_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -15;
      }
      break;
    }
    case 15: {
      uber.actual = &(actual->aux_in);
      uber.type = DEJSON_KR_AUX_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -16;
      }
      break;
    }
    case 16: {
      uber.actual = &(actual->aux_out);
      uber.type = DEJSON_KR_AUX_PATH_INFO;
      json[tokens[k].end] = '\0';
      res += info_unpack_fr_json(&json[tokens[k].start],&uber);
      if (res < 0) {
        return -17;
      }
      break;
    }
  }


  return res;
}
int kr_xpdr_path_info_fr_json(char *json, void *st) {
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
  struct kr_xpdr_path_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_xpdr_path_info *)st;
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
  type = kr_strto_kr_xpdr_type(&json[tokens[k].start]);
  if (type < 0) {
    return -1;
  }
  actual->type = type;
  k++;

  if (ntokens > k && tokens[k].type != JSMN_STRING) {
    return -2;
  }

  json[tokens[k].end] = '\0';
  if (strncmp(&json[tokens[k].start],"adp",3)) {
    return -2;
  }

  k++;
  if (ntokens > k && tokens[k].type != JSMN_OBJECT) {
    return -2;
  }

  index = kr_xpdr_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->adp);
  uber.actual = &(uber_sub);
  uber.type = DEJSON_KR_XPDR_TYPE_INFO;
  json[tokens[k].end] = '\0';
  res += info_unpack_fr_json(&json[tokens[k].start],&uber);
  if (res < 0) {
    return -2;
  }

  k += res;

  res = k;

  return res;
}
