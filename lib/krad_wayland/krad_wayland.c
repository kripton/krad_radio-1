#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <signal.h>
#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>
#include "krad_system.h"
#include "krad_wayland.h"

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
  int display_fd;
  kr_wayland_path window[KR_WL_MAX_WINDOWS];
  kr_wayland_path *key_window;
  kr_wayland_path *pointer_window;
  struct wl_display *display;
  struct wl_registry *registry;
  struct wl_compositor *compositor;
  struct wl_shell *shell;
  struct wl_shm *shm;
  uint32_t formats;
  uint32_t mask;
  struct wl_shm_listener shm_listener;
  struct wl_seat *seat;
  struct wl_pointer *pointer;
  struct wl_keyboard *keyboard;
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

static int kr_wayland_path_create_shm_buffer (kr_wayland_path *window,
 int width, int height, int frames, uint32_t format, void **data_out);

static void kr_wayland_handle_configure (void *data,
 struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width,
 int32_t height);
static void kr_wayland_handle_popup_done (void *data,
 struct wl_shell_surface *shell_surface);
static void kr_wayland_handle_ping (void *data,
 struct wl_shell_surface *shell_surface, uint32_t serial);
static void kr_wayland_seat_handle_capabilities (void *data,
 struct wl_seat *seat, enum wl_seat_capability caps);
static void kr_wayland_shm_format (void *data, struct wl_shm *wl_shm,
 uint32_t format);
static void kr_wayland_handle_global (void *data, struct wl_registry *registry,
 uint32_t id, const char *interface, uint32_t version);
static void kr_wayland_frame_listener (void *data,
 struct wl_callback *callback, uint32_t time);

#include "input.c"

static void kr_wayland_handle_configure(void *data,
 struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width,
 int32_t height) {
  /* Nothing here */
}

static void kr_wayland_handle_popup_done(void *data,
 struct wl_shell_surface *shell_surface) {
  /* Nothing here */
}

static void kr_wayland_handle_ping(void *data,
 struct wl_shell_surface *shell_surface, uint32_t serial) {
  wl_shell_surface_pong(shell_surface, serial);
}

static void kr_wayland_seat_handle_capabilities(void *data,
 struct wl_seat *seat, enum wl_seat_capability caps) {
  kr_wayland *wayland = data;
  if ((caps & WL_SEAT_CAPABILITY_POINTER) && !wayland->pointer) {
    wayland->pointer = wl_seat_get_pointer(seat);
    /*wl_pointer_set_user_data (wayland->pointer, wayland);*/
    wl_pointer_add_listener (wayland->pointer, &wayland->pointer_listener,
     wayland);
  } else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && wayland->pointer) {
    wl_pointer_destroy(wayland->pointer);
    wayland->pointer = NULL;
  }
  if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !wayland->keyboard) {
    wayland->keyboard = wl_seat_get_keyboard(seat);
    /*wl_keyboard_set_user_data(wayland->keyboard, wayland);*/
    wl_keyboard_add_listener(wayland->keyboard, &wayland->keyboard_listener,
     wayland);
  } else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && wayland->keyboard) {
    wl_keyboard_destroy(wayland->keyboard);
    wayland->keyboard = NULL;
  }
}

static void kr_wayland_shm_format(void *data, struct wl_shm *wl_shm,
 uint32_t format) {
  kr_wayland *wayland = data;
  wayland->formats |= (1 << format);
}

static void kr_wayland_handle_global(void *data, struct wl_registry *registry,
 uint32_t id, const char *interface, uint32_t version) {
  kr_wayland *wayland = data;
  if (strcmp(interface, "wl_compositor") == 0) {
    wayland->compositor =
      wl_registry_bind (wayland->registry, id,
       &wl_compositor_interface, 1);
  } else if (strcmp(interface, "wl_shell") == 0) {
    wayland->shell = wl_registry_bind(wayland->registry, id,
     &wl_shell_interface, 1);
  } else if (strcmp(interface, "wl_seat") == 0) {
    wayland->seat = wl_registry_bind(wayland->registry, id,
     &wl_seat_interface, 1);
    wl_seat_add_listener(wayland->seat,
     &wayland->seat_listener, wayland);
  } else if (strcmp(interface, "wl_shm") == 0) {
    wayland->shm = wl_registry_bind(wayland->registry, id,
     &wl_shm_interface, 1);
    wl_shm_add_listener(wayland->shm, &wayland->shm_listener,
     wayland);
  }
}

