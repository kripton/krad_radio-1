#include "krad_jack.h"

struct kr_jack_path {
  kr_jack *jack;
  kr_jack_path_info info;
  jack_port_t *ports[KR_JACK_CHANNELS_MAX];
  void *user;
  kr_jack_path_event_cb *event_cb;
  kr_jack_path_audio_cb *audio_cb;
};

struct kr_jack {
  jack_client_t *client;
  kr_jack_info info;
  int set_thread_name;
  void *user;
  kr_jack_event_cb *event_cb;
};

static int xrun_cb(void *arg);
static void shutdown_cb(void *arg);
static int jack_process(jack_nframes_t nframes, void *arg);
static int path_setup_info_check(kr_jack_path_info *info);
static int path_setup_check(kr_jack_path_setup *setup);
static int jack_setup_check(kr_jack_setup *setup);

static int xrun_cb(void *arg) {
  kr_jack *jack = (kr_jack *)arg;
  jack->info.xruns++;
  printke("Krad Jack: %s xrun number %d!", jack->info.client_name,
   jack->info.xruns);
  return 0;
}

static void shutdown_cb(void *arg) {
  kr_jack *jack = (kr_jack *)arg;
  jack->info.state = KR_JACK_OFFLINE;
  printke("Krad Jack: shutdown callback, oh dear!");
}

static int jack_process(jack_nframes_t nframes, void *arg) {
  kr_jack *jack;
  jack = (kr_jack *)arg;
  if (jack->set_thread_name == 0) {
    //krad_system_set_thread_name("kr_jack_io");
    jack->set_thread_name = 1;
  }
  jack->info.frames += nframes;
  if (jack->info.period_size != nframes) {
    printke("whoa doggie period size changed w/o warning! %u -> %u",
     jack->info.period_size, nframes);
    jack->info.period_size = nframes;
    /* callback event?? */
  }
  return 0;
}

int kr_jack_path_prepare(kr_jack_path *path) {
  kr_jack_path_audio_cb_arg cb_arg;
  int i;
  cb_arg.user = path->user;
  cb_arg.path = path;
  cb_arg.audio.channels = path->info.channels;
  cb_arg.audio.rate = path->jack->info.sample_rate;
  cb_arg.audio.count = path->jack->info.period_size;
  if (path->info.direction == KR_JACK_INPUT) {
    cb_arg.event = KR_JACK_AUDIO_INPUT;
  } else {
    cb_arg.event = KR_JACK_AUDIO_OUTPUT;
  }
  for (i = 0; i < path->info.channels; i++) {
    cb_arg.audio.samples[i] = jack_port_get_buffer(path->ports[i],
     path->jack->info.period_size);
  }
  path->audio_cb(&cb_arg);
  return 0;
}

static void *process_thread(void *arg) {
  kr_jack *jack;
  jack_nframes_t frames;
  int status;
  jack = (kr_jack *)arg;
  status = 0;
  static int test = 0;
  for (;;) {
    frames = jack_cycle_wait(jack->client);
    status = jack_process(frames, jack);
    jack_cycle_signal(jack->client, status);
    test++;
    if (status) {
      break;
    }
    if (test > 6000) {
      status = 1;
    }
  }
  return 0;
}

static int path_setup_info_check(kr_jack_path_info *info) {
  if ((info->channels < 1) || (info->channels > KR_JACK_CHANNELS_MAX)) {
    return -1;
  }
  if ((info->direction != KR_JACK_INPUT)
   && (info->direction != KR_JACK_OUTPUT)) {
    return -2;
  }
  if (memchr(info->name + 1, '\0', sizeof(info->name) - 1) == NULL) {
    return -3;
  }
  if (strlen(info->name) == 0) return -4;
  return 0;
}

static int path_setup_check(kr_jack_path_setup *setup) {
  if (setup->user == NULL) return -1;
  if (setup->audio_cb == NULL) return -2;
  if (setup->event_cb == NULL) return -3;
  if (path_setup_info_check(&setup->info)) {
    printke("jack path setup info failed check");
    return -4;
  }
  return 0;
}

static int jack_setup_check(kr_jack_setup *setup) {
  if (setup->user == NULL) return -1;
  if (setup->event_cb == NULL) return -2;
  /* FIXME check server / client name */
  return 0;
}

int kr_jack_unlink(kr_jack_path *path) {
  int c;
  int ret;
  if (path == NULL) return -1;
  printk("JACK path unlink called for %s", path->info.name);
  for (c = 0; c < path->info.channels; c++) {
    ret = jack_port_unregister(path->jack->client, path->ports[c]);
    if (ret != 0) {
      //FIXME watcha gonna do bout that? deal with it?
    }
    path->ports[c] = NULL;
  }
  free(path);
  printk("JACK path unlink finished");
  return 0;
}

