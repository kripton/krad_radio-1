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

static void router_test(kr_radio *radio) {
  kr_crate2 crate;
  memset(&crate, 0, sizeof(kr_crate2));

  strcpy(crate.address, "/mixer/Music3");
  crate.method = KR_PUT;
  kr_app_server_route(radio->app, &crate);

  strcpy(crate.address, "/mixer/Music3");
  crate.method = KR_GET;
  kr_app_server_route(radio->app, &crate);

  strcpy(crate.address, "/mixer");
  crate.method = KR_GET;
  kr_app_server_route(radio->app, &crate);

  strcpy(crate.address, "/mixer");
  crate.method = KR_PUT;
  kr_app_server_route(radio->app, &crate);

  strcpy(crate.address, "/mixer/Music3");
  crate.method = KR_DELETE;
  kr_app_server_route(radio->app, &crate);

  strcpy(crate.address, "/mixer");
  crate.method = KR_PUT;
  kr_app_server_route(radio->app, &crate);
}

static void mapper_test(kr_radio *radio) {
  kr_app_address_mapper mapper;
  memset(&mapper, 0, sizeof(kr_app_address_mapper));
  strcpy(mapper.prefix, "/mixer");
  mapper.ptr = radio->mixer;
  mapper.create = kr_mixer_mkpath;
  mapper.patch = kr_mixer_path_ctl;
  mapper.destroy = kr_mixer_unlink;
  kr_app_server_add_mapper(radio->app, &mapper);
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
  app_setup.app = radio;
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
        mapper_test(radio);
        router_test(radio);
        kr_app_server_run(radio->app);
        return radio;
      }
    }
  }
  kr_radio_destroy(radio);
  return NULL;
}
