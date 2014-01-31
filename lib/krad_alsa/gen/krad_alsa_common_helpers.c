#include "krad_alsa_common_helpers.h"

int kr_alsa_info_patch_apply(struct kr_alsa_info *info, kr_alsa_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_alsa_info, card), 
    offsetof(struct kr_alsa_info, name)
  };
  const size_t sz[2] = { sizeof(info->card), 
    sizeof(info->name)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_alsa_path_info_patch_apply(struct kr_alsa_path_info *info, kr_alsa_path_info_patch *patch) {
  const ptrdiff_t off[1] = { offsetof(struct kr_alsa_path_info, card_num)
  };
  const size_t sz[1] = { sizeof(info->card_num)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_alsa_info_init(void *st) {
  struct kr_alsa_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_alsa_info *)st;
  memset(actual, 0, sizeof(struct kr_alsa_info));

  return 0;
}

int kr_alsa_info_valid(void *st) {
  struct kr_alsa_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_alsa_info *)st;
  for (i = 0; i < 40; i++) {
    if (!actual->name[i]) {
      break;
    }
    if (i == 39 && actual->name[i]) {
      return -3;
    }
  }

  return 0;
}

int kr_alsa_info_random(void *st) {
  struct kr_alsa_info *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_alsa_info *)st;
  memset(actual, 0, sizeof(struct kr_alsa_info));
  for (i = 0; i < 40; i++) {
    scale = (double)25 / RAND_MAX;
    actual->name[i] = 97 + floor(rand() * scale);
    if (i == 39) {
      actual->name[39] = '\0';
    }
  }

  return 0;
}

int kr_alsa_path_info_init(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_alsa_path_info_valid(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_alsa_path_info_random(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

