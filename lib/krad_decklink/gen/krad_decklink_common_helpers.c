#include "krad_decklink_common_helpers.h"

kr_decklink_info_member kr_decklink_info_strto_member(char *string, int len) {
  if (!strncmp(string,"device",len)) {
    return KR_DECKLINK_INFO_DEVICE;
  }
  return -1;
}

kr_decklink_path_info_member kr_decklink_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"device",len)) {
    return KR_DECKLINK_PATH_INFO_DEVICE;
  }
  if (!strncmp(string,"width",len)) {
    return KR_DECKLINK_PATH_INFO_WIDTH;
  }
  if (!strncmp(string,"height",len)) {
    return KR_DECKLINK_PATH_INFO_HEIGHT;
  }
  if (!strncmp(string,"num",len)) {
    return KR_DECKLINK_PATH_INFO_NUM;
  }
  if (!strncmp(string,"den",len)) {
    return KR_DECKLINK_PATH_INFO_DEN;
  }
  if (!strncmp(string,"video_connector",len)) {
    return KR_DECKLINK_PATH_INFO_VIDEO_CONNECTOR;
  }
  if (!strncmp(string,"audio_connector",len)) {
    return KR_DECKLINK_PATH_INFO_AUDIO_CONNECTOR;
  }
  return -1;
}

int kr_decklink_info_patch_apply(struct kr_decklink_info *info, kr_decklink_info_patch *patch) {
  const ptrdiff_t off[1] = { offsetof(struct kr_decklink_info, device)
  };
  const size_t sz[1] = { sizeof(info->device)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_decklink_info_patch_path(kr_decklink_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_decklink_info_strto_member(name);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  return patch->value;
}

int kr_decklink_path_info_patch_apply(struct kr_decklink_path_info *info, kr_decklink_path_info_patch *patch) {
  const ptrdiff_t off[7] = { offsetof(struct kr_decklink_path_info, device), 
    offsetof(struct kr_decklink_path_info, width), offsetof(struct kr_decklink_path_info, height), 
    offsetof(struct kr_decklink_path_info, num), offsetof(struct kr_decklink_path_info, den), 
    offsetof(struct kr_decklink_path_info, video_connector), offsetof(struct kr_decklink_path_info, audio_connector)
  };
  const size_t sz[7] = { sizeof(info->device), 
    sizeof(info->width), sizeof(info->height), 
    sizeof(info->num), sizeof(info->den), 
    sizeof(info->video_connector), sizeof(info->audio_connector)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_decklink_path_info_patch_path(kr_decklink_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_decklink_path_info_strto_member(name);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  return patch->value;
}

int kr_decklink_info_init(struct kr_decklink_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_decklink_info));

  return 0;
}

int kr_decklink_info_valid(struct kr_decklink_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 64; i++) {
    if (!st->device[i]) {
      break;
    }
    if (i == 63 && st->device[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_decklink_info_random(struct kr_decklink_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_decklink_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->device[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->device[63] = '\0';
    }
  }

  return 0;
}

int kr_decklink_path_info_init(struct kr_decklink_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_decklink_path_info));

  return 0;
}

int kr_decklink_path_info_valid(struct kr_decklink_path_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 64; i++) {
    if (!st->device[i]) {
      break;
    }
    if (i == 63 && st->device[i]) {
      return -2;
    }
  }
  for (i = 0; i < 64; i++) {
    if (!st->video_connector[i]) {
      break;
    }
    if (i == 63 && st->video_connector[i]) {
      return -7;
    }
  }
  for (i = 0; i < 64; i++) {
    if (!st->audio_connector[i]) {
      break;
    }
    if (i == 63 && st->audio_connector[i]) {
      return -8;
    }
  }

  return 0;
}

int kr_decklink_path_info_random(struct kr_decklink_path_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_decklink_path_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->device[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->device[63] = '\0';
    }
  }
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->video_connector[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->video_connector[63] = '\0';
    }
  }
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->audio_connector[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->audio_connector[63] = '\0';
    }
  }

  return 0;
}

