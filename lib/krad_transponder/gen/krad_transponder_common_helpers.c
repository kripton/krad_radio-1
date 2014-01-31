#include "krad_transponder_common_helpers.h"

int krad_link_av_mode_t_to_index(int val) {
  switch (val) {
    case AUDIO_ONLY:
      return 0;
    case VIDEO_ONLY:
      return 1;
    case AUDIO_AND_VIDEO:
      return 2;
  }
  return -1;
}

char *kr_strfr_krad_link_av_mode_t(int val) {
  switch (val) {
    case AUDIO_ONLY:
      return "audio_only";
    case VIDEO_ONLY:
      return "video_only";
    case AUDIO_AND_VIDEO:
      return "audio_and_video";
  }
  return NULL;
}

int kr_strto_krad_link_av_mode_t(char *string) {
  if (!strcmp(string,"audio_only")) {
    return AUDIO_ONLY;
  }
  if (!strcmp(string,"video_only")) {
    return VIDEO_ONLY;
  }
  if (!strcmp(string,"audio_and_video")) {
    return AUDIO_AND_VIDEO;
  }

  return -1;
}

int kr_txpdr_su_type_t_to_index(int val) {
  switch (val) {
    case DEMUX:
      return 0;
    case DECODE:
      return 1;
    case MUX:
      return 2;
    case ENCODE:
      return 3;
    case RAWIN:
      return 4;
    case RAWOUT:
      return 5;
    case FAILURE:
      return 6;
  }
  return -1;
}

char *kr_strfr_kr_txpdr_su_type_t(int val) {
  switch (val) {
    case DEMUX:
      return "demux";
    case DECODE:
      return "decode";
    case MUX:
      return "mux";
    case ENCODE:
      return "encode";
    case RAWIN:
      return "rawin";
    case RAWOUT:
      return "rawout";
    case FAILURE:
      return "failure";
  }
  return NULL;
}

int kr_strto_kr_txpdr_su_type_t(char *string) {
  if (!strcmp(string,"demux")) {
    return DEMUX;
  }
  if (!strcmp(string,"decode")) {
    return DECODE;
  }
  if (!strcmp(string,"mux")) {
    return MUX;
  }
  if (!strcmp(string,"encode")) {
    return ENCODE;
  }
  if (!strcmp(string,"rawin")) {
    return RAWIN;
  }
  if (!strcmp(string,"rawout")) {
    return RAWOUT;
  }
  if (!strcmp(string,"failure")) {
    return FAILURE;
  }

  return -1;
}

int krad_link_transport_mode_t_to_index(int val) {
  switch (val) {
    case TCP:
      return 0;
    case UDP:
      return 1;
    case FILESYSTEM:
      return 2;
    case FAIL:
      return 3;
  }
  return -1;
}

char *kr_strfr_krad_link_transport_mode_t(int val) {
  switch (val) {
    case TCP:
      return "tcp";
    case UDP:
      return "udp";
    case FILESYSTEM:
      return "filesystem";
    case FAIL:
      return "fail";
  }
  return NULL;
}

int kr_strto_krad_link_transport_mode_t(char *string) {
  if (!strcmp(string,"tcp")) {
    return TCP;
  }
  if (!strcmp(string,"udp")) {
    return UDP;
  }
  if (!strcmp(string,"filesystem")) {
    return FILESYSTEM;
  }
  if (!strcmp(string,"fail")) {
    return FAIL;
  }

  return -1;
}

int kr_transponder_path_io_type_to_index(int val) {
  switch (val) {
    case KR_XPDR_MIXER:
      return 0;
    case KR_XPDR_COMPOSITOR:
      return 1;
    case KR_XPDR_ADAPTER:
      return 2;
  }
  return -1;
}

char *kr_strfr_kr_transponder_path_io_type(int val) {
  switch (val) {
    case KR_XPDR_MIXER:
      return "kr_xpdr_mixer";
    case KR_XPDR_COMPOSITOR:
      return "kr_xpdr_compositor";
    case KR_XPDR_ADAPTER:
      return "kr_xpdr_adapter";
  }
  return NULL;
}

