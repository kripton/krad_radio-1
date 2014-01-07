#include "krad_radio.h"
#include "krad_mixer.h"
#include "krad_compositor.h"
#include "krad_transponder.h"
#include "krad_interweb.h"
#include "krad_app_server.h"
#include "krad_timer.h"

struct kr_radio {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_transponder *transponder;
  kr_web_server *web;
  kr_app_server *app;
};

int kr_radio_destroy(kr_radio *radio) {
  kr_timer *timer;
  if (radio == NULL) return -1;
  timer = kr_timer_create_with_name("shutdown");
  kr_timer_start(timer);
  if (radio->app != NULL) {
    kr_app_server_disable(radio->app);
  }
  kr_timer_status(timer);
  if (radio->web != NULL) {
    kr_web_server_destroy(&radio->web);
  }
  kr_timer_status(timer);
  if (radio->transponder != NULL) {
    kr_transponder_destroy(radio->transponder);
    radio->transponder = NULL;
  }
  kr_timer_status(timer);
  if (radio->mixer != NULL) {
    kr_mixer_destroy(radio->mixer);
    radio->mixer = NULL;
  }
  kr_timer_status(timer);
  if (radio->compositor != NULL) {
    kr_compositor_destroy(radio->compositor);
    radio->compositor = NULL;
  }
  kr_timer_status(timer);
  if (radio->app != NULL) {
    kr_app_server_destroy(radio->app);
    radio->app = NULL;
  }
  if (timer != NULL) {
    kr_timer_finish(timer);
    printk("Krad Radio took %"PRIu64"ms to shutdown",
           kr_timer_duration_ms(timer));
    kr_timer_destroy(timer);
    timer = NULL;
  }
  free(radio);
  printk("Krad Radio exited cleanly");
  return 0;
}

static void setup_maps(kr_radio *radio) {
  void *map;
  kr_router_map_setup setup;
  memset(&setup, 0, sizeof(kr_router_map_setup));
  strcpy(setup.prefix, "/mixer");
  setup.ptr = radio->mixer;
  setup.create = kr_mixer_mkpath;
  setup.patch = kr_mixer_path_ctl;
  setup.destroy = kr_mixer_unlink;
  map = kr_app_server_map_create(radio->app, &setup);
  if (map == NULL) {
    printke("map was null");
  }
  memset(&setup, 0, sizeof(kr_router_map_setup));
  strcpy(setup.prefix, "/transponder");
  setup.ptr = radio->transponder;
  setup.create = kr_transponder_mkpath;
  //setup.patch = kr_mixer_path_ctl;
  setup.destroy = kr_transponder_unlink;
  map = kr_app_server_map_create(radio->app, &setup);
  if (map == NULL) {
    printke("map was null");
  }
}

kr_radio *kr_radio_create(char *sysname) {
  kr_radio *radio;
  kr_app_server_setup app_setup;
  kr_mixer_setup mixer_setup;
  kr_compositor_setup compositor_setup;
  kr_transponder_setup transponder_setup;
  radio = calloc(1, sizeof(kr_radio));
  strncpy(app_setup.appname, "krad_radio", sizeof(app_setup.appname));
  strncpy(app_setup.sysname, sysname, sizeof(app_setup.sysname));
  radio->app = kr_app_server_create(&app_setup);
  if (radio->app) {
    kr_mixer_setup_init(&mixer_setup);
    mixer_setup.user = radio;
    //mixer_setup.cb = kr_mixer_server_info_cb;
    radio->mixer = kr_mixer_create(&mixer_setup);
    if (radio->mixer) {
      kr_compositor_setup_init(&compositor_setup);
      radio->compositor = kr_compositor_create(&compositor_setup);
      if (radio->compositor) {
        transponder_setup.mixer = radio->mixer;
        transponder_setup.compositor = radio->compositor;
        radio->transponder = kr_transponder_create(&transponder_setup);
        setup_maps(radio);
        kr_app_server_enable(radio->app);
        return radio;
      }
    }
  }
  kr_radio_destroy(radio);
  return NULL;
}
