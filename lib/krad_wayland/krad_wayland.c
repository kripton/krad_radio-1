#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <errno.h>
#include <signal.h>
#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>
#include "krad_system.h"
#include "krad_wayland.h"

#define KR_WL_MAX_WINDOWS 4
#define KR_WL_BUFFER_COUNT 1

typedef struct kr_wayland kr_wayland;
typedef struct kr_wayland_path kr_wayland_path;

enum kr_wayland_event_type {
  KR_WL_FRAME,
  KR_WL_POINTER,
  KR_WL_KEY
};

typedef struct {
  int x;
  int y;
  int click;
  int pointer_in;
  int pointer_out;
} kr_wayland_pointer_event;

typedef struct {
  int key;
  int down;
} kr_wayland_key_event;

typedef struct {
  uint8_t *buffer;
} kr_wayland_frame_event;

typedef struct {
  int type;
  kr_wayland_pointer_event pointer_event;
  kr_wayland_key_event key_event;
  kr_wayland_frame_event frame_event;
} kr_wayland_event;

typedef struct {
  kr_wayland_path_info info;
  int (*callback)(void *, kr_wayland_event *);
  void *user;
} kr_wayland_path_setup;

typedef struct {
  kr_wayland_info info;
} kr_wayland_setup;

struct kr_wayland_path {
  int width;
  int height;
  int pointer_x;
  int pointer_y;
  int click;
  int pointer_in;
  int pointer_out;
  int active;
  int fullscreen;
  struct wl_surface *surface;
  struct wl_shell_surface *shell_surface;
  struct wl_buffer *buffer;
  void *shm_data;
  struct wl_callback *callback;
  struct wl_shell_surface_listener surface_listener;
  struct wl_callback_listener frame_listener;
  int current_buffer;
  int frame_size;
  struct wl_buffer *buffers[KR_WL_BUFFER_COUNT];
  int (*user_callback)(void *, kr_wayland_event *);
  void *user;
  kr_wayland *wayland;
};

struct kr_wayland {
  int fd;
  kr_wayland_path window[KR_WL_MAX_WINDOWS];
  kr_wayland_path *key_window;
  kr_wayland_path *pointer_window;
  struct wl_display *display;
  struct wl_registry *registry;
  struct wl_compositor *compositor;
  struct wl_shell *shell;
  struct wl_shm *shm;
  uint32_t formats;
  struct wl_seat *seat;
  struct wl_pointer *pointer;
  struct wl_keyboard *keyboard;
  struct wl_shm_listener shm_listener;
  struct wl_seat_listener seat_listener;
  struct wl_pointer_listener pointer_listener;
  struct wl_keyboard_listener keyboard_listener;
  struct wl_registry_listener registry_listener;
  struct {
    struct xkb_context *context;
    struct xkb_keymap *keymap;
    struct xkb_state *state;
    xkb_mod_mask_t control_mask;
    xkb_mod_mask_t alt_mask;
    xkb_mod_mask_t shift_mask;
  } xkb;
  kr_wayland_info info;
};

static void handle_configure(void *data, struct wl_shell_surface *ss,
 uint32_t edges, int32_t width, int32_t height);
static void handle_popup_done(void *data, struct wl_shell_surface *sh_surface);
static void handle_ping(void *data, struct wl_shell_surface *sh_surface,
 uint32_t serial);
static void handle_shm_format(void *data, struct wl_shm *wl_shm, uint32_t format);
static void handle_global(void *data, struct wl_registry *registry,
 uint32_t id, const char *interface, uint32_t version);
static void kw_frame_listener(void *data, struct wl_callback *callback,
 uint32_t time);

#include "input.c"

static void handle_configure(void *data, struct wl_shell_surface *ss,
 uint32_t edges, int32_t width, int32_t height) {
  /* Nothing here */
}

static void handle_popup_done(void *data,
 struct wl_shell_surface *shell_surface) {
  /* Nothing here */
}

static void handle_ping(void *data, struct wl_shell_surface *ss, uint32_t serial) {
  wl_shell_surface_pong(ss, serial);
}

static void handle_shm_format(void *data, struct wl_shm *wl_shm, uint32_t format) {
  kr_wayland *kw;
  kw = (kr_wayland *)data;
  kw->formats |= (1 << format);
}

