#include "krad_x11_common_helpers.h"

int kr_x11_info_patch_apply(struct kr_x11_info *info, kr_x11_info_patch *patch) {
  const ptrdiff_t off[1] = { offsetof(struct kr_x11_info, display)
  };
  const size_t sz[1] = { sizeof(info->display)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
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

int kr_x11_info_init(void *st) {
  struct kr_x11_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_x11_info *)st;
  memset(actual, 0, sizeof(struct kr_x11_info));

  return 0;
}

int kr_x11_info_valid(void *st) {
  struct kr_x11_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_x11_info *)st;
  for (i = 0; i < 64; i++) {
    if (!actual->display[i]) {
      break;
    }
    if (i == 63 && actual->display[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_x11_info_random(void *st) {
  struct kr_x11_info *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_x11_info *)st;
  memset(actual, 0, sizeof(struct kr_x11_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    actual->display[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      actual->display[63] = '\0';
    }
  }

  return 0;
}

int kr_x11_path_info_init(void *st) {
  struct kr_x11_path_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_x11_path_info *)st;
  memset(actual, 0, sizeof(struct kr_x11_path_info));

  return 0;
}

int kr_x11_path_info_valid(void *st) {
  struct kr_x11_path_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_x11_path_info *)st;
  for (i = 0; i < 64; i++) {
    if (!actual->display[i]) {
      break;
    }
    if (i == 63 && actual->display[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_x11_path_info_random(void *st) {
  struct kr_x11_path_info *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_x11_path_info *)st;
  memset(actual, 0, sizeof(struct kr_x11_path_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    actual->display[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      actual->display[63] = '\0';
    }
  }

  return 0;
}

