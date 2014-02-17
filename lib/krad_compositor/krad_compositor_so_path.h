#ifndef KRAD_COMPOSITOR_SO_PATH_H
#define KRAD_COMPOSITOR_SO_PATH_H

typedef struct kr_compositor_port_setup kr_compositor_port_setup;

struct kr_compositor_port_setup {
  kr_compositor_path_info info;
  void *frame_user;
  void *control_user;
};

kr_compositor_path *kr_compositor_port(kr_compositor *comp,
 kr_compositor_port_setup *setup);

int kr_compositor_frame(kr_compositor_path *path, kr_image *image);

#endif