static void handle_global(void *data, struct wl_registry *registry,
 uint32_t id, const char *interface, uint32_t version) {
  kr_wayland *kw;
  kw = (kr_wayland *)data;
  if (strcmp(interface, "wl_compositor") == 0) {
    kw->compositor = wl_registry_bind(kw->registry, id,
     &wl_compositor_interface, 1);
  } else if (strcmp(interface, "wl_shell") == 0) {
    kw->shell = wl_registry_bind(kw->registry, id, &wl_shell_interface, 1);
  } else if (strcmp(interface, "wl_seat") == 0) {
    kw->seat = wl_registry_bind(kw->registry, id, &wl_seat_interface, 1);
    wl_seat_add_listener(kw->seat, &kw->seat_listener, kw);
  } else if (strcmp(interface, "wl_shm") == 0) {
    kw->shm = wl_registry_bind(kw->registry, id, &wl_shm_interface, 1);
    wl_shm_add_listener(kw->shm, &kw->shm_listener, kw);
  }
}

/*
static int kw_create_shm_buffer(kr_wayland_path *window,
 int width, int height, int frames, uint32_t format, void **data_out) {
  char filename[] = "/tmp/wayland-shm-XXXXXX";
  struct wl_shm_pool *pool;
  int fd;
  int size;
  int stride;
  void *data;
  int b;
  b = 0;
  fd = mkstemp(filename);
  if (fd < 0) {
    fprintf(stderr, "open %s failed: %m\n", filename);
    return -1;
  }
  stride = width * 4;
  window->frame_size = stride * height;
  size = window->frame_size * frames;
  if (ftruncate(fd, size) < 0) {
    fprintf (stderr, "ftruncate failed: %m\n");
    close(fd);
    return -1;
  }
  data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  unlink(filename);
  if (data == MAP_FAILED) {
    fprintf(stderr, "mmap failed: %m\n");
    close(fd);
    return -1;
  }
  pool = wl_shm_create_pool(window->wayland->shm, fd, size);
  for (b = 0; b < KR_WL_BUFFER_COUNT; b++) {
    window->buffers[b] = wl_shm_pool_create_buffer(pool,
     b * window->frame_size, width, height, stride, format);
  }
  wl_shm_pool_destroy(pool);
  close(fd);
  *data_out = data;
  return 0;
}

static void kw_frame_listener(void *data,
 struct wl_callback *callback, uint32_t time) {
  kr_wayland_path *window;
  kr_wayland_event wayland_event;
  int updated;
  window = data;
  updated = 0;
  memset(&wayland_event, 0, sizeof(kr_wayland_event));
  if (window->user_callback != NULL) {
    wayland_event.type = KR_WL_FRAME;
    wayland_event.frame_event.buffer = (uint8_t *)window->shm_data;
    updated = window->user_callback(window->user, &wayland_event);
  }
  wl_surface_attach(window->surface, window->buffer, 0, 0);
  if (updated) {
    wl_surface_damage(window->surface, 0, 0, window->width, window->height);
  } else {
    wl_surface_damage(window->surface, 0, 0, 10, 10);
  }
  if (callback) {
    wl_callback_destroy(callback);
  }
  window->callback = wl_surface_frame(window->surface);
  window->frame_listener.done = kw_frame_listener;
  wl_callback_add_listener(window->callback, &window->frame_listener,
   window);
  wl_surface_commit(window->surface);
}

kr_wayland_path *kr_wayland_mkpath(kr_wayland *wayland,
 kr_wayland_path_setup *setup) {
  kr_wayland_path *window;
  struct wl_region *opaque;
  int ret;
  int i;
  if ((wayland == NULL) || (setup == NULL)) {
    return NULL;
  }
  if ((setup->info.width == 0) || (setup->info.height == 0) ||
      (setup->info.width > 8192) || (setup->info.height > 8192)
      || setup->callback == NULL) {
    return NULL;
  }
  for (i = 0; i < KR_WL_MAX_WINDOWS; i++) {
    if (wayland->window[i].active == 0) {
      break;
    }
  }
  if (i == KR_WL_MAX_WINDOWS) {
    return NULL;
  }
  window = &wayland->window[i];
  window->wayland = wayland;
  window->user_callback = setup->callback;
  window->user = setup->user;
  window->width = setup->info.width;
  window->height = setup->info.height;
  window->fullscreen = setup->info.fullscreen;
  ret = kw_create_shm_buffer(window, window->width,
   window->height, KR_WL_BUFFER_COUNT, WL_SHM_FORMAT_XRGB8888,
   &window->shm_data);
  if (ret != 0) {
    wayland->window[i].active = 0;
    return NULL;
  }
  window->current_buffer = 0;
  window->buffer = window->buffers[window->current_buffer];
  window->surface_listener.ping = handle_ping;
  window->surface_listener.configure = handle_configure;
  window->surface_listener.popup_done = handle_popup_done;
  window->surface = wl_compositor_create_surface(wayland->compositor);
  window->shell_surface = wl_shell_get_shell_surface(wayland->shell,
   window->surface);
  opaque = wl_compositor_create_region(wayland->compositor);
  wl_region_add(opaque, 0, 0, window->width, window->height);
  wl_surface_set_opaque_region(window->surface, opaque);
  wl_region_destroy(opaque);
  *//*wl_shell_surface_set_title(wayland->window->shell_surface,
   wayland->window->title);*//*
   wl_shell_surface_add_listener(window->shell_surface,
    &window->surface_listener, window);
  wl_shell_surface_set_toplevel(window->shell_surface);
  kw_frame_listener(window, NULL, 0);
  wl_display_roundtrip(wayland->display);
  if (window->fullscreen == 1) {
    wl_shell_surface_set_fullscreen(window->shell_surface, 1, 0, NULL);
  }
  wayland->window[i].active = 1;
  return window;
}

int kr_wayland_unlink(kr_wayland_path **win) {
  int i;
  kr_wayland_path *window;
  kr_wayland *wayland;
  if ((win == NULL) || (*win == NULL)) {
    return -1;
  }
  window = *win;
  wayland = window->wayland;
  for (i = 0; i < KR_WL_MAX_WINDOWS; i++) {
    if (&wayland->window[i] == window) {
      break;
    }
  }
  wl_display_sync(wayland->display);
  if (window->callback) {
    wl_callback_destroy(window->callback);
  }
  wl_display_sync(wayland->display);
  wl_buffer_destroy(window->buffer);
  wl_shell_surface_destroy(window->shell_surface);
  wl_surface_destroy(window->surface);
  wl_display_sync(wayland->display);
  wayland->window[i].active = 0;
  *win = NULL;
  return 0;
}
*/

