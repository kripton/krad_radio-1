#include "krad_adapter_common_helpers.h"

kr_adapter_info_member kr_adapter_info_strto_member(char *string, int len) {
  if (!strncmp(string,"api",len)) {
    return KR_ADAPTER_INFO_API;
  }
  if (!strncmp(string,"api_info",len)) {
    return KR_ADAPTER_INFO_API_INFO;
  }
  return -1;
}

kr_adapter_path_info_member kr_adapter_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"name",len)) {
    return KR_ADAPTER_PATH_INFO_NAME;
  }
  if (!strncmp(string,"dir",len)) {
    return KR_ADAPTER_PATH_INFO_DIR;
  }
  if (!strncmp(string,"api",len)) {
    return KR_ADAPTER_PATH_INFO_API;
  }
  if (!strncmp(string,"info",len)) {
    return KR_ADAPTER_PATH_INFO_INFO;
  }
  return -1;
}

int kr_adapter_path_direction_to_index(int val) {
  switch (val) {
    case KR_ADP_PATH_INPUT:
      return 0;
    case KR_ADP_PATH_OUTPUT:
      return 1;
  }
  return -1;
}

char *kr_strfr_kr_adapter_path_direction(int val) {
  switch (val) {
    case KR_ADP_PATH_INPUT:
      return "kr_adp_path_input";
    case KR_ADP_PATH_OUTPUT:
      return "kr_adp_path_output";
  }
  return NULL;
}

int kr_strto_kr_adapter_path_direction(char *string) {
  if (!strcmp(string,"kr_adp_path_input")) {
    return KR_ADP_PATH_INPUT;
  }
  if (!strcmp(string,"kr_adp_path_output")) {
    return KR_ADP_PATH_OUTPUT;
  }
  return -1;
}

int kr_adapter_api_to_index(int val) {
  switch (val) {
    case KR_ADP_JACK:
      return 0;
    case KR_ADP_WAYLAND:
      return 1;
    case KR_ADP_V4L2:
      return 2;
    case KR_ADP_DECKLINK:
      return 3;
    case KR_ADP_X11:
      return 4;
    case KR_ADP_ALSA:
      return 5;
    case KR_ADP_KRAD:
      return 6;
    case KR_ADP_ENCODER:
      return 7;
  }
  return -1;
}

char *kr_strfr_kr_adapter_api(int val) {
  switch (val) {
    case KR_ADP_JACK:
      return "kr_adp_jack";
    case KR_ADP_WAYLAND:
      return "kr_adp_wayland";
    case KR_ADP_V4L2:
      return "kr_adp_v4l2";
    case KR_ADP_DECKLINK:
      return "kr_adp_decklink";
    case KR_ADP_X11:
      return "kr_adp_x11";
    case KR_ADP_ALSA:
      return "kr_adp_alsa";
    case KR_ADP_KRAD:
      return "kr_adp_krad";
    case KR_ADP_ENCODER:
      return "kr_adp_encoder";
  }
  return NULL;
}

int kr_strto_kr_adapter_api(char *string) {
  if (!strcmp(string,"kr_adp_jack")) {
    return KR_ADP_JACK;
  }
  if (!strcmp(string,"kr_adp_wayland")) {
    return KR_ADP_WAYLAND;
  }
  if (!strcmp(string,"kr_adp_v4l2")) {
    return KR_ADP_V4L2;
  }
  if (!strcmp(string,"kr_adp_decklink")) {
    return KR_ADP_DECKLINK;
  }
  if (!strcmp(string,"kr_adp_x11")) {
    return KR_ADP_X11;
  }
  if (!strcmp(string,"kr_adp_alsa")) {
    return KR_ADP_ALSA;
  }
  if (!strcmp(string,"kr_adp_krad")) {
    return KR_ADP_KRAD;
  }
  if (!strcmp(string,"kr_adp_encoder")) {
    return KR_ADP_ENCODER;
  }
  return -1;
}

int kr_adapter_info_patch_apply(struct kr_adapter_info *info, kr_adapter_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_adapter_info, api), 
    offsetof(struct kr_adapter_info, api_info)
  };
  const size_t sz[2] = { sizeof(info->api), 
    sizeof(info->api_info)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_value *kr_adapter_info_address_to_patch(kr_adapter_info_patch *patch, kr_address2 *addr) {
   if (patch == NULL) return NULL;
  if (addr->count < 1) return NULL;
  if (addr->len[0] < 1) return NULL;
  patch->member = kr_adapter_info_strto_member(addr->path[0]);
  if (patch->member < 1) return NULL;
  switch(memb_type) {
      default: break;
    }
  }
  return patch->value;
}

int kr_adapter_path_info_patch_apply(struct kr_adapter_path_info *info, kr_adapter_path_info_patch *patch) {
  const ptrdiff_t off[4] = { offsetof(struct kr_adapter_path_info, name), 
    offsetof(struct kr_adapter_path_info, dir), offsetof(struct kr_adapter_path_info, api), 
    offsetof(struct kr_adapter_path_info, info)
  };
  const size_t sz[4] = { sizeof(info->name), 
    sizeof(info->dir), sizeof(info->api), 
    sizeof(info->info)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_value *kr_adapter_path_info_address_to_patch(kr_adapter_path_info_patch *patch, kr_address2 *addr) {
   if (patch == NULL) return NULL;
  if (addr->count < 1) return NULL;
  if (addr->len[0] < 1) return NULL;
  patch->member = kr_adapter_path_info_strto_member(addr->path[0]);
  if (patch->member < 1) return NULL;
  switch(memb_type) {
      default: break;
    }
  }
  return patch->value;
}

int kr_adapter_api_info_init(kr_adapter_api_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_adapter_api_info));
  switch (idx) {
    case 0: {
      kr_jack_info_init(&st->jack);
      break;
    }
    case 1: {
      kr_wayland_info_init(&st->wayland);
      break;
    }
    case 2: {
      kr_v4l2_info_init(&st->v4l2);
      break;
    }
    case 3: {
      kr_decklink_info_init(&st->decklink);
      break;
    }
    case 4: {
      kr_x11_info_init(&st->x11);
      break;
    }
    case 5: {
      kr_alsa_info_init(&st->alsa);
      break;
    }
  }


  return -1;
}

