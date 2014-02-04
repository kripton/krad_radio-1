#ifndef KRAD_COMPOSITOR_SO_PATH_H
#define KRAD_COMPOSITOR_SO_PATH_H

typedef struct kr_compositor_io_path_setup kr_compositor_io_path_setup;
typedef struct kr_compositor_path_frame_cb_arg kr_compositor_path_frame_cb_arg;
typedef void (kr_compositor_path_frame_cb)(kr_compositor_path_frame_cb_arg *);

struct kr_compositor_path_frame_cb_arg {
  kr_image image;
  void *user;
};

struct kr_compositor_io_path_setup {
  kr_compositor_path_info info;
  void *frame_user;
  void *control_user;
  kr_compositor_path_frame_cb *frame_cb;
};

kr_compositor_path *kr_compositor_mkso(kr_compositor *compositor,
 kr_compositor_io_path_setup *setup);

#endif