kr_jack_path *kr_jack_mkpath(kr_jack *jack, kr_jack_path_setup *setup) {
  int c;
  kr_jack_path *path;
  int port_flags;
  char *port_suffix;
  char port_name[192];
  if ((jack == NULL) || (setup == NULL)) return NULL;
  if (path_setup_check(setup)) {
    printke("jack path setup failed check");
    return NULL;
  }
  path = kr_allocz(1, sizeof(kr_jack_path));
  path->jack = jack;
  path->user = setup->user;
  path->audio_cb = setup->audio_cb;
  path->event_cb = setup->event_cb;
  memcpy(&path->info, &setup->info, sizeof(kr_jack_path_info));
  if (jack->info.state != KR_JACK_ONLINE) {
    printke("Not creating JACK API ports because jack server is offline");
    return path;
  }
  if (path->info.direction == KR_JACK_INPUT) {
    port_flags = JackPortIsInput;
    port_suffix = "audio_in";
  } else {
    port_flags = JackPortIsOutput;
    port_suffix = "audio_out";
  }
  printk("JACK mkpath: %s (%d chan)", path->info.name, path->info.channels);
  for (c = 0; c < path->info.channels; c++) {
    snprintf(port_name, sizeof(port_name), "%s/%s %d", path->info.name,
     port_suffix, c + 1);
    path->ports[c] = jack_port_register(path->jack->client, port_name,
     JACK_DEFAULT_AUDIO_TYPE, port_flags, 0);
    if (path->ports[c] == NULL) {
      printke("JACK: port register failure: %s", port_name);
      //FIXME fail better
    }
  }
  return path;
}

int kr_jack_destroy(kr_jack *jack) {
  printk("Jack destroy started");
  if (jack == NULL) return -1;
  if (jack->client != NULL) {
    jack_client_close(jack->client);
    jack->client = NULL;
    jack->info.state = KR_JACK_OFFLINE;
  }
  if (jack->info.xruns == 0) {
    printk("JACK had no xruns processing %"PRIu64" frames!",
      jack->info.frames);
  } else {
    printke("JACK had %u xruns by destroy :/", jack->info.xruns);
  }
  free(jack);
  printk("Jack destroy complete");
  return 0;
}

kr_jack *kr_jack_create(kr_jack_setup *setup) {
  kr_jack *jack;
  char *name;
  char old_thread_name[16];
  jack_status_t status;
  jack_options_t options;
  if (setup == NULL) return NULL;
  if (jack_setup_check(setup)) return NULL;
  memset(old_thread_name, 0, sizeof(old_thread_name));
  jack = kr_allocz(1, sizeof(kr_jack));
  strncpy(jack->info.client_name, setup->client_name,
   sizeof(jack->info.client_name));
  jack->event_cb = setup->event_cb;
  jack->user = setup->user;
  if ((setup->server_name[0] != '\0')
   && (memchr(setup->server_name + 1, '\0', sizeof(setup->server_name) - 1)
    != NULL)) {
    strncpy(jack->info.server_name, setup->server_name,
     sizeof(jack->info.server_name));
    options = JackNoStartServer | JackServerName;
  } else {
    jack->info.server_name[0] = '\0';
    options = JackNoStartServer;
  }
  kr_systm_get_thread_name(old_thread_name);
  krad_system_set_thread_name("kr_jack");
  jack->client = jack_client_open(jack->info.client_name, options, &status,
   jack->info.server_name);
  if (jack->client == NULL) {
    printke("Krad Jack: jack_client_open() failed, status = 0x%2.0x", status);
    if (status & JackServerFailed) {
      printke("Krad Jack: Unable to connect to JACK server");
    }
    return jack;
  }
  if (status & JackNameNotUnique) {
    name = jack_get_client_name(jack->client);
    strncpy(jack->info.client_name, name, sizeof(jack->info.client_name));
    printke("Krad Jack: unique name `%s' assigned", jack->info.client_name);
  }
  jack->info.sample_rate = jack_get_sample_rate(jack->client);
  jack->info.period_size = jack_get_buffer_size(jack->client);
  //jack_set_process_callback(jack->client, jack_process, jack);
  jack_set_process_thread(jack->client, process_thread, jack);
  jack_on_shutdown(jack->client, shutdown_cb, jack);
  //jack_set_xrun_callback (jack->client, xrun_cb, jack);
  /* FIXME sample rate / period size callbacks */
  //jack_set_port_registration_callback(jack->client, port_registration, jack);
  //jack_set_port_connect_callback(jack->client, port_connection, jack);
  if (jack_activate(jack->client)) {
    jack->info.state = KR_JACK_OFFLINE;
  } else {
    jack->info.state = KR_JACK_ONLINE;
  }
  krad_system_set_thread_name(old_thread_name);
  return jack;
}

int kr_jack_detect() {
  return kr_jack_detect_server_name(NULL);
}

int kr_jack_detect_server_name(char *name) {
  jack_client_t *client;
  jack_options_t options;
  jack_status_t status;
  char client_name[128];
  sprintf(client_name, "kr_jack_detect_%d", rand());
  if (name != NULL) {
    options = JackNoStartServer | JackServerName;
  } else {
    options = JackNoStartServer;
  }
  client = jack_client_open(client_name, options, &status, name);
  if (client == NULL) {
    return 0;
  } else {
    jack_client_close(client);
    return 1;
  }
}
