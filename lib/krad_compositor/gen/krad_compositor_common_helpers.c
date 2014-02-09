#include "krad_compositor_common_helpers.h"

kr_compositor_output_info_member kr_compositor_output_info_strto_member(char *string, int len) {
  if (!strncmp(string,"w",len)) {
    return KR_COMPOSITOR_OUTPUT_INFO_W;
  }
  if (!strncmp(string,"h",len)) {
    return KR_COMPOSITOR_OUTPUT_INFO_H;
  }
  if (!strncmp(string,"opacity",len)) {
    return KR_COMPOSITOR_OUTPUT_INFO_OPACITY;
  }
  return -1;
}

kr_compositor_bus_info_member kr_compositor_bus_info_strto_member(char *string, int len) {
  if (!strncmp(string,"opacity",len)) {
    return KR_COMPOSITOR_BUS_INFO_OPACITY;
  }
  return -1;
}

kr_compositor_input_info_member kr_compositor_input_info_strto_member(char *string, int len) {
  if (!strncmp(string,"crop",len)) {
    return KR_COMPOSITOR_INPUT_INFO_CROP;
  }
  if (!strncmp(string,"pos",len)) {
    return KR_COMPOSITOR_INPUT_INFO_POS;
  }
  if (!strncmp(string,"view",len)) {
    return KR_COMPOSITOR_INPUT_INFO_VIEW;
  }
  if (!strncmp(string,"z",len)) {
    return KR_COMPOSITOR_INPUT_INFO_Z;
  }
  if (!strncmp(string,"rotation",len)) {
    return KR_COMPOSITOR_INPUT_INFO_ROTATION;
  }
  if (!strncmp(string,"opacity",len)) {
    return KR_COMPOSITOR_INPUT_INFO_OPACITY;
  }
  return -1;
}

kr_compositor_source_info_member kr_compositor_source_info_strto_member(char *string, int len) {
  if (!strncmp(string,"w",len)) {
    return KR_COMPOSITOR_SOURCE_INFO_W;
  }
  if (!strncmp(string,"h",len)) {
    return KR_COMPOSITOR_SOURCE_INFO_H;
  }
  return -1;
}

kr_overlay_info_member kr_overlay_info_strto_member(char *string, int len) {
  if (!strncmp(string,"type",len)) {
    return KR_OVERLAY_INFO_TYPE;
  }
  if (!strncmp(string,"info",len)) {
    return KR_OVERLAY_INFO_INFO;
  }
  return -1;
}

kr_compositor_path_info_member kr_compositor_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"type",len)) {
    return KR_COMPOSITOR_PATH_INFO_TYPE;
  }
  if (!strncmp(string,"info",len)) {
    return KR_COMPOSITOR_PATH_INFO_INFO;
  }
  return -1;
}

int kr_compositor_path_type_to_index(int val) {
  switch (val) {
    case KR_COM_SOURCE:
      return 0;
    case KR_COM_OVERLAY:
      return 1;
    case KR_COM_INPUT:
      return 2;
    case KR_COM_BUS:
      return 3;
    case KR_COM_OUTPUT:
      return 4;
  }
  return -1;
}

char *kr_strfr_kr_compositor_path_type(int val) {
  switch (val) {
    case KR_COM_SOURCE:
      return "kr_com_source";
    case KR_COM_OVERLAY:
      return "kr_com_overlay";
    case KR_COM_INPUT:
      return "kr_com_input";
    case KR_COM_BUS:
      return "kr_com_bus";
    case KR_COM_OUTPUT:
      return "kr_com_output";
  }
  return NULL;
}

int kr_strto_kr_compositor_path_type(char *string) {
  if (!strcmp(string,"kr_com_source")) {
    return KR_COM_SOURCE;
  }
  if (!strcmp(string,"kr_com_overlay")) {
    return KR_COM_OVERLAY;
  }
  if (!strcmp(string,"kr_com_input")) {
    return KR_COM_INPUT;
  }
  if (!strcmp(string,"kr_com_bus")) {
    return KR_COM_BUS;
  }
  if (!strcmp(string,"kr_com_output")) {
    return KR_COM_OUTPUT;
  }
  return -1;
}

int kr_compositor_overlay_type_to_index(int val) {
  switch (val) {
    case KR_SPRITE:
      return 0;
    case KR_TEXT:
      return 1;
    case KR_VECTOR:
      return 2;
  }
  return -1;
}

