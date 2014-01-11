#include "krad_radio_payload_helpers.h"

int kr_radio_payload_type_to_index(int val) {
  switch (val) {
    case PL_KR_NIL:
      return 0;
    case PL_KR_TRANSPONDER_PATH_INFO:
      return 1;
    case PL_KR_MIXER_PATH_INFO:
      return 2;
    case PL_KR_COMPOSITOR_PATH_INFO:
      return 3;
  }
  return -1;
}

char *kr_strfr_kr_radio_payload_type(int val) {
  switch (val) {
    case PL_KR_NIL:
      return "pl_kr_nil";
    case PL_KR_TRANSPONDER_PATH_INFO:
      return "pl_kr_transponder_path_info";
    case PL_KR_MIXER_PATH_INFO:
      return "pl_kr_mixer_path_info";
    case PL_KR_COMPOSITOR_PATH_INFO:
      return "pl_kr_compositor_path_info";
  }
  return NULL;
}

int kr_strto_kr_radio_payload_type(char *string) {
  if (!strcmp(string,"pl_kr_nil")) {
    return PL_KR_NIL;
  }
  if (!strcmp(string,"pl_kr_transponder_path_info")) {
    return PL_KR_TRANSPONDER_PATH_INFO;
  }
  if (!strcmp(string,"pl_kr_mixer_path_info")) {
    return PL_KR_MIXER_PATH_INFO;
  }
  if (!strcmp(string,"pl_kr_compositor_path_info")) {
    return PL_KR_COMPOSITOR_PATH_INFO;
  }

  return -1;
}

int kr_nil_init(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_nil_valid(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_nil_random(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
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
      kr_nil_init(&actual->nil);
      break;
    }
    case 1: {
      kr_transponder_path_info_init(&actual->transponder_path_info);
      break;
    }
    case 2: {
      kr_mixer_path_info_init(&actual->mixer_path_info);
      break;
    }
    case 3: {
      kr_compositor_path_info_init(&actual->compositor_path_info);
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
      kr_nil_valid(&actual->nil);
      break;
    }
    case 1: {
      kr_transponder_path_info_valid(&actual->transponder_path_info);
      break;
    }
    case 2: {
      kr_mixer_path_info_valid(&actual->mixer_path_info);
      break;
    }
    case 3: {
      kr_compositor_path_info_valid(&actual->compositor_path_info);
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
  memset(actual, 0, sizeof(kr_radio_payload));
  switch (idx) {
    case 0: {
      kr_nil_random(&actual->nil);
      break;
    }
    case 1: {
      kr_transponder_path_info_random(&actual->transponder_path_info);
      break;
    }
    case 2: {
      kr_mixer_path_info_random(&actual->mixer_path_info);
      break;
    }
    case 3: {
      kr_compositor_path_info_random(&actual->compositor_path_info);
      break;
    }
  }


  return -1;
}

