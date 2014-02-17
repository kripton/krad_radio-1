/*
static void path_output(kr_compositor_path *path, kr_image *image);
static int path_render(kr_compositor_path *path,
  kr_compositor_input_info *input_info, kr_image *image, cairo_t *cr);

static void path_output(kr_compositor_path *path, kr_image *image) {
  kr_frame_event event;
  event.user = path->frame_user;
  path->frame_cb(&event);
  memcpy(event.image.px, image->px, image->w * image->h * 4);
}

static int path_render(kr_compositor_path *path,
  kr_compositor_input_info *input_info, kr_image *dst, cairo_t *cr) {
  return 0;
}
*/

static void output_source_render(kr_compositor_path *output, kr_compositor_path *source) {
  kr_chain user_chains[16];
  int n;
  int i;
  n = kr_graph_chains(output->compositor->graph, &output->g.elem, &source->g.elem, user_chains, 16);
  for (i = 0; i < n; i++) {
    //transform = get_transform(&user_chains[i]);
    //render(output, source, transform);
  }
}

static void output_render(kr_compositor_path *output) {
  void *source_users[16];
  kr_compositor_path *source;
  int i;
  int n;
  n = kr_graph_sources(output->compositor->graph,
    &output->g.elem, source_users, 16);
  for (i = 0; i < n; i++) {
    source = (kr_compositor_path *)source_users[i];
    output_source_render(output, source);
  }
}

int kr_compositor_process(kr_compositor_path *path) {
  void *output_users[16];
  kr_compositor_path *output;
  int i;
  int n;
  if (path == NULL) return -1;
  if (path->info.type == KR_COM_OUTPUT) {
    //if (path->writeable) {
      output_render(path);
    //}
    return 0;
  } else {
    n = kr_graph_outputs(path->compositor->graph,
        &path->g.elem, output_users, 16);
    for (i = 0; i < n; i++) {
      output = (kr_compositor_path *)output_users[i];
      //if (output->writeable) {
      output_render(output);
      //}
    }
  }
  return 0;
}


