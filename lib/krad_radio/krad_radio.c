#include "krad_radio.h"
#include "krad_mixer.h"
#include "krad_compositor.h"
#include "krad_transponder.h"
#include "krad_web_server.h"
#include "krad_app_server.h"
#include "krad_router.h"
#include "krad_timer.h"

struct kr_radio {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_transponder *transponder;
  kr_web_server *web;
  kr_app_server *app;
};

static void web_event(kr_web_event *event);
static void mixer_event(kr_mixer_event *event);
static void compositor_event(kr_compositor_event *event);
static void xpdr_event(kr_transponder_event *event);
static int setup_maps(kr_radio *radio);

static void web_event(kr_web_event *event) {
  kr_radio *radio;
  kr_app_server_client_setup client_setup;
  radio = (kr_radio *)event->user;
  switch (event->type) {
    case KR_WEB_CLIENT_CREATE:
      printk("Radio: Web client create event");
      client_setup.fd = event->fd;
      client_setup.in = event->in;
      client_setup.out = event->out;
      client_setup.state_tracker = event->state_tracker;
      client_setup.state_tracker_sz = event->state_tracker_sz;
      client_setup.input_cb = event->input_cb;
      client_setup.output_cb = event->output_cb;
      kr_app_server_client_create(radio->app, &client_setup);
      break;
    default:
      printke("Radio: Bad event from Web Server");
      break;
  }
}

static void mixer_event(kr_mixer_event *event) {
  kr_radio *radio;
  kr_route_setup route_setup;
  radio = (kr_radio *)event->user;
  switch (event->type) {
    case KR_MIXER_CREATE:
      printk("Radio: Mixer path create event");
      route_setup.ptr = radio->mixer;
      route_setup.name = event->user_path;
      route_setup.ctx = event->path;
      route_setup.payload.mixer_path_info = event->info;
      event->user_path = kr_app_server_route_create(radio->app, &route_setup);
      break;
    case KR_MIXER_PATCH:
      printk("Radio: Mixer path patch event");
      /* update the info struct in the route */
      break;
    case KR_MIXER_DESTROY:
      printk("Radio: Mixer path delete event");
      kr_app_server_route_destroy(radio->app, event->user_path);
      break;
    default:
      printke("Radio: Bad event from mixer");
      break;
  }
}

static void compositor_event(kr_compositor_event *event) {
  kr_radio *radio;
  kr_route_setup route_setup;
  radio = (kr_radio *)event->user;
  switch (event->type) {
    case KR_COM_CREATE:
      printk("Radio: Compositor path create event");
      route_setup.ptr = radio->compositor;
      route_setup.name = event->user_path;
      route_setup.ctx = event->path;
      route_setup.payload.compositor_path_info = event->info;
      event->user_path = kr_app_server_route_create(radio->app, &route_setup);
      break;
    case KR_COM_PATCH:
      printk("Radio: Compositor path patch event");
      /* update the info struct in the route */
      break;
    case KR_COM_DESTROY:
      printk("Radio: Compositor path delete event");
      kr_app_server_route_destroy(radio->app, event->user_path);
      break;
    default:
      printke("Radio: Bad event from compositor");
      break;
  }
}

static void xpdr_event(kr_transponder_event *event) {
  printk("got a xpdr event");
}

