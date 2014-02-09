#ifndef KRAD_COMPOSITOR_SO_PATH_H
#define KRAD_COMPOSITOR_SO_PATH_H

typedef struct kr_compositor_port_setup kr_compositor_port_setup;
typedef struct kr_frame_event kr_frame_event;
typedef void (kr_frame_cb)(kr_frame_event *);

struct kr_frame_event {
  kr_image image;
  void *user;
};

struct kr_compositor_port_setup {
  kr_compositor_path_info info;
  void *frame_user;
  void *control_user;
  kr_frame_cb *frame_cb;
};

kr_compositor_path *kr_compositor_port(kr_compositor *comp,
 kr_compositor_port_setup *setup);

#endif
