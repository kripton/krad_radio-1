#include "kr_xpdr_common_helpers.h"

kr_xpdr_path_info_member kr_xpdr_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"type",len)) {
    return KR_XPDR_PATH_INFO_TYPE;
  }
  if (!strncmp(string,"adp",len)) {
    return KR_XPDR_PATH_INFO_ADP;
  }
  return -1;
}

int kr_xpdr_type_to_index(int val) {
  switch (val) {
    case KR_JACK:
      return 0;
    case KR_WAYLAND:
      return 1;
    case KR_V4L2:
      return 2;
    case KR_DECKLINK:
      return 3;
    case KR_X11:
      return 4;
    case KR_ALSA:
      return 5;
    case KR_AUX:
      return 6;
    case KR_JACK_IN:
      return 7;
    case KR_JACK_OUT:
      return 8;
    case KR_WAYLAND_OUT:
      return 9;
    case KR_V4L2_IN:
      return 10;
    case KR_DECKLINK_IN:
      return 11;
    case KR_X11_IN:
      return 12;
    case KR_ALSA_IN:
      return 13;
    case KR_ALSA_OUT:
      return 14;
    case KR_AUX_IN:
      return 15;
    case KR_AUX_OUT:
      return 16;
  }
  return -1;
}

char *kr_strfr_kr_xpdr_type(int val) {
  switch (val) {
    case KR_JACK:
      return "kr_jack";
    case KR_WAYLAND:
      return "kr_wayland";
    case KR_V4L2:
      return "kr_v4l2";
    case KR_DECKLINK:
      return "kr_decklink";
    case KR_X11:
      return "kr_x11";
    case KR_ALSA:
      return "kr_alsa";
    case KR_AUX:
      return "kr_aux";
    case KR_JACK_IN:
      return "kr_jack_in";
    case KR_JACK_OUT:
      return "kr_jack_out";
    case KR_WAYLAND_OUT:
      return "kr_wayland_out";
    case KR_V4L2_IN:
      return "kr_v4l2_in";
    case KR_DECKLINK_IN:
      return "kr_decklink_in";
    case KR_X11_IN:
      return "kr_x11_in";
    case KR_ALSA_IN:
      return "kr_alsa_in";
    case KR_ALSA_OUT:
      return "kr_alsa_out";
    case KR_AUX_IN:
      return "kr_aux_in";
    case KR_AUX_OUT:
      return "kr_aux_out";
  }
  return NULL;
}

int kr_strto_kr_xpdr_type(char *string) {
  if (!strcmp(string,"kr_jack")) {
    return KR_JACK;
  }
  if (!strcmp(string,"kr_wayland")) {
    return KR_WAYLAND;
  }
  if (!strcmp(string,"kr_v4l2")) {
    return KR_V4L2;
  }
  if (!strcmp(string,"kr_decklink")) {
    return KR_DECKLINK;
  }
  if (!strcmp(string,"kr_x11")) {
    return KR_X11;
  }
  if (!strcmp(string,"kr_alsa")) {
    return KR_ALSA;
  }
  if (!strcmp(string,"kr_aux")) {
    return KR_AUX;
  }
  if (!strcmp(string,"kr_jack_in")) {
    return KR_JACK_IN;
  }
  if (!strcmp(string,"kr_jack_out")) {
    return KR_JACK_OUT;
  }
  if (!strcmp(string,"kr_wayland_out")) {
    return KR_WAYLAND_OUT;
  }
  if (!strcmp(string,"kr_v4l2_in")) {
    return KR_V4L2_IN;
  }
  if (!strcmp(string,"kr_decklink_in")) {
    return KR_DECKLINK_IN;
  }
  if (!strcmp(string,"kr_x11_in")) {
    return KR_X11_IN;
  }
  if (!strcmp(string,"kr_alsa_in")) {
    return KR_ALSA_IN;
  }
  if (!strcmp(string,"kr_alsa_out")) {
    return KR_ALSA_OUT;
  }
  if (!strcmp(string,"kr_aux_in")) {
    return KR_AUX_IN;
  }
  if (!strcmp(string,"kr_aux_out")) {
    return KR_AUX_OUT;
  }
  return -1;
}

