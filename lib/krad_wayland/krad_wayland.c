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
#include "krad_image_pool.h"

#define KR_WL_MAX_WINDOWS 4
#define KR_WL_BUFFER_COUNT 2

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

struct kr_wayland_path {
  int pointer_x;
  int pointer_y;
  int click;
  int pointer_in;
  int pointer_out;
  int active;
  int fullscreen;
  struct wl_surface *surface;
  struct wl_shell_surface *shell_surface;
  struct wl_callback *callback;
  struct wl_shell_surface_listener surface_listener;
  struct wl_callback_listener frame_listener;
  struct wl_buffer *buffer[KR_WL_BUFFER_COUNT];
  kr_image_pool *pool;
  int (*user_callback)(void *, kr_wayland_event *);
  void *user;
  kr_wayland_path_info *info;
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
  kr_wayland_info *info;
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
  printk("Wayland: surface configure");
}

static void handle_popup_done(void *data,
 struct wl_shell_surface *shell_surface) {
  /* Nothing here */
  printk("Wayland: surface popup done");
}

static void handle_ping(void *data, struct wl_shell_surface *ss, uint32_t serial) {
  wl_shell_surface_pong(ss, serial);
  printk("Wayland: surface ping");
}

static void handle_shm_format(void *data, struct wl_shm *wl_shm, uint32_t format) {
  kr_wayland *kw;
  kw = (kr_wayland *)data;
  kw->formats |= (1 << format);
  printk("Wayland: shm format %u", format);
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
  printk("Wayland: global %s", interface);
}

/*
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
  kw->info->state = KR_WL_DISCONNECTED;
}

static void kw_connect(kr_wayland *kw) {
  char *display_name;
  display_name = NULL;
  if (strnlen(kw->info->display_name, sizeof(kw->info->display_name))) {
    display_name = kw->info->display_name;
  }
  kw->display = wl_display_connect(display_name);
  if (display_name == NULL) {
    display_name = "default";
  }
  if (kw->display == NULL) {
    kw->info->state = KR_WL_DISCONNECTED;
    printke("Wayland: Unable to connect to %s display", display_name);
    return;
  }
  printk("Wayland: Connected to %s display", display_name);
  kw->fd = wl_display_get_fd(kw->display);
  kw->info->state = KR_WL_CONNECTED;
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

int kr_wl_lctl(kr_adapter_path *path, kr_patchset *patchset) {
  if (path == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("Wayland window controlled");
  return 0;
}

int kr_wl_unlink(kr_adapter_path *path) {
  int i;
  kr_wayland_path *window;
  kr_wayland *kw;
  if (path == NULL) return -1;
  printk("Wayland window removed");
  window = (kr_wayland_path *)path->handle;
  kw = window->wayland;
  for (i = 0; i < KR_WL_MAX_WINDOWS; i++) {
    if (&kw->window[i] == window) {
      break;
    }
  }
  wl_display_sync(kw->display);
  if (window->callback) {
    wl_callback_destroy(window->callback);
  }
  wl_display_sync(kw->display);
  for (i = 0; i < KR_WL_BUFFER_COUNT; i++) {
    wl_buffer_destroy(window->buffer[i]);
  }
  kr_pool_destroy(window->pool);
  wl_shell_surface_destroy(window->shell_surface);
  wl_surface_destroy(window->surface);
  wl_display_sync(kw->display);
  kw->window[i].active = 0;
  return 0;
}

int kr_wl_link(kr_adapter *adapter, kr_adapter_path *path) {
  kr_wayland *kw;
  kr_wayland_path *window;
  kr_wayland_path_info *info;
  struct wl_region *opaque;
  struct wl_shm_pool *pool;
  kr_image image;
  void *buf;
  int i;
  if (adapter == NULL) return -1;
  if (path == NULL) return -2;
  kw = (kr_wayland *)adapter->handle;
  info = &path->info->adp.wl_out;
  if ((info->width == 0) || (info->height == 0)
   || (info->width > 8192) || (info->height > 8192)) {
    printke("Wayland: window too big");
    return -2;
  }
  for (i = 0; i < KR_WL_MAX_WINDOWS; i++) {
    if (kw->window[i].active == 0) {
      break;
    }
  }
  if (i == KR_WL_MAX_WINDOWS) {
    return -1;
  }
  window = &kw->window[i];
  window->wayland = kw;
  window->info = &path->info->adp.wl_out;
  window->user = path->user;
  memset(&image, 0, sizeof(image));
  image.w = info->width;
  image.h = info->height;
  image.pps[0] = image.w * 4;
  window->pool = kr_image_pool_create(&image, 2);
  if (window->pool == NULL) {
    printke("Wayland: error creating kr image pool");
    return -1;
  }
  pool = wl_shm_create_pool(window->wayland->shm, kr_pool_fd(window->pool),
   kr_pool_size(window->pool));
  if (pool == NULL) {
    printke("Wayland: error creating wl_shm_pool");
    return -1;
  }
  for (i = 0; i < KR_WL_BUFFER_COUNT; i++) {
    buf = kr_pool_slice(window->pool);
    window->buffer[i] = wl_shm_pool_create_buffer(pool,
     kr_pool_offsetof(window->pool, buf), image.w, image.h, image.pps[0],
     WL_SHM_FORMAT_XRGB8888);
    if (window->buffer[i] == NULL) {
      printke("Wayland: error creating wl buffer from wl shm pool");
    }
  }
  wl_shm_pool_destroy(pool);
  window->surface_listener.ping = handle_ping;
  window->surface_listener.configure = handle_configure;
  window->surface_listener.popup_done = handle_popup_done;
  window->surface = wl_compositor_create_surface(kw->compositor);
  window->shell_surface = wl_shell_get_shell_surface(kw->shell, window->surface);
  opaque = wl_compositor_create_region(kw->compositor);
  wl_region_add(opaque, 0, 0, window->info->width, window->info->height);
  wl_surface_set_opaque_region(window->surface, opaque);
  wl_region_destroy(opaque);
  /*wl_shell_surface_set_title(wayland->window->shell_surface,
   wayland->window->title);*/
  wl_shell_surface_add_listener(window->shell_surface,
   &window->surface_listener, window);
  wl_shell_surface_set_toplevel(window->shell_surface);
  //kw_frame_listener(window, NULL, 0);
  if (window->fullscreen == 1) {
    wl_shell_surface_set_fullscreen(window->shell_surface, 1, 0, NULL);
  }
  window->active = 1;
  path->handle = window;
  printk("Wayland window created");
  return 0;
}

int kr_wl_ctl(kr_adapter *adp, kr_patchset *patchset) {
  if (adp == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("Wayland adapter controlled");
  return 0;
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

int kr_wl_open(kr_adapter *adapter) {
  if (adapter == NULL) return -1;
  printk("Wayland: Adapter opened");
  kr_wayland *kw;
  adapter->handle = kr_allocz(1, sizeof(kr_wayland));
  kw = (kr_wayland *)adapter->handle;
  kw->info = &adapter->info->adp.wl;
  kw_init(kw);
  kw_connect(kw);
  adapter->fd = kw->fd;
  return 0;
}