int kr_wayland_handle_out(kr_wayland *kw) {
  int ret;
  while (wl_display_prepare_read(kw->display) != 0) {
    ret = wl_display_dispatch_pending(kw->display);
    if (ret == -1) {
      printke("Wayland: Error on dispatch pending");
    }
  }
  ret = wl_display_flush(kw->display);
  if ((ret == -1) && (errno != EAGAIN)) {
    ret = errno;
    printke("Wayland: Error on display flush: %s", strerror(ret));
    return -1;
  }
  /* Can poll now */
  return 0;
}

int kr_wayland_handle_in(kr_wayland *kw) {
  int ret;
  ret = wl_display_read_events(kw->display);
  if (ret == -1) {
    ret = errno;
    printke("Wayland: Error on read events: %s", strerror(ret));
    return -1;
  }
  ret = wl_display_dispatch_pending(kw->display);
  if (ret == -1) {
    printke("Wayland: Error on dispatch pending");
    return -1;
  }
  return 0;
}

void kw_teardown(kr_wayland *kw) {
  if (kw == NULL) return;
  if (kw->pointer != NULL) {
    wl_pointer_destroy(kw->pointer);
    kw->pointer = NULL;
  }
  if (kw->keyboard != NULL) {
    wl_keyboard_destroy(kw->keyboard);
    kw->keyboard = NULL;
  }
  if (kw->xkb.state) {
    xkb_state_unref(kw->xkb.state);
    kw->xkb.state = NULL;
  }
  if (kw->xkb.keymap) {
    xkb_map_unref(kw->xkb.keymap);
    kw->xkb.keymap = NULL;
  }
  if (kw->seat) {
    wl_seat_destroy(kw->seat);
    kw->seat = NULL;
  }
  if (kw->xkb.context) {
    xkb_context_unref(kw->xkb.context);
    kw->xkb.context = NULL;
  }
  if (kw->shm) {
    wl_shm_destroy(kw->shm);
    kw->shm = NULL;
  }
  if (kw->shell) {
    wl_shell_destroy(kw->shell);
    kw->shell = NULL;
  }
  if (kw->compositor) {
    wl_compositor_destroy(kw->compositor);
    kw->compositor = NULL;
  }
  if (kw->registry) {
    wl_registry_destroy(kw->registry);
    kw->registry = NULL;
  }
  if (kw->display) {
    /* FIXME We only want to run flush if !EPOLLERR && !EPOLLHUP on the fd */
    wl_display_flush(kw->display);
    wl_display_disconnect(kw->display);
    kw->display = NULL;
  }
  kw->info.state = KR_WL_DISCONNECTED;
}

