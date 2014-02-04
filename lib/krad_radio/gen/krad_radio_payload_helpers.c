#include "krad_radio_payload_helpers.h"

int kr_payload_type_to_index(int val) {
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

char *kr_strfr_kr_payload_type(int val) {
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

int kr_strto_kr_payload_type(char *string) {
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

int kr_nil_init(struct kr_nil *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_nil_valid(struct kr_nil *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_nil_random(struct kr_nil *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_payload_init(kr_payload *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_payload));
  switch (idx) {
    case 0: {
      kr_nil_init(&st->nil);
      break;
    }
    case 1: {
      kr_transponder_path_info_init(&st->transponder_path_info);
      break;
    }
    case 2: {
      kr_mixer_path_info_init(&st->mixer_path_info);
      break;
    }
    case 3: {
      kr_compositor_path_info_init(&st->compositor_path_info);
      break;
    }
  }


  return -1;
}

int kr_payload_valid(kr_payload *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  switch (idx) {
    case 0: {
      kr_nil_valid(&st->nil);
      break;
    }
    case 1: {
      kr_transponder_path_info_valid(&st->transponder_path_info);
      break;
    }
    case 2: {
      kr_mixer_path_info_valid(&st->mixer_path_info);
      break;
    }
    case 3: {
      kr_compositor_path_info_valid(&st->compositor_path_info);
      break;
    }
  }


  return -1;
}

int kr_payload_random(kr_payload *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_payload));
  switch (idx) {
    case 0: {
      kr_nil_random(&st->nil);
      break;
    }
    case 1: {
      kr_transponder_path_info_random(&st->transponder_path_info);
      break;
    }
    case 2: {
      kr_mixer_path_info_random(&st->mixer_path_info);
      break;
    }
    case 3: {
      kr_compositor_path_info_random(&st->compositor_path_info);
      break;
    }
  }


  return -1;
}