char *kr_strfr_kr_compositor_overlay_type(int val) {
  switch (val) {
    case KR_SPRITE:
      return "kr_sprite";
    case KR_TEXT:
      return "kr_text";
    case KR_VECTOR:
      return "kr_vector";
  }
  return NULL;
}

int kr_strto_kr_compositor_overlay_type(char *string) {
  if (!strcmp(string,"kr_sprite")) {
    return KR_SPRITE;
  }
  if (!strcmp(string,"kr_text")) {
    return KR_TEXT;
  }
  if (!strcmp(string,"kr_vector")) {
    return KR_VECTOR;
  }
  return -1;
}

int kr_compositor_output_info_patch_apply(struct kr_compositor_output_info *info, kr_compositor_output_info_patch *patch) {
  const ptrdiff_t off[3] = { offsetof(struct kr_compositor_output_info, w), 
    offsetof(struct kr_compositor_output_info, h), offsetof(struct kr_compositor_output_info, opacity)
  };
  const size_t sz[3] = { sizeof(info->w), 
    sizeof(info->h), sizeof(info->opacity)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_compositor_output_info_patch_path(kr_compositor_output_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_compositor_output_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_compositor_bus_info_patch_apply(struct kr_compositor_bus_info *info, kr_compositor_bus_info_patch *patch) {
  const ptrdiff_t off[1] = { offsetof(struct kr_compositor_bus_info, opacity)
  };
  const size_t sz[1] = { sizeof(info->opacity)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_compositor_bus_info_patch_path(kr_compositor_bus_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_compositor_bus_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_compositor_input_info_patch_apply(struct kr_compositor_input_info *info, kr_compositor_input_info_patch *patch) {
  const ptrdiff_t off[6] = { offsetof(struct kr_compositor_input_info, crop), 
    offsetof(struct kr_compositor_input_info, pos), offsetof(struct kr_compositor_input_info, view), 
    offsetof(struct kr_compositor_input_info, z), offsetof(struct kr_compositor_input_info, rotation), 
    offsetof(struct kr_compositor_input_info, opacity)
  };
  const size_t sz[6] = { sizeof(info->crop), 
    sizeof(info->pos), sizeof(info->view), 
    sizeof(info->z), sizeof(info->rotation), 
    sizeof(info->opacity)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_compositor_input_info_patch_path(kr_compositor_input_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_compositor_input_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_compositor_source_info_patch_apply(struct kr_compositor_source_info *info, kr_compositor_source_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_compositor_source_info, w), 
    offsetof(struct kr_compositor_source_info, h)
  };
  const size_t sz[2] = { sizeof(info->w), 
    sizeof(info->h)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_compositor_source_info_patch_path(kr_compositor_source_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_compositor_source_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_overlay_info_patch_apply(struct kr_overlay_info *info, kr_overlay_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_overlay_info, type), 
    offsetof(struct kr_overlay_info, info)
  };
  const size_t sz[2] = { sizeof(info->type), 
    sizeof(info->info)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_overlay_info_patch_path(kr_overlay_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_overlay_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_compositor_path_info_patch_apply(struct kr_compositor_path_info *info, kr_compositor_path_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(struct kr_compositor_path_info, type), 
    offsetof(struct kr_compositor_path_info, info)
  };
  const size_t sz[2] = { sizeof(info->type), 
    sizeof(info->info)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_compositor_path_info_patch_path(kr_compositor_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_compositor_path_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_rect_init(struct kr_rect *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_rect_valid(struct kr_rect *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_rect_random(struct kr_rect *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_output_info_init(struct kr_compositor_output_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_output_info_valid(struct kr_compositor_output_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_output_info_random(struct kr_compositor_output_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_bus_info_init(struct kr_compositor_bus_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_bus_info_valid(struct kr_compositor_bus_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_bus_info_random(struct kr_compositor_bus_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_input_info_init(struct kr_compositor_input_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_compositor_input_info));
  kr_rect_init(&st->crop);
  kr_rect_init(&st->pos);
  kr_perspective_view_init(&st->view);

  return 0;
}

int kr_compositor_input_info_valid(struct kr_compositor_input_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_rect_valid(&st->crop);
  kr_rect_valid(&st->pos);
  kr_perspective_view_valid(&st->view);

  return 0;
}

int kr_compositor_input_info_random(struct kr_compositor_input_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_compositor_input_info));
  kr_rect_random(&st->crop);
  kr_rect_random(&st->pos);
  kr_perspective_view_random(&st->view);

  return 0;
}

int kr_compositor_source_info_init(struct kr_compositor_source_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_source_info_valid(struct kr_compositor_source_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_source_info_random(struct kr_compositor_source_info *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_overlay_type_info_init(kr_overlay_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_overlay_type_info));
  switch (idx) {
    case 0: {
      kr_text_info_init(&st->text);
      break;
    }
    case 1: {
      kr_vector_info_init(&st->vector);
      break;
    }
    case 2: {
      kr_sprite_info_init(&st->sprite);
      break;
    }
  }


  return -1;
}

int kr_overlay_type_info_valid(kr_overlay_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  switch (idx) {
    case 0: {
      kr_text_info_valid(&st->text);
      break;
    }
    case 1: {
      kr_vector_info_valid(&st->vector);
      break;
    }
    case 2: {
      kr_sprite_info_valid(&st->sprite);
      break;
    }
  }


  return -1;
}

int kr_overlay_type_info_random(kr_overlay_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_overlay_type_info));
  switch (idx) {
    case 0: {
      kr_text_info_random(&st->text);
      break;
    }
    case 1: {
      kr_vector_info_random(&st->vector);
      break;
    }
    case 2: {
      kr_sprite_info_random(&st->sprite);
      break;
    }
  }


  return -1;
}

int kr_overlay_info_init(struct kr_overlay_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_overlay_info));
  kr_overlay_type_info_init(&st->info,kr_compositor_overlay_type_to_index(st->type));

  return 0;
}

int kr_overlay_info_valid(struct kr_overlay_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_overlay_type_info_valid(&st->info,kr_compositor_overlay_type_to_index(st->type));

  return 0;
}

int kr_overlay_info_random(struct kr_overlay_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_overlay_info));
  kr_overlay_type_info_random(&st->info,kr_compositor_overlay_type_to_index(st->type));

  return 0;
}

int kr_compositor_path_type_info_init(kr_compositor_path_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_compositor_path_type_info));
  switch (idx) {
    case 0: {
      kr_compositor_output_info_init(&st->output_info);
      break;
    }
    case 1: {
      kr_compositor_bus_info_init(&st->bus_info);
      break;
    }
    case 2: {
      kr_compositor_input_info_init(&st->input_info);
      break;
    }
    case 3: {
      kr_compositor_source_info_init(&st->source_info);
      break;
    }
  }


  return -1;
}

int kr_compositor_path_type_info_valid(kr_compositor_path_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  switch (idx) {
    case 0: {
      kr_compositor_output_info_valid(&st->output_info);
      break;
    }
    case 1: {
      kr_compositor_bus_info_valid(&st->bus_info);
      break;
    }
    case 2: {
      kr_compositor_input_info_valid(&st->input_info);
      break;
    }
    case 3: {
      kr_compositor_source_info_valid(&st->source_info);
      break;
    }
  }


  return -1;
}

int kr_compositor_path_type_info_random(kr_compositor_path_type_info *st, int idx) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_compositor_path_type_info));
  switch (idx) {
    case 0: {
      kr_compositor_output_info_random(&st->output_info);
      break;
    }
    case 1: {
      kr_compositor_bus_info_random(&st->bus_info);
      break;
    }
    case 2: {
      kr_compositor_input_info_random(&st->input_info);
      break;
    }
    case 3: {
      kr_compositor_source_info_random(&st->source_info);
      break;
    }
  }


  return -1;
}

int kr_compositor_path_info_init(struct kr_compositor_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_compositor_path_info));
  kr_compositor_path_type_info_init(&st->info,kr_compositor_path_type_to_index(st->type));

  return 0;
}

int kr_compositor_path_info_valid(struct kr_compositor_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_compositor_path_type_info_valid(&st->info,kr_compositor_path_type_to_index(st->type));

  return 0;
}

int kr_compositor_path_info_random(struct kr_compositor_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_compositor_path_info));
  kr_compositor_path_type_info_random(&st->info,kr_compositor_path_type_to_index(st->type));

  return 0;
}