int kr_wl_close(kr_adapter *adp) {
  kr_wayland *kw;
  if (adp == NULL) return -1;
  printk("Wayland: Adapter Closing");
  kw = (kr_wayland *)adp->handle;
  kw_teardown(kw);
  free(kw);
  adp->handle = NULL;
  printk("Wayland: Adapter Closed");
  return 0;
}

static void kw_connect(kr_wayland *kw) {
  char *display_name;
  display_name = NULL;
  if (strnlen(kw->info.display_name, sizeof(kw->info.display_name))) {
    display_name = kw->info.display_name;
  }
  kw->display = wl_display_connect(display_name);
  if (display_name == NULL) {
    display_name = "default";
  }
  if (kw->display == NULL) {
    kw->info.state = KR_WL_DISCONNECTED;
    printke("Wayland: Unable to connect to %s display", display_name);
    return;
  }
  printk("Wayland: Connected to %s display", display_name);
  kw->fd = wl_display_get_fd(kw->display);
  kw->info.state = KR_WL_CONNECTED;
  kw->xkb.context = xkb_context_new(0);
  kw->registry = wl_display_get_registry(kw->display);
  wl_registry_add_listener(kw->registry, &kw->registry_listener, kw);
  wl_display_roundtrip(kw->display);
}

static void kw_init(kr_wayland *kw) {
  kw->formats = 0;
  kw->pointer_listener.enter = pointer_handle_enter;
  kw->pointer_listener.leave = pointer_handle_leave;
  kw->pointer_listener.motion = pointer_handle_motion;
  kw->pointer_listener.button = pointer_handle_button;
  kw->pointer_listener.axis = pointer_handle_axis;
  kw->keyboard_listener.keymap = keyboard_handle_keymap;
  kw->keyboard_listener.enter = keyboard_handle_enter;
  kw->keyboard_listener.leave = keyboard_handle_leave;
  kw->keyboard_listener.key = keyboard_handle_key;
  kw->keyboard_listener.modifiers = keyboard_handle_modifiers;
  kw->seat_listener.capabilities = handle_seat_capabilities;
  kw->shm_listener.format = handle_shm_format;
  kw->registry_listener.global = handle_global;
}

kr_adapter *kr_wl_open(kr_adapter_setup *setup) {
  if (setup == NULL) return NULL;
  printk("Wayland: Adapter opened");
  kr_wayland *kw;
  setup->adapter->handle = kr_allocz(1, sizeof(kr_wayland));
  kw = (kr_wayland *)setup->adapter->handle;
  kw->info = setup->info.adp.wl;
  kw_init(kw);
  kw_connect(kw);
  return kw;
}

int kr_wl_lctl(kr_adapter_path *path, kr_patchset *patchset) {
  if (path == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("Wayland window controlled");
  return 0;
}

int kr_wl_unlink(kr_adapter_path *path) {
  if (path == NULL) return -1;
  printk("Wayland window removed");
  return 0;
}

kr_adapter_path *kr_wl_link(kr_adapter *adp, kr_adapter_path_setup *setup) {
  if (adp == NULL) return NULL;
  if (setup == NULL) return NULL;
  printk("Wayland window created");
  return NULL;
}

int kr_wl_ctl(kr_adapter *adp, kr_patchset *patchset) {
  if (adp == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("Wayland adapter controlled");
  return 0;
}
