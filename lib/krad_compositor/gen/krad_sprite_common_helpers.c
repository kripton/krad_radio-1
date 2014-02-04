#include "krad_sprite_common_helpers.h"

kr_sprite_info_member kr_sprite_info_strto_member(char *string, int len) {
  if (!strncmp(string,"filename",len)) {
    return KR_SPRITE_INFO_FILENAME;
  }
  if (!strncmp(string,"rate",len)) {
    return KR_SPRITE_INFO_RATE;
  }
  if (!strncmp(string,"w",len)) {
    return KR_SPRITE_INFO_W;
  }
  if (!strncmp(string,"h",len)) {
    return KR_SPRITE_INFO_H;
  }
  return -1;
}

int kr_sprite_info_patch_apply(struct kr_sprite_info *info, kr_sprite_info_patch *patch) {
  const ptrdiff_t off[4] = { offsetof(struct kr_sprite_info, filename), 
    offsetof(struct kr_sprite_info, rate), offsetof(struct kr_sprite_info, w), 
    offsetof(struct kr_sprite_info, h)
  };
  const size_t sz[4] = { sizeof(info->filename), 
    sizeof(info->rate), sizeof(info->w), 
    sizeof(info->h)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_sprite_info_patch_path(kr_sprite_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_sprite_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_sprite_info_init(struct kr_sprite_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_sprite_info));

  return 0;
}

int kr_sprite_info_valid(struct kr_sprite_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 256; i++) {
    if (!st->filename[i]) {
      break;
    }
    if (i == 255 && st->filename[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_sprite_info_random(struct kr_sprite_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_sprite_info));
  for (i = 0; i < 256; i++) {
    scale = (double)25 / RAND_MAX;
    st->filename[i] = 97 + floor(rand() * scale);
    if (i == 255) {
      st->filename[255] = '\0';
    }
  }

  return 0;
}

