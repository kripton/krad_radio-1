#ifndef KRAD_COMPOSITOR_PATH_H
#define KRAD_COMPOSITOR_PATH_H

#include "krad_av.h"
#include "krad_convert.h"
#include "krad_perspective.h"

typedef struct kr_compositor_path kr_compositor_path;

int kr_compositor_process(kr_compositor_path *path);
int kr_compositor_ctl(kr_compositor_path *path, kr_compositor_path_info_patch *patch);
int kr_compositor_remove(kr_compositor_path *path);
int kr_compositor_bus(kr_compositor *c, kr_compositor_path_info *i, void *user);
int kr_compositor_link(kr_compositor_path *output, kr_compositor_path *from,
  kr_compositor_path_info *info, void *user);
int kr_compositor_path_info_get(kr_compositor_path *path,
 kr_compositor_path_info *info);
size_t kr_compositor_path_size();

#endif