int kr_xpdr_path_info_patch_apply(struct kr_xpdr_path_info *info, kr_xpdr_path_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_xpdr_path_info, type), 
    offsetof(struct kr_xpdr_path_info, adp)
  };
  const size_t sz[2] = { sizeof(info->type), 
    sizeof(info->adp)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_xpdr_path_info_patch_path(kr_xpdr_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_xpdr_path_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_xpdr_type_info_init(kr_xpdr_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_xpdr_type_info));
  switch (idx) {
    case 0: {
      kr_jack_info_init(&st->jack);
      break;
    }
    case 1: {
      kr_wayland_info_init(&st->wl);
      break;
    }
    case 2: {
      kr_v4l2_info_init(&st->v4l2);
      break;
    }
    case 3: {
      kr_decklink_info_init(&st->dl);
      break;
    }
    case 4: {
      kr_x11_info_init(&st->x11);
      break;
    }
    case 7: {
      kr_jack_path_info_init(&st->jack_in);
      break;
    }
    case 8: {
      kr_jack_path_info_init(&st->jack_out);
      break;
    }
    case 9: {
      kr_wayland_path_info_init(&st->wl_out);
      break;
    }
    case 10: {
      kr_v4l2_open_info_init(&st->v4l2_in);
      break;
    }
    case 11: {
      kr_decklink_path_info_init(&st->dl_in);
      break;
    }
    case 12: {
      kr_x11_path_info_init(&st->x11_in);
      break;
    }
    case 13: {
      kr_alsa_path_info_init(&st->alsa_in);
      break;
    }
    case 14: {
      kr_alsa_path_info_init(&st->alsa_out);
      break;
    }
  }


  return -1;
}

int kr_xpdr_type_info_valid(kr_xpdr_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  switch (idx) {
    case 0: {
      kr_jack_info_valid(&st->jack);
      break;
    }
    case 1: {
      kr_wayland_info_valid(&st->wl);
      break;
    }
    case 2: {
      kr_v4l2_info_valid(&st->v4l2);
      break;
    }
    case 3: {
      kr_decklink_info_valid(&st->dl);
      break;
    }
    case 4: {
      kr_x11_info_valid(&st->x11);
      break;
    }
    case 7: {
      kr_jack_path_info_valid(&st->jack_in);
      break;
    }
    case 8: {
      kr_jack_path_info_valid(&st->jack_out);
      break;
    }
    case 9: {
      kr_wayland_path_info_valid(&st->wl_out);
      break;
    }
    case 10: {
      kr_v4l2_open_info_valid(&st->v4l2_in);
      break;
    }
    case 11: {
      kr_decklink_path_info_valid(&st->dl_in);
      break;
    }
    case 12: {
      kr_x11_path_info_valid(&st->x11_in);
      break;
    }
    case 13: {
      kr_alsa_path_info_valid(&st->alsa_in);
      break;
    }
    case 14: {
      kr_alsa_path_info_valid(&st->alsa_out);
      break;
    }
  }


  return -1;
}

int kr_xpdr_type_info_random(kr_xpdr_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_xpdr_type_info));
  switch (idx) {
    case 0: {
      kr_jack_info_random(&st->jack);
      break;
    }
    case 1: {
      kr_wayland_info_random(&st->wl);
      break;
    }
    case 2: {
      kr_v4l2_info_random(&st->v4l2);
      break;
    }
    case 3: {
      kr_decklink_info_random(&st->dl);
      break;
    }
    case 4: {
      kr_x11_info_random(&st->x11);
      break;
    }
    case 7: {
      kr_jack_path_info_random(&st->jack_in);
      break;
    }
    case 8: {
      kr_jack_path_info_random(&st->jack_out);
      break;
    }
    case 9: {
      kr_wayland_path_info_random(&st->wl_out);
      break;
    }
    case 10: {
      kr_v4l2_open_info_random(&st->v4l2_in);
      break;
    }
    case 11: {
      kr_decklink_path_info_random(&st->dl_in);
      break;
    }
    case 12: {
      kr_x11_path_info_random(&st->x11_in);
      break;
    }
    case 13: {
      kr_alsa_path_info_random(&st->alsa_in);
      break;
    }
    case 14: {
      kr_alsa_path_info_random(&st->alsa_out);
      break;
    }
  }


  return -1;
}

int kr_xpdr_path_info_init(struct kr_xpdr_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_xpdr_path_info));

  return 0;
}

int kr_xpdr_path_info_valid(struct kr_xpdr_path_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_xpdr_path_info_random(struct kr_xpdr_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_xpdr_path_info));

  return 0;
}

