#include "krad_v4l2_common_helpers.h"

kr_v4l2_path_info_member kr_v4l2_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"width",len)) {
    return KR_V4L2_PATH_INFO_WIDTH;
  }
  if (!strncmp(string,"height",len)) {
    return KR_V4L2_PATH_INFO_HEIGHT;
  }
  if (!strncmp(string,"num",len)) {
    return KR_V4L2_PATH_INFO_NUM;
  }
  if (!strncmp(string,"den",len)) {
    return KR_V4L2_PATH_INFO_DEN;
  }
  if (!strncmp(string,"format",len)) {
    return KR_V4L2_PATH_INFO_FORMAT;
  }
  return -1;
}

kr_v4l2_info_member kr_v4l2_info_strto_member(char *string, int len) {
  if (!strncmp(string,"dev",len)) {
    return KR_V4L2_INFO_DEV;
  }
  if (!strncmp(string,"priority",len)) {
    return KR_V4L2_INFO_PRIORITY;
  }
  if (!strncmp(string,"state",len)) {
    return KR_V4L2_INFO_STATE;
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

int kr_v4l2_path_info_patch_apply(kr_v4l2_path_info *info, kr_v4l2_path_info_patch *patch) {
  const ptrdiff_t off[5] = { offsetof(kr_v4l2_path_info, width), 
    offsetof(kr_v4l2_path_info, height), offsetof(kr_v4l2_path_info, num), 
    offsetof(kr_v4l2_path_info, den), offsetof(kr_v4l2_path_info, format)
  };
  const size_t sz[5] = { sizeof(info->width), 
    sizeof(info->height), sizeof(info->num), 
    sizeof(info->den), sizeof(info->format)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_v4l2_path_info_patch_path(kr_v4l2_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_v4l2_path_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_v4l2_info_patch_apply(kr_v4l2_info *info, kr_v4l2_info_patch *patch) {
  const ptrdiff_t off[3] = { offsetof(kr_v4l2_info, dev), 
    offsetof(kr_v4l2_info, priority), offsetof(kr_v4l2_info, state)
  };
  const size_t sz[3] = { sizeof(info->dev), 
    sizeof(info->priority), sizeof(info->state)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_v4l2_info_patch_path(kr_v4l2_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_v4l2_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_v4l2_path_info_init(kr_v4l2_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_v4l2_path_info_valid(kr_v4l2_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_v4l2_path_info_random(kr_v4l2_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_v4l2_info_init(kr_v4l2_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_v4l2_info));

  return 0;
}

int kr_v4l2_info_valid(kr_v4l2_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_v4l2_info_random(kr_v4l2_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_v4l2_info));

  return 0;
}

