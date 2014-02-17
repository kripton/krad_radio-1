static void compositor_frame(kr_frame_event *event);
static void mixer_audio(kr_audio_event *event);
static void adapter_avio(kr_avio_event *event);
static void adapter_event(kr_adapter_event *event);

static void compositor_frame(kr_frame_event *event) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)event->user;
  event->image = path->link.image;
}

static void mixer_audio(kr_audio_event *event) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)event->user;
  event->audio = path->link.audio;
}

static void adapter_avio(kr_avio_event *event) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)event->user;
  path->link.audio = event->audio;
  path->link.image = event->image;
  if (event->state == KR_AVIO_WANT_OUTPUT_FRAME) {
    printk("XPDR: Got AVIO event with state: want output frame");
  }
  if (event->state == KR_AVIO_HAVE_INPUT_FRAME) {
    printk("XPDR: Got AVIO event with state: have input frame");
  }
  if (event->state == KR_AVIO_NONE) {
    printk("XPDR: Got AVIO event with state NONE");
  }
  /* FIXME take event image/audio and call a m/c function to
   * propagate the state */
}

static void adapter_event(kr_adapter_event *event) {
  /*
  kr_xpdr *xpdr;
  kr_xpdr_path *path;
  kr_adapter_info info;
  int i;
  path = (kr_xpdr_path *)event->user;
  */
  /* Created / destroyed? or is that on xpdr ? */
  if (event->type == KR_ADAPTER_DISCONNECTED) {
    printk("Adapter Disconnected");
    return;
  }
  if (event->type == KR_ADAPTER_RECONNECTED) {
    printk("Adapter Reconnected");
    return;
  }
  if (event->type != KR_ADAPTER_PROCESS) {
    printk("Wasn't a process event");
    return;
  }
  /*
  kr_adapter_get_info(event->adapter, &info);
  *//* I think we should trigger all of the mixer ports that are connected
   * to this adapter path *//*
  int test = 0;
  i = 0;
  while ((path = kr_pool_iterate_active(xpdr->path_pool, &i))) {
    if (path->info.output.type == ADAPTER) {
      if (path->info.output.type == COMPOSITOR) {
        kr_compositor_process(path->input.compositor_port);
      }
      if (path->info.input.type == MIXER) {
        kr_mixer_process(path->input.mixer_port);
      }
      test++;
    }
  }
  */
}


