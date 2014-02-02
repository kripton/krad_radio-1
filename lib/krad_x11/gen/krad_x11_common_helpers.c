#include "krad_x11_common_helpers.h"

kr_x11_info_member kr_x11_info_strto_member(char *string, int len) {
  if (!strncmp(string,"display",len)) {
    return KR_X11_INFO_DISPLAY;
  }
  return -1;
}

kr_x11_path_info_member kr_x11_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"display",len)) {
    return KR_X11_PATH_INFO_DISPLAY;
  }
  if (!strncmp(string,"width",len)) {
    return KR_X11_PATH_INFO_WIDTH;
  }
  if (!strncmp(string,"height",len)) {
    return KR_X11_PATH_INFO_HEIGHT;
  }
  if (!strncmp(string,"num",len)) {
    return KR_X11_PATH_INFO_NUM;
  }
  if (!strncmp(string,"den",len)) {
    return KR_X11_PATH_INFO_DEN;
  }
  if (!strncmp(string,"x",len)) {
    return KR_X11_PATH_INFO_X;
  }
  if (!strncmp(string,"y",len)) {
    return KR_X11_PATH_INFO_Y;
  }
  return -1;
}

int kr_x11_info_patch_apply(struct kr_x11_info *info, kr_x11_info_patch *patch) {
  const ptrdiff_t off[1] = { offsetof(struct kr_x11_info, display)
  };
  const size_t sz[1] = { sizeof(info->display)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_x11_info_patch_path(kr_x11_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_x11_info_strto_member(name);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  return patch->value;
}

int kr_x11_path_info_patch_apply(struct kr_x11_path_info *info, kr_x11_path_info_patch *patch) {
  const ptrdiff_t off[7] = { offsetof(struct kr_x11_path_info, display), 
    offsetof(struct kr_x11_path_info, width), offsetof(struct kr_x11_path_info, height), 
    offsetof(struct kr_x11_path_info, num), offsetof(struct kr_x11_path_info, den), 
    offsetof(struct kr_x11_path_info, x), offsetof(struct kr_x11_path_info, y)
  };
  const size_t sz[7] = { sizeof(info->display), 
    sizeof(info->width), sizeof(info->height), 
    sizeof(info->num), sizeof(info->den), 
    sizeof(info->x), sizeof(info->y)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_x11_path_info_patch_path(kr_x11_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_x11_path_info_strto_member(name);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  return patch->value;
}

int kr_x11_info_init(struct kr_x11_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_x11_info));

  return 0;
}

int kr_x11_info_valid(struct kr_x11_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 64; i++) {
    if (!st->display[i]) {
      break;
    }
    if (i == 63 && st->display[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_x11_info_random(struct kr_x11_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_x11_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->display[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->display[63] = '\0';
    }
  }

  return 0;
}

int kr_x11_path_info_init(struct kr_x11_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_x11_path_info));

  return 0;
}

int kr_x11_path_info_valid(struct kr_x11_path_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 64; i++) {
    if (!st->display[i]) {
      break;
    }
    if (i == 63 && st->display[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_x11_path_info_random(struct kr_x11_path_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_x11_path_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->display[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->display[63] = '\0';
    }
  }

  return 0;
}