int kr_adapter_api_info_valid(kr_adapter_api_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  switch (idx) {
    case 0: {
      kr_jack_info_valid(&st->jack);
      break;
    }
    case 1: {
      kr_wayland_info_valid(&st->wayland);
      break;
    }
    case 2: {
      kr_v4l2_info_valid(&st->v4l2);
      break;
    }
    case 3: {
      kr_decklink_info_valid(&st->decklink);
      break;
    }
    case 4: {
      kr_x11_info_valid(&st->x11);
      break;
    }
    case 5: {
      kr_alsa_info_valid(&st->alsa);
      break;
    }
  }


  return -1;
}

int kr_adapter_api_info_random(kr_adapter_api_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_adapter_api_info));
  switch (idx) {
    case 0: {
      kr_jack_info_random(&st->jack);
      break;
    }
    case 1: {
      kr_wayland_info_random(&st->wayland);
      break;
    }
    case 2: {
      kr_v4l2_info_random(&st->v4l2);
      break;
    }
    case 3: {
      kr_decklink_info_random(&st->decklink);
      break;
    }
    case 4: {
      kr_x11_info_random(&st->x11);
      break;
    }
    case 5: {
      kr_alsa_info_random(&st->alsa);
      break;
    }
  }


  return -1;
}

int kr_adapter_api_path_info_init(kr_adapter_api_path_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_adapter_api_path_info));
  switch (idx) {
    case 0: {
      kr_jack_path_info_init(&st->jack);
      break;
    }
    case 1: {
      kr_wayland_path_info_init(&st->wayland);
      break;
    }
    case 2: {
      kr_v4l2_open_info_init(&st->v4l2);
      break;
    }
    case 3: {
      kr_decklink_path_info_init(&st->decklink);
      break;
    }
    case 4: {
      kr_x11_path_info_init(&st->x11);
      break;
    }
    case 5: {
      kr_alsa_path_info_init(&st->alsa);
      break;
    }
  }


  return -1;
}

int kr_adapter_api_path_info_valid(kr_adapter_api_path_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  switch (idx) {
    case 0: {
      kr_jack_path_info_valid(&st->jack);
      break;
    }
    case 1: {
      kr_wayland_path_info_valid(&st->wayland);
      break;
    }
    case 2: {
      kr_v4l2_open_info_valid(&st->v4l2);
      break;
    }
    case 3: {
      kr_decklink_path_info_valid(&st->decklink);
      break;
    }
    case 4: {
      kr_x11_path_info_valid(&st->x11);
      break;
    }
    case 5: {
      kr_alsa_path_info_valid(&st->alsa);
      break;
    }
  }


  return -1;
}

int kr_adapter_api_path_info_random(kr_adapter_api_path_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_adapter_api_path_info));
  switch (idx) {
    case 0: {
      kr_jack_path_info_random(&st->jack);
      break;
    }
    case 1: {
      kr_wayland_path_info_random(&st->wayland);
      break;
    }
    case 2: {
      kr_v4l2_open_info_random(&st->v4l2);
      break;
    }
    case 3: {
      kr_decklink_path_info_random(&st->decklink);
      break;
    }
    case 4: {
      kr_x11_path_info_random(&st->x11);
      break;
    }
    case 5: {
      kr_alsa_path_info_random(&st->alsa);
      break;
    }
  }


  return -1;
}

int kr_adapter_info_init(struct kr_adapter_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_adapter_info));
  kr_adapter_api_info_init(&st->api_info,kr_adapter_api_to_index(st->api));

  return 0;
}

int kr_adapter_info_valid(struct kr_adapter_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_adapter_api_info_valid(&st->api_info,kr_adapter_api_to_index(st->api));

  return 0;
}

int kr_adapter_info_random(struct kr_adapter_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_adapter_info));
  kr_adapter_api_info_random(&st->api_info,kr_adapter_api_to_index(st->api));

  return 0;
}

int kr_adapter_path_info_init(struct kr_adapter_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_adapter_path_info));
  kr_adapter_api_path_info_init(&st->info,kr_adapter_api_to_index(st->api));

  return 0;
}

int kr_adapter_path_info_valid(struct kr_adapter_path_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 64; i++) {
    if (!st->name[i]) {
      break;
    }
    if (i == 63 && st->name[i]) {
      return -2;
    }
  }
  kr_adapter_api_path_info_valid(&st->info,kr_adapter_api_to_index(st->api));

  return 0;
}

int kr_adapter_path_info_random(struct kr_adapter_path_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_adapter_path_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->name[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->name[63] = '\0';
    }
  }
  kr_adapter_api_path_info_random(&st->info,kr_adapter_api_to_index(st->api));

  return 0;
}

