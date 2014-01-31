#include "krad_wayland_common_helpers.h"

int kr_wayland_info_patch_apply(kr_wayland_info *info, kr_wayland_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(kr_wayland_info, state), 
    offsetof(kr_wayland_info, display_name)
  };
  const size_t sz[2] = { sizeof(info->state), 
    sizeof(info->display_name)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_wayland_path_info_patch_apply(kr_wayland_path_info *info, kr_wayland_path_info_patch *patch) {
  const ptrdiff_t off[5] = { offsetof(kr_wayland_path_info, display_name), 
    offsetof(kr_wayland_path_info, state), offsetof(kr_wayland_path_info, width), 
    offsetof(kr_wayland_path_info, height), offsetof(kr_wayland_path_info, fullscreen)
  };
  const size_t sz[5] = { sizeof(info->display_name), 
    sizeof(info->state), sizeof(info->width), 
    sizeof(info->height), sizeof(info->fullscreen)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_wayland_info_init(void *st) {
  kr_wayland_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (kr_wayland_info *)st;
  memset(actual, 0, sizeof(kr_wayland_info));

  return 0;
}

int kr_wayland_info_valid(void *st) {
  kr_wayland_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (kr_wayland_info *)st;
  for (i = 0; i < 128; i++) {
    if (!actual->display_name[i]) {
      break;
    }
    if (i == 127 && actual->display_name[i]) {
      return -3;
    }
  }

  return 0;
}

int kr_wayland_info_random(void *st) {
  kr_wayland_info *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (kr_wayland_info *)st;
  memset(actual, 0, sizeof(kr_wayland_info));
  for (i = 0; i < 128; i++) {
    scale = (double)25 / RAND_MAX;
    actual->display_name[i] = 97 + floor(rand() * scale);
    if (i == 127) {
      actual->display_name[127] = '\0';
    }
  }

  return 0;
}

int kr_wayland_path_info_init(void *st) {
  kr_wayland_path_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (kr_wayland_path_info *)st;
  memset(actual, 0, sizeof(kr_wayland_path_info));

  return 0;
}

int kr_wayland_path_info_valid(void *st) {
  kr_wayland_path_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (kr_wayland_path_info *)st;
  for (i = 0; i < 128; i++) {
    if (!actual->display_name[i]) {
      break;
    }
    if (i == 127 && actual->display_name[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_wayland_path_info_random(void *st) {
  kr_wayland_path_info *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (kr_wayland_path_info *)st;
  memset(actual, 0, sizeof(kr_wayland_path_info));
  for (i = 0; i < 128; i++) {
    scale = (double)25 / RAND_MAX;
    actual->display_name[i] = 97 + floor(rand() * scale);
    if (i == 127) {
      actual->display_name[127] = '\0';
    }
  }

  return 0;
}

