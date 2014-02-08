#include "krad_transponder_common_helpers.h"

kr_transponder_path_info_member kr_transponder_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"type",len)) {
    return KR_TRANSPONDER_PATH_INFO_TYPE;
  }
  if (!strncmp(string,"aio",len)) {
    return KR_TRANSPONDER_PATH_INFO_AIO;
  }
  return -1;
}

int kr_transponder_path_type_to_index(int val) {
  switch (val) {
    case KR_XR_JACK:
      return 0;
    case KR_XR_JACK_INPUT:
      return 1;
    case KR_XR_JACK_OUTPUT:
      return 2;
    case KR_XR_WAYLAND:
      return 3;
    case KR_XR_WAYLAND_OUTPUT:
      return 4;
    case KR_XR_V4L2:
      return 5;
    case KR_XR_V4L2_INPUT:
      return 6;
    case KR_XR_DECKLINK:
      return 7;
    case KR_XR_DECKLINK_INPUT:
      return 8;
    case KR_XR_X11:
      return 9;
    case KR_XR_X11_INPUT:
      return 10;
    case KR_XR_ALSA:
      return 11;
    case KR_XR_ALSA_INPUT:
      return 12;
    case KR_XR_ALSA_OUTPUT:
      return 13;
    case KR_XR_KRAD:
      return 14;
    case KR_XR_KRAD_INPUT:
      return 15;
    case KR_XR_KRAD_OUTPUT:
      return 16;
  }
  return -1;
}

char *kr_strfr_kr_transponder_path_type(int val) {
  switch (val) {
    case KR_XR_JACK:
      return "kr_xr_jack";
    case KR_XR_JACK_INPUT:
      return "kr_xr_jack_input";
    case KR_XR_JACK_OUTPUT:
      return "kr_xr_jack_output";
    case KR_XR_WAYLAND:
      return "kr_xr_wayland";
    case KR_XR_WAYLAND_OUTPUT:
      return "kr_xr_wayland_output";
    case KR_XR_V4L2:
      return "kr_xr_v4l2";
    case KR_XR_V4L2_INPUT:
      return "kr_xr_v4l2_input";
    case KR_XR_DECKLINK:
      return "kr_xr_decklink";
    case KR_XR_DECKLINK_INPUT:
      return "kr_xr_decklink_input";
    case KR_XR_X11:
      return "kr_xr_x11";
    case KR_XR_X11_INPUT:
      return "kr_xr_x11_input";
    case KR_XR_ALSA:
      return "kr_xr_alsa";
    case KR_XR_ALSA_INPUT:
      return "kr_xr_alsa_input";
    case KR_XR_ALSA_OUTPUT:
      return "kr_xr_alsa_output";
    case KR_XR_KRAD:
      return "kr_xr_krad";
    case KR_XR_KRAD_INPUT:
      return "kr_xr_krad_input";
    case KR_XR_KRAD_OUTPUT:
      return "kr_xr_krad_output";
  }
  return NULL;
}

int kr_strto_kr_transponder_path_type(char *string) {
  if (!strcmp(string,"kr_xr_jack")) {
    return KR_XR_JACK;
  }
  if (!strcmp(string,"kr_xr_jack_input")) {
    return KR_XR_JACK_INPUT;
  }
  if (!strcmp(string,"kr_xr_jack_output")) {
    return KR_XR_JACK_OUTPUT;
  }
  if (!strcmp(string,"kr_xr_wayland")) {
    return KR_XR_WAYLAND;
  }
  if (!strcmp(string,"kr_xr_wayland_output")) {
    return KR_XR_WAYLAND_OUTPUT;
  }
  if (!strcmp(string,"kr_xr_v4l2")) {
    return KR_XR_V4L2;
  }
  if (!strcmp(string,"kr_xr_v4l2_input")) {
    return KR_XR_V4L2_INPUT;
  }
  if (!strcmp(string,"kr_xr_decklink")) {
    return KR_XR_DECKLINK;
  }
  if (!strcmp(string,"kr_xr_decklink_input")) {
    return KR_XR_DECKLINK_INPUT;
  }
  if (!strcmp(string,"kr_xr_x11")) {
    return KR_XR_X11;
  }
  if (!strcmp(string,"kr_xr_x11_input")) {
    return KR_XR_X11_INPUT;
  }
  if (!strcmp(string,"kr_xr_alsa")) {
    return KR_XR_ALSA;
  }
  if (!strcmp(string,"kr_xr_alsa_input")) {
    return KR_XR_ALSA_INPUT;
  }
  if (!strcmp(string,"kr_xr_alsa_output")) {
    return KR_XR_ALSA_OUTPUT;
  }
  if (!strcmp(string,"kr_xr_krad")) {
    return KR_XR_KRAD;
  }
  if (!strcmp(string,"kr_xr_krad_input")) {
    return KR_XR_KRAD_INPUT;
  }
  if (!strcmp(string,"kr_xr_krad_output")) {
    return KR_XR_KRAD_OUTPUT;
  }
  return -1;
}

