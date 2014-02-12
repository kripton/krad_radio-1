#include "krad_wayland_common_helpers.h"

kr_wayland_info_member kr_wayland_info_strto_member(char *string, int len) {
  if (!strncmp(string,"display_name",len)) {
    return KR_WAYLAND_INFO_DISPLAY_NAME;
  }
  if (!strncmp(string,"state",len)) {
    return KR_WAYLAND_INFO_STATE;
  }
  return -1;
}

kr_wayland_path_info_member kr_wayland_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"width",len)) {
    return KR_WAYLAND_PATH_INFO_WIDTH;
  }
  if (!strncmp(string,"height",len)) {
    return KR_WAYLAND_PATH_INFO_HEIGHT;
  }
  if (!strncmp(string,"fullscreen",len)) {
    return KR_WAYLAND_PATH_INFO_FULLSCREEN;
  }
  return -1;
}

int kr_wayland_state_to_index(int val) {
  switch (val) {
    case KR_WL_DISCONNECTED:
      return 0;
    case KR_WL_CONNECTED:
      return 1;
  }
  return -1;
}

char *kr_strfr_kr_wayland_state(int val) {
  switch (val) {
    case KR_WL_DISCONNECTED:
      return "kr_wl_disconnected";
    case KR_WL_CONNECTED:
      return "kr_wl_connected";
  }
  return NULL;
}

int kr_strto_kr_wayland_state(char *string) {
  if (!strcmp(string,"kr_wl_disconnected")) {
    return KR_WL_DISCONNECTED;
  }
  if (!strcmp(string,"kr_wl_connected")) {
    return KR_WL_CONNECTED;
  }
  return -1;
}

int kr_wayland_info_patch_apply(kr_wayland_info *info, kr_wayland_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(kr_wayland_info, display_name), 
    offsetof(kr_wayland_info, state)
  };
  const size_t sz[2] = { sizeof(info->display_name), 
    sizeof(info->state)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_wayland_info_patch_path(kr_wayland_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_wayland_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_wayland_path_info_patch_apply(kr_wayland_path_info *info, kr_wayland_path_info_patch *patch) {
  const ptrdiff_t off[3] = { offsetof(kr_wayland_path_info, width), 
    offsetof(kr_wayland_path_info, height), offsetof(kr_wayland_path_info, fullscreen)
  };
  const size_t sz[3] = { sizeof(info->width), 
    sizeof(info->height), sizeof(info->fullscreen)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_wayland_path_info_patch_path(kr_wayland_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_wayland_path_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_wayland_info_init(kr_wayland_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_wayland_info));

  return 0;
}

int kr_wayland_info_valid(kr_wayland_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 128; i++) {
    if (!st->display_name[i]) {
      break;
    }
    if (i == 127 && st->display_name[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_wayland_info_random(kr_wayland_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_wayland_info));
  for (i = 0; i < 128; i++) {
    scale = (double)25 / RAND_MAX;
    st->display_name[i] = 97 + floor(rand() * scale);
    if (i == 127) {
      st->display_name[127] = '\0';
    }
  }

  return 0;
}

int kr_wayland_path_info_init(kr_wayland_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_wayland_path_info_valid(kr_wayland_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_wayland_path_info_random(kr_wayland_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

