#include "kr_aux_common_helpers.h"

kr_aux_info_member kr_aux_info_strto_member(char *string, int len) {
  if (!strncmp(string,"monkeyname",len)) {
    return KR_AUX_INFO_MONKEYNAME;
  }
  return -1;
}

kr_aux_path_info_member kr_aux_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"ncoconuts",len)) {
    return KR_AUX_PATH_INFO_NCOCONUTS;
  }
  return -1;
}

int kr_aux_info_patch_apply(struct kr_aux_info *info, kr_aux_info_patch *patch) {
  const ptrdiff_t off[1] = { offsetof(struct kr_aux_info, monkeyname)
  };
  const size_t sz[1] = { sizeof(info->monkeyname)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_aux_info_patch_path(kr_aux_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_aux_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_aux_path_info_patch_apply(struct kr_aux_path_info *info, kr_aux_path_info_patch *patch) {
  const ptrdiff_t off[1] = { offsetof(struct kr_aux_path_info, ncoconuts)
  };
  const size_t sz[1] = { sizeof(info->ncoconuts)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_aux_path_info_patch_path(kr_aux_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_aux_path_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_aux_info_init(struct kr_aux_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_aux_info));

  return 0;
}

int kr_aux_info_valid(struct kr_aux_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 64; i++) {
    if (!st->monkeyname[i]) {
      break;
    }
    if (i == 63 && st->monkeyname[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_aux_info_random(struct kr_aux_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_aux_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->monkeyname[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->monkeyname[63] = '\0';
    }
  }

  return 0;
}

int kr_aux_path_info_init(struct kr_aux_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_aux_path_info_valid(struct kr_aux_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_aux_path_info_random(struct kr_aux_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

