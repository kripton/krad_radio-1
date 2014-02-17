static int adapter_avio(kr_av_event *event);
static void adapter_event(kr_adapter_event *event);

static void mixer_audio(kr_audio_event *event) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)event->user;
  //event->audio = path->link.audio;
}

static int adapter_avio(kr_av_event *event) {
  int ret;
  ret = -1;
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)event->user;
  if ((path->mode == KR_VIDEO_IN) || (path->mode == KR_VIDEO_OUT)) {
    if (path->mode == KR_VIDEO_IN) {
      printk("XPDR: Got a frame in from a video in adapter");
    }
    if (path->mode == KR_VIDEO_OUT) {
      printk("XPDR: Got a frame request from a video out adapter");
    }
    ret = kr_compositor_frame(path->link.compositor_port, event->image);
  }
  return ret;
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
