#include "krad_alsa_common_helpers.h"

kr_alsa_info_member kr_alsa_info_strto_member(char *string, int len) {
  if (!strncmp(string,"card",len)) {
    return KR_ALSA_INFO_CARD;
  }
  if (!strncmp(string,"name",len)) {
    return KR_ALSA_INFO_NAME;
  }
  return -1;
}

kr_alsa_path_info_member kr_alsa_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"card_num",len)) {
    return KR_ALSA_PATH_INFO_CARD_NUM;
  }
  return -1;
}

int kr_alsa_info_patch_apply(struct kr_alsa_info *info, kr_alsa_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_alsa_info, card), 
    offsetof(struct kr_alsa_info, name)
  };
  const size_t sz[2] = { sizeof(info->card), 
    sizeof(info->name)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_alsa_info_patch_path(kr_alsa_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_alsa_info_strto_member(name);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  return patch->value;
}

int kr_alsa_path_info_patch_apply(struct kr_alsa_path_info *info, kr_alsa_path_info_patch *patch) {
  const ptrdiff_t off[1] = { offsetof(struct kr_alsa_path_info, card_num)
  };
  const size_t sz[1] = { sizeof(info->card_num)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_alsa_path_info_patch_path(kr_alsa_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_alsa_path_info_strto_member(name);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  return patch->value;
}

int kr_alsa_info_init(struct kr_alsa_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_alsa_info));

  return 0;
}

int kr_alsa_info_valid(struct kr_alsa_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 40; i++) {
    if (!st->name[i]) {
      break;
    }
    if (i == 39 && st->name[i]) {
      return -3;
    }
  }

  return 0;
}

int kr_alsa_info_random(struct kr_alsa_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_alsa_info));
  for (i = 0; i < 40; i++) {
    scale = (double)25 / RAND_MAX;
    st->name[i] = 97 + floor(rand() * scale);
    if (i == 39) {
      st->name[39] = '\0';
    }
  }

  return 0;
}

int kr_alsa_path_info_init(struct kr_alsa_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_alsa_path_info_valid(struct kr_alsa_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_alsa_path_info_random(struct kr_alsa_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