static int setup_maps(kr_radio *radio) {
  void *map;
  kr_router_map_setup setup;
  setup.prefix = "/mixer";
  setup.ptr = radio->mixer;
  setup.payload_type = PL_KR_MIXER_PATH_INFO;
  setup.create = (kr_router_map_create_handler *)kr_mixer_mkbus;
  setup.connect = (kr_router_map_connect_handler *)kr_mixer_mkinput;
  setup.patch = (kr_router_map_patch_handler *)kr_mixer_path_ctl;
  setup.destroy = (kr_router_map_destroy_handler *)kr_mixer_unlink;
  map = kr_app_server_map_create(radio->app, &setup);
  if (map == NULL) {
    printke("Radio: router map was null");
    return -1;
  }
  setup.prefix = "/compositor";
  setup.ptr = radio->compositor;
  setup.payload_type = PL_KR_COMPOSITOR_PATH_INFO;
  setup.create = (kr_router_map_create_handler *)kr_compositor_mkbus;
  setup.connect = NULL;
  setup.patch = (kr_router_map_patch_handler *)kr_compositor_path_ctl;
  setup.destroy = (kr_router_map_destroy_handler *)kr_compositor_unlink;
  map = kr_app_server_map_create(radio->app, &setup);
  if (map == NULL) {
    printke("Radio: router map was null");
    return -1;
  }
  setup.prefix = "/transponder";
  setup.ptr = radio->transponder;
  setup.create = (kr_router_map_create_handler *)kr_transponder_mkpath;
  setup.connect = NULL;
  setup.patch = (kr_router_map_patch_handler *)kr_transponder_path_ctl;
  setup.destroy = (kr_router_map_destroy_handler *)kr_transponder_unlink;
  map = kr_app_server_map_create(radio->app, &setup);
  if (map == NULL) {
    printke("Radio: router map was null");
    return -1;
  }
  return 0;
}

kr_radio *kr_radio_create(char *sysname) {
  int ret;
  kr_radio *radio;
  kr_web_server_setup web_setup;
  kr_app_server_setup app_setup;
  kr_mixer_setup mixer_setup;
  kr_compositor_setup compositor_setup;
  kr_transponder_setup transponder_setup;
  printk("Radio: Creating");
  radio = kr_allocz(1, sizeof(kr_radio));
  strncpy(app_setup.appname, "krad_radio", sizeof(app_setup.appname));
  strncpy(app_setup.sysname, sysname, sizeof(app_setup.sysname));
  radio->app = kr_app_server_create(&app_setup);
  if (radio->app) {
    mixer_setup.path_count = 32;
    mixer_setup.user = radio;
    mixer_setup.event_cb = mixer_event;
    radio->mixer = kr_mixer_create(&mixer_setup);
    if (radio->mixer) {
      memset(&compositor_setup, 0, sizeof(kr_compositor_setup));
      compositor_setup.event_cb = compositor_event;
      compositor_setup.user = radio;
      radio->compositor = kr_compositor_create(&compositor_setup);
      if (radio->compositor) {
        transponder_setup.mixer = radio->mixer;
        transponder_setup.compositor = radio->compositor;
        transponder_setup.user = radio;
        transponder_setup.event_cb = xpdr_event;
        transponder_setup.path_count = 32;
        /* FIXME need to enable adapter monitor after maps setup
         * so that we can pick up the adapter paths */
        radio->transponder = kr_transponder_create(&transponder_setup);
        if (radio->transponder) {
          memset(&web_setup, 0, sizeof(kr_web_server_setup));
          web_setup.sysname = sysname;
          web_setup.event_cb = web_event;
          web_setup.user = radio;
          srand(time(NULL));
          web_setup.port = 3000 + rand() % 32000;
          radio->web = kr_web_server_create(&web_setup);
          if (radio->web) {
            ret = setup_maps(radio);
            if (ret == 0) {
              ret = kr_app_server_enable(radio->app);
              if (ret == 0) {
                printk("Radio: Created");
                return radio;
              }
            }
          }
        }
      }
    }
  }
  kr_radio_destroy(radio);
  return NULL;
}

int kr_radio_destroy(kr_radio *radio) {
  kr_timer *timer;
  if (radio == NULL) return -1;
  printk("Radio: Destroying");
  timer = kr_timer_alloca();
  kr_timer_name_set(timer, "Destroy");
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
  kr_timer_finish(timer);
  printk("Radio: Destroyed in %"PRIu64"ms", kr_timer_duration_ms(timer));
  free(radio);
  return 0;
}
