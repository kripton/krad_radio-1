#include "krad_app_helpers.h"

int kr_radio_payload_type_to_index(int val) {
  switch (val) {
    case PL_KR_TRANSPONDER_PATH_INFO:
      return 0;
    case PL_KR_MIXER_PATH_INFO:
      return 1;
  }
  return -1;
}

char *kr_strfr_kr_radio_payload_type(int val) {
  switch (val) {
    case PL_KR_TRANSPONDER_PATH_INFO:
      return "pl_kr_transponder_path_info";
    case PL_KR_MIXER_PATH_INFO:
      return "pl_kr_mixer_path_info";
  }
  return NULL;
}

int kr_strto_kr_radio_payload_type(char *string) {
  if (!strcmp(string,"pl_kr_transponder_path_info")) {
    return PL_KR_TRANSPONDER_PATH_INFO;
  }
  if (!strcmp(string,"pl_kr_mixer_path_info")) {
    return PL_KR_MIXER_PATH_INFO;
  }

  return -1;
}

int kr_app_method_to_index(int val) {
  switch (val) {
    case KR_GET:
      return 0;
    case KR_PUT:
      return 1;
    case KR_POST:
      return 2;
    case KR_PATCH:
      return 3;
    case KR_DELETE:
      return 4;
  }
  return -1;
}

char *kr_strfr_kr_app_method(int val) {
  switch (val) {
    case KR_GET:
      return "kr_get";
    case KR_PUT:
      return "kr_put";
    case KR_POST:
      return "kr_post";
    case KR_PATCH:
      return "kr_patch";
    case KR_DELETE:
      return "kr_delete";
  }
  return NULL;
}

int kr_strto_kr_app_method(char *string) {
  if (!strcmp(string,"kr_get")) {
    return KR_GET;
  }
  if (!strcmp(string,"kr_put")) {
    return KR_PUT;
  }
  if (!strcmp(string,"kr_post")) {
    return KR_POST;
  }
  if (!strcmp(string,"kr_patch")) {
    return KR_PATCH;
  }
  if (!strcmp(string,"kr_delete")) {
    return KR_DELETE;
  }

  return -1;
}

int kr_radio_payload_init(void *st, int idx) {
  kr_radio_payload *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (kr_radio_payload *)st;
  memset(actual, 0, sizeof(kr_radio_payload));
  switch (idx) {
    case 0: {
      kr_transponder_path_info_init(&actual->transponder_path_info);
      break;
    }
    case 1: {
      kr_mixer_path_info_init(&actual->mixer_path_info);
      break;
    }
  }


  return -1;
}

int kr_radio_payload_valid(void *st, int idx) {
  kr_radio_payload *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (kr_radio_payload *)st;
  switch (idx) {
    case 0: {
      kr_transponder_path_info_valid(&actual->transponder_path_info);
      break;
    }
    case 1: {
      kr_mixer_path_info_valid(&actual->mixer_path_info);
      break;
    }
  }


  return -1;
}

int kr_radio_payload_random(void *st, int idx) {
  kr_radio_payload *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (kr_radio_payload *)st;
  memset(st, 0, sizeof(kr_radio_payload));
  switch (idx) {
    case 0: {
      kr_transponder_path_info_random(&actual->transponder_path_info);
      break;
    }
    case 1: {
      kr_mixer_path_info_random(&actual->mixer_path_info);
      break;
    }
  }


  return -1;
}

int kr_crate2_init(void *st) {
  struct kr_crate2 *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_crate2 *)st;
  memset(actual, 0, sizeof(struct kr_crate2));
  for (i = 0; i < 64; i++) {
  }
  kr_radio_payload_init(&actual->payload,kr_radio_payload_type_to_index(actual->payload_type));

  return 0;
}

int kr_crate2_valid(void *st) {
  struct kr_crate2 *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_crate2 *)st;
  for (i = 0; i < 64; i++) {
    if (!actual->address[i]) {
      break;
    }
    if (i == 63 && actual->address[i]) {
      return -2;
    }
  }
  kr_radio_payload_valid(&actual->payload,kr_radio_payload_type_to_index(actual->payload_type));

  return 0;
}

int kr_crate2_random(void *st) {
  struct kr_crate2 *actual;

  int i;
  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_crate2 *)st;
  memset(st, 0, sizeof(struct kr_crate2));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    actual->address[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      actual->address[63] = '\0';
    }
  }
  kr_radio_payload_random(&actual->payload,kr_radio_payload_type_to_index(actual->payload_type));

  return 0;
}

