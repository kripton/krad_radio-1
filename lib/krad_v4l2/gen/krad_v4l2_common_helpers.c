#include "krad_v4l2_common_helpers.h"

kr_v4l2_info_member kr_v4l2_info_strto_member(char *string, int len) {
  if (!strncmp(string,"kr_v4l2_info_dev",len)) {
    return KR_V4L2_INFO_DEV;
  }
  if (!strncmp(string,"kr_v4l2_info_priority",len)) {
    return KR_V4L2_INFO_PRIORITY;
  }
  if (!strncmp(string,"kr_v4l2_info_state",len)) {
    return KR_V4L2_INFO_STATE;
  }
  if (!strncmp(string,"kr_v4l2_info_mode",len)) {
    return KR_V4L2_INFO_MODE;
  }
  return -1;
}

kr_v4l2_open_info_member kr_v4l2_open_info_strto_member(char *string, int len) {
  if (!strncmp(string,"kr_v4l2_open_info_dev",len)) {
    return KR_V4L2_OPEN_INFO_DEV;
  }
  if (!strncmp(string,"kr_v4l2_open_info_priority",len)) {
    return KR_V4L2_OPEN_INFO_PRIORITY;
  }
  if (!strncmp(string,"kr_v4l2_open_info_mode",len)) {
    return KR_V4L2_OPEN_INFO_MODE;
  }
  return -1;
}

int kr_v4l2_state_to_index(int val) {
  switch (val) {
    case KR_V4L2_VOID:
      return 0;
    case KR_V4L2_OPEN:
      return 1;
    case KR_V4L2_CAPTURE:
      return 2;
  }
  return -1;
}

char *kr_strfr_kr_v4l2_state(int val) {
  switch (val) {
    case KR_V4L2_VOID:
      return "kr_v4l2_void";
    case KR_V4L2_OPEN:
      return "kr_v4l2_open";
    case KR_V4L2_CAPTURE:
      return "kr_v4l2_capture";
  }
  return NULL;
}

int kr_strto_kr_v4l2_state(char *string) {
  if (!strcmp(string,"kr_v4l2_void")) {
    return KR_V4L2_VOID;
  }
  if (!strcmp(string,"kr_v4l2_open")) {
    return KR_V4L2_OPEN;
  }
  if (!strcmp(string,"kr_v4l2_capture")) {
    return KR_V4L2_CAPTURE;
  }
  return -1;
}

int kr_v4l2_info_patch_apply(struct kr_v4l2_info *info, kr_v4l2_info_patch *patch) {
  const ptrdiff_t off[4] = { offsetof(struct kr_v4l2_info, dev), 
    offsetof(struct kr_v4l2_info, priority), offsetof(struct kr_v4l2_info, state), 
    offsetof(struct kr_v4l2_info, mode)
  };
  const size_t sz[4] = { sizeof(info->dev), 
    sizeof(info->priority), sizeof(info->state), 
    sizeof(info->mode)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_v4l2_open_info_patch_apply(struct kr_v4l2_open_info *info, kr_v4l2_open_info_patch *patch) {
  const ptrdiff_t off[3] = { offsetof(struct kr_v4l2_open_info, dev), 
    offsetof(struct kr_v4l2_open_info, priority), offsetof(struct kr_v4l2_open_info, mode)
  };
  const size_t sz[3] = { sizeof(info->dev), 
    sizeof(info->priority), sizeof(info->mode)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_v4l2_mode_init(struct kr_v4l2_mode *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_v4l2_mode_valid(struct kr_v4l2_mode *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_v4l2_mode_random(struct kr_v4l2_mode *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_v4l2_info_init(struct kr_v4l2_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_v4l2_info));
  kr_v4l2_mode_init(&st->mode);

  return 0;
}

int kr_v4l2_info_valid(struct kr_v4l2_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_v4l2_mode_valid(&st->mode);

  return 0;
}

int kr_v4l2_info_random(struct kr_v4l2_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_v4l2_info));
  kr_v4l2_mode_random(&st->mode);

  return 0;
}

int kr_v4l2_open_info_init(struct kr_v4l2_open_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_v4l2_open_info));
  kr_v4l2_mode_init(&st->mode);

  return 0;
}

int kr_v4l2_open_info_valid(struct kr_v4l2_open_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_v4l2_mode_valid(&st->mode);

  return 0;
}

int kr_v4l2_open_info_random(struct kr_v4l2_open_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_v4l2_open_info));
  kr_v4l2_mode_random(&st->mode);

  return 0;
}