int kr_strto_kr_transponder_path_io_type(char *string) {
  if (!strcmp(string,"kr_xpdr_mixer")) {
    return KR_XPDR_MIXER;
  }
  if (!strcmp(string,"kr_xpdr_compositor")) {
    return KR_XPDR_COMPOSITOR;
  }
  if (!strcmp(string,"kr_xpdr_adapter")) {
    return KR_XPDR_ADAPTER;
  }

  return -1;
}

int kr_transponder_info_patch_apply(struct kr_transponder_info *info, kr_transponder_info_patch *patch) {
  const ptrdiff_t off[1] = { offsetof(struct kr_transponder_info, active_paths)
  };
  const size_t sz[1] = { sizeof(info->active_paths)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_transponder_path_io_info_patch_apply(struct kr_transponder_path_io_info *info, kr_transponder_path_io_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_transponder_path_io_info, type), 
    offsetof(struct kr_transponder_path_io_info, info)
  };
  const size_t sz[2] = { sizeof(info->type), 
    sizeof(info->info)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_transponder_path_info_patch_apply(struct kr_transponder_path_info *info, kr_transponder_path_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_transponder_path_info, input), 
    offsetof(struct kr_transponder_path_info, output)
  };
  const size_t sz[2] = { sizeof(info->input), 
    sizeof(info->output)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_transponder_info_init(struct kr_transponder_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_transponder_info_valid(struct kr_transponder_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_transponder_info_random(struct kr_transponder_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_transponder_path_io_path_info_init(kr_transponder_path_io_path_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_transponder_path_io_path_info));
  switch (idx) {
    case 0: {
      kr_mixer_path_info_init(&st->mixer_path_info);
      break;
    }
    case 1: {
      kr_compositor_path_info_init(&st->compositor_path_info);
      break;
    }
    case 2: {
      kr_adapter_path_info_init(&st->adapter_path_info);
      break;
    }
  }


  return -1;
}

int kr_transponder_path_io_path_info_valid(kr_transponder_path_io_path_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  switch (idx) {
    case 0: {
      kr_mixer_path_info_valid(&st->mixer_path_info);
      break;
    }
    case 1: {
      kr_compositor_path_info_valid(&st->compositor_path_info);
      break;
    }
    case 2: {
      kr_adapter_path_info_valid(&st->adapter_path_info);
      break;
    }
  }


  return -1;
}

int kr_transponder_path_io_path_info_random(kr_transponder_path_io_path_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_transponder_path_io_path_info));
  switch (idx) {
    case 0: {
      kr_mixer_path_info_random(&st->mixer_path_info);
      break;
    }
    case 1: {
      kr_compositor_path_info_random(&st->compositor_path_info);
      break;
    }
    case 2: {
      kr_adapter_path_info_random(&st->adapter_path_info);
      break;
    }
  }


  return -1;
}

int kr_transponder_path_io_info_init(struct kr_transponder_path_io_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_transponder_path_io_info));
  kr_transponder_path_io_path_info_init(&st->info,kr_transponder_path_io_type_to_index(st->type));

  return 0;
}

int kr_transponder_path_io_info_valid(struct kr_transponder_path_io_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_transponder_path_io_path_info_valid(&st->info,kr_transponder_path_io_type_to_index(st->type));

  return 0;
}

int kr_transponder_path_io_info_random(struct kr_transponder_path_io_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_transponder_path_io_info));
  kr_transponder_path_io_path_info_random(&st->info,kr_transponder_path_io_type_to_index(st->type));

  return 0;
}

int kr_transponder_path_info_init(struct kr_transponder_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_transponder_path_info));
  kr_transponder_path_io_info_init(&st->input);
  kr_transponder_path_io_info_init(&st->output);

  return 0;
}

int kr_transponder_path_info_valid(struct kr_transponder_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_transponder_path_io_info_valid(&st->input);
  kr_transponder_path_io_info_valid(&st->output);

  return 0;
}

int kr_transponder_path_info_random(struct kr_transponder_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_transponder_path_info));
  kr_transponder_path_io_info_random(&st->input);
  kr_transponder_path_io_info_random(&st->output);

  return 0;
}

int kr_transponder_path_patch_init(struct kr_transponder_path_patch *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_transponder_path_patch_valid(struct kr_transponder_path_patch *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_transponder_path_patch_random(struct kr_transponder_path_patch *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