static int kr_wayland_path_create_shm_buffer(kr_wayland_path *window,
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

static void kr_wayland_frame_listener(void *data,
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
  window->frame_listener.done = kr_wayland_frame_listener;
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
  ret = kr_wayland_path_create_shm_buffer(window, window->width,
   window->height, KR_WL_BUFFER_COUNT, WL_SHM_FORMAT_XRGB8888,
   &window->shm_data);
  if (ret != 0) {
    wayland->window[i].active = 0;
    return NULL;
  }
  window->current_buffer = 0;
  window->buffer = window->buffers[window->current_buffer];
  window->surface_listener.ping = kr_wayland_handle_ping;
  window->surface_listener.configure = kr_wayland_handle_configure;
  window->surface_listener.popup_done = kr_wayland_handle_popup_done;
  window->surface = wl_compositor_create_surface(wayland->compositor);
  window->shell_surface = wl_shell_get_shell_surface(wayland->shell,
   window->surface);
  opaque = wl_compositor_create_region(wayland->compositor);
  wl_region_add(opaque, 0, 0, window->width, window->height);
  wl_surface_set_opaque_region(window->surface, opaque);
  wl_region_destroy(opaque);
  /*wl_shell_surface_set_title(wayland->window->shell_surface,
   wayland->window->title);*/
   wl_shell_surface_add_listener(window->shell_surface,
    &window->surface_listener, window);
  wl_shell_surface_set_toplevel(window->shell_surface);
  kr_wayland_frame_listener(window, NULL, 0);
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

int kr_wayland_get_fd(kr_wayland *wayland) {
  if (wayland == NULL) {
    return -1;
  }
  return wayland->display_fd;
}

int kr_wayland_process(kr_wayland *wayland) {
  /* TODO Check for disconnect etc */
  wl_display_dispatch(wayland->display);
  wl_display_roundtrip(wayland->display);
  return 0;
}

int kr_wayland_destroy(kr_wayland **wl) {
  int i;
  kr_wayland *wayland;
  if ((wl == NULL) || (*wl == NULL)) {
    return -1;
  }
  wayland = *wl;
  for (i = 0; i < KR_WL_MAX_WINDOWS; i++) {
    if (wayland->window[i].active == 1) {
      wl_display_sync(wayland->display);
      if (wayland->window[i].callback) {
        wl_callback_destroy(wayland->window[i].callback);
      }
      wl_display_sync(wayland->display);
      wl_buffer_destroy(wayland->window[i].buffer);
      wl_shell_surface_destroy(wayland->window[i].shell_surface);
      wl_surface_destroy(wayland->window[i].surface);
      wl_display_sync(wayland->display);
      wayland->window[i].active = 0;
    }
  }
  if (wayland->pointer != NULL) {
    wl_pointer_destroy(wayland->pointer);
    wayland->pointer = NULL;
  }
  if (wayland->keyboard != NULL) {
    wl_keyboard_destroy(wayland->keyboard);
    wayland->keyboard = NULL;
  }
  if (wayland->xkb.state) {
    xkb_state_unref(wayland->xkb.state);
  }
  if (wayland->xkb.keymap) {
    xkb_map_unref(wayland->xkb.keymap);
  }
  if (wayland->xkb.context) {
    xkb_context_unref(wayland->xkb.context);
  }
  if (wayland->shm) {
    wl_shm_destroy (wayland->shm);
  }
  if (wayland->shell) {
    wl_shell_destroy (wayland->shell);
  }
  if (wayland->compositor) {
    wl_compositor_destroy (wayland->compositor);
  }
  if (wayland->seat) {
    wl_seat_destroy (wayland->seat);
  }
  wl_registry_destroy(wayland->registry);
  wl_display_flush(wayland->display);
  wl_display_disconnect(wayland->display);
  free(wayland);
  *wl = NULL;
  return 0;
}

kr_wayland *kr_wayland_create(kr_wayland_setup *setup) {
  kr_wayland *wayland;
  wayland = kr_allocz(1, sizeof(kr_wayland));
  if (setup != NULL) {
    wayland->info = setup->info;
  }
  if (strlen(wayland->info.display_name)) {
    wayland->display = wl_display_connect(wayland->info.display_name);
  } else {
    wayland->display = wl_display_connect(NULL);
  }
  /* FIXME fail better */
  if (wayland->display == NULL) {
    free(wayland);
    return NULL;
  }
  wayland->xkb.context = xkb_context_new(0);
  wayland->pointer_listener.enter = pointer_handle_enter;
  wayland->pointer_listener.leave = pointer_handle_leave;
  wayland->pointer_listener.motion = pointer_handle_motion;
  wayland->pointer_listener.button = pointer_handle_button;
  wayland->pointer_listener.axis = pointer_handle_axis;
  wayland->keyboard_listener.keymap = keyboard_handle_keymap;
  wayland->keyboard_listener.enter = keyboard_handle_enter;
  wayland->keyboard_listener.leave = keyboard_handle_leave;
  wayland->keyboard_listener.key = keyboard_handle_key;
  wayland->keyboard_listener.modifiers = keyboard_handle_modifiers;
  wayland->seat_listener.capabilities = kr_wayland_seat_handle_capabilities;
  wayland->shm_listener.format = kr_wayland_shm_format;
  wayland->formats = 0;
  wayland->registry_listener.global = kr_wayland_handle_global;
  wayland->registry = wl_display_get_registry (wayland->display);
  wl_registry_add_listener(wayland->registry, &wayland->registry_listener,
   wayland);
  wl_display_roundtrip(wayland->display);
  wayland->display_fd = wl_display_get_fd(wayland->display);
  return wayland;
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

int kr_wl_close(kr_adapter *adp) {
  if (adp == NULL) return -1;
  printk("Wayland adapter closed");
  return 0;
}

kr_adapter *kr_wl_open(kr_adapter_setup *setup) {
  if (setup == NULL) return NULL;
  printk("Wayland adapter opened");
  return NULL;
}
