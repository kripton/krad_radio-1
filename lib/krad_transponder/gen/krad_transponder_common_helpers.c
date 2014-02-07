#include "krad_transponder_common_helpers.h"

kr_transponder_path_io_info_member kr_transponder_path_io_info_strto_member(char *string, int len) {
  if (!strncmp(string,"type",len)) {
    return KR_TRANSPONDER_PATH_IO_INFO_TYPE;
  }
  if (!strncmp(string,"info",len)) {
    return KR_TRANSPONDER_PATH_IO_INFO_INFO;
  }
  return -1;
}

kr_transponder_path_info_member kr_transponder_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"input",len)) {
    return KR_TRANSPONDER_PATH_INFO_INPUT;
  }
  if (!strncmp(string,"output",len)) {
    return KR_TRANSPONDER_PATH_INFO_OUTPUT;
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

int kr_transponder_path_io_info_patch_apply(struct kr_transponder_path_io_info *info, kr_transponder_path_io_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_transponder_path_io_info, type), 
    offsetof(struct kr_transponder_path_io_info, info)
  };
  const size_t sz[2] = { sizeof(info->type), 
    sizeof(info->info)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_transponder_path_io_info_patch_path(kr_transponder_path_io_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_transponder_path_io_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
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

kr_var *kr_transponder_path_info_patch_path(kr_transponder_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_transponder_path_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    case KR_TRANSPONDER_PATH_INFO_INPUT:
       if (kr_path_step(path) != 0) return NULL;
       return kr_transponder_path_io_info_patch_path(&patch->value.input_patch, path);
    case KR_TRANSPONDER_PATH_INFO_OUTPUT:
       if (kr_path_step(path) != 0) return NULL;
       return kr_transponder_path_io_info_patch_path(&patch->value.output_patch, path);
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
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