int kr_transponder_path_info_patch_apply(struct kr_transponder_path_info *info, kr_transponder_path_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_transponder_path_info, type), 
    offsetof(struct kr_transponder_path_info, aio)
  };
  const size_t sz[2] = { sizeof(info->type), 
    sizeof(info->aio)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_transponder_path_info_patch_path(kr_transponder_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_transponder_path_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_transponder_path_type_info_init(kr_transponder_path_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_transponder_path_type_info));
  switch (idx) {
    case 0: {
      kr_jack_info_init(&st->jack);
      break;
    }
    case 1: {
      kr_jack_path_info_init(&st->jack_input);
      break;
    }
    case 2: {
      kr_jack_path_info_init(&st->jack_output);
      break;
    }
    case 3: {
      kr_wayland_info_init(&st->wayland);
      break;
    }
    case 4: {
      kr_wayland_path_info_init(&st->wayland_output);
      break;
    }
    case 5: {
      kr_v4l2_info_init(&st->v4l2);
      break;
    }
    case 6: {
      kr_v4l2_open_info_init(&st->v4l2_input);
      break;
    }
    case 7: {
      kr_decklink_info_init(&st->decklink);
      break;
    }
    case 8: {
      kr_decklink_path_info_init(&st->decklink_input);
      break;
    }
    case 9: {
      kr_x11_info_init(&st->x11);
      break;
    }
    case 10: {
      kr_x11_path_info_init(&st->x11_input);
      break;
    }
    case 11: {
      kr_alsa_info_init(&st->alsa);
      break;
    }
    case 12: {
      kr_alsa_path_info_init(&st->alsa_input);
      break;
    }
    case 13: {
      kr_alsa_path_info_init(&st->alsa_output);
      break;
    }
  }


  return -1;
}

int kr_transponder_path_type_info_valid(kr_transponder_path_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  switch (idx) {
    case 0: {
      kr_jack_info_valid(&st->jack);
      break;
    }
    case 1: {
      kr_jack_path_info_valid(&st->jack_input);
      break;
    }
    case 2: {
      kr_jack_path_info_valid(&st->jack_output);
      break;
    }
    case 3: {
      kr_wayland_info_valid(&st->wayland);
      break;
    }
    case 4: {
      kr_wayland_path_info_valid(&st->wayland_output);
      break;
    }
    case 5: {
      kr_v4l2_info_valid(&st->v4l2);
      break;
    }
    case 6: {
      kr_v4l2_open_info_valid(&st->v4l2_input);
      break;
    }
    case 7: {
      kr_decklink_info_valid(&st->decklink);
      break;
    }
    case 8: {
      kr_decklink_path_info_valid(&st->decklink_input);
      break;
    }
    case 9: {
      kr_x11_info_valid(&st->x11);
      break;
    }
    case 10: {
      kr_x11_path_info_valid(&st->x11_input);
      break;
    }
    case 11: {
      kr_alsa_info_valid(&st->alsa);
      break;
    }
    case 12: {
      kr_alsa_path_info_valid(&st->alsa_input);
      break;
    }
    case 13: {
      kr_alsa_path_info_valid(&st->alsa_output);
      break;
    }
  }


  return -1;
}

int kr_transponder_path_type_info_random(kr_transponder_path_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_transponder_path_type_info));
  switch (idx) {
    case 0: {
      kr_jack_info_random(&st->jack);
      break;
    }
    case 1: {
      kr_jack_path_info_random(&st->jack_input);
      break;
    }
    case 2: {
      kr_jack_path_info_random(&st->jack_output);
      break;
    }
    case 3: {
      kr_wayland_info_random(&st->wayland);
      break;
    }
    case 4: {
      kr_wayland_path_info_random(&st->wayland_output);
      break;
    }
    case 5: {
      kr_v4l2_info_random(&st->v4l2);
      break;
    }
    case 6: {
      kr_v4l2_open_info_random(&st->v4l2_input);
      break;
    }
    case 7: {
      kr_decklink_info_random(&st->decklink);
      break;
    }
    case 8: {
      kr_decklink_path_info_random(&st->decklink_input);
      break;
    }
    case 9: {
      kr_x11_info_random(&st->x11);
      break;
    }
    case 10: {
      kr_x11_path_info_random(&st->x11_input);
      break;
    }
    case 11: {
      kr_alsa_info_random(&st->alsa);
      break;
    }
    case 12: {
      kr_alsa_path_info_random(&st->alsa_input);
      break;
    }
    case 13: {
      kr_alsa_path_info_random(&st->alsa_output);
      break;
    }
  }


  return -1;
}

int kr_transponder_path_info_init(struct kr_transponder_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_transponder_path_info));
  kr_transponder_path_type_info_init(&st->aio,kr_transponder_path_type_to_index(st->type));

  return 0;
}

int kr_transponder_path_info_valid(struct kr_transponder_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_transponder_path_type_info_valid(&st->aio,kr_transponder_path_type_to_index(st->type));

  return 0;
}

int kr_transponder_path_info_random(struct kr_transponder_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_transponder_path_info));
  kr_transponder_path_type_info_random(&st->aio,kr_transponder_path_type_to_index(st->type));

  return 0;
}

