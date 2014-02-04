#include "krad_text_common_helpers.h"

kr_text_info_member kr_text_info_strto_member(char *string, int len) {
  if (!strncmp(string,"string",len)) {
    return KR_TEXT_INFO_STRING;
  }
  if (!strncmp(string,"font",len)) {
    return KR_TEXT_INFO_FONT;
  }
  if (!strncmp(string,"red",len)) {
    return KR_TEXT_INFO_RED;
  }
  if (!strncmp(string,"green",len)) {
    return KR_TEXT_INFO_GREEN;
  }
  if (!strncmp(string,"blue",len)) {
    return KR_TEXT_INFO_BLUE;
  }
  return -1;
}

int kr_text_info_patch_apply(struct kr_text_info *info, kr_text_info_patch *patch) {
  const ptrdiff_t off[5] = { offsetof(struct kr_text_info, string), 
    offsetof(struct kr_text_info, font), offsetof(struct kr_text_info, red), 
    offsetof(struct kr_text_info, green), offsetof(struct kr_text_info, blue)
  };
  const size_t sz[5] = { sizeof(info->string), 
    sizeof(info->font), sizeof(info->red), 
    sizeof(info->green), sizeof(info->blue)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_text_info_patch_path(kr_text_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_text_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_text_info_init(struct kr_text_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_text_info));

  return 0;
}

int kr_text_info_valid(struct kr_text_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 512; i++) {
    if (!st->string[i]) {
      break;
    }
    if (i == 511 && st->string[i]) {
      return -2;
    }
  }
  for (i = 0; i < 256; i++) {
    if (!st->font[i]) {
      break;
    }
    if (i == 255 && st->font[i]) {
      return -3;
    }
  }

  return 0;
}

int kr_text_info_random(struct kr_text_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_text_info));
  for (i = 0; i < 512; i++) {
    scale = (double)25 / RAND_MAX;
    st->string[i] = 97 + floor(rand() * scale);
    if (i == 511) {
      st->string[511] = '\0';
    }
  }
  for (i = 0; i < 256; i++) {
    scale = (double)25 / RAND_MAX;
    st->font[i] = 97 + floor(rand() * scale);
    if (i == 255) {
      st->font[255] = '\0';
    }
  }

  return 0;
}

