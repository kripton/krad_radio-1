struct kr_compositor_path {
  kr_compositor_path_info info;
  kr_compositor_control_easers easers;
  void *frame_user;
  void *control_user;
  kr_compositor_path_frame_cb *frame_cb;
  kr_compositor *compositor;
  kr_easer crop_x_easer;
  kr_easer crop_y_easer;
  kr_easer crop_width_easer;
  kr_easer crop_height_easer;
  kr_convert converter;
  kr_perspective *perspective;
  kr_vertex *graph_vrt;
};

static void path_tick(kr_compositor_path *path);

static float kr_round3(float f) {
  f = rintf(f * 1000.0);
  f = f / 1000.0;
  return f;
}

size_t kr_compositor_path_size() {
  return sizeof(kr_compositor_path);
}

kr_compositor_path_type path_type_get(kr_compositor_path *path) {
  return path->info.type;
}

void controls_tick(kr_compositor_controls *c, kr_compositor_control_easers *e) {
  if (kr_easer_active(&e->x)) {
    c->x = kr_easer_process(&e->x, c->x, NULL);
  }
  if (kr_easer_active(&e->y)) {
    c->y = kr_easer_process(&e->y, c->y, NULL);
  }
/*if (kr_easer_active(&e->z)) {
    c->z = kr_easer_process(&e->z, c->z, NULL);
  }*/
  if (kr_easer_active(&e->w)) {
    c->w = kr_easer_process(&e->w, c->w, NULL);
  }
  if (kr_easer_active(&e->h)) {
    c->h = kr_easer_process(&e->h, c->h, NULL);
  }
  if (kr_easer_active(&e->opacity)) {
    c->opacity = kr_easer_process(&e->opacity, c->opacity, NULL);
  }
  if (kr_easer_active(&e->rotation)) {
    c->rotation = kr_easer_process(&e->rotation, c->rotation, NULL);
  }
}

static void path_tick(kr_compositor_path *path) {
  controls_tick(&path->info.controls, &path->easers);
  if (kr_easer_active(&path->crop_x_easer)) {
    path->info.crop_x = kr_easer_process(&path->crop_x_easer,
     path->info.crop_x, NULL);
  }
  if (kr_easer_active(&path->crop_y_easer)) {
    path->info.crop_y = kr_easer_process(&path->crop_y_easer,
     path->info.crop_y, NULL);
  }
  if (kr_easer_active(&path->crop_width_easer)) {
    path->info.crop_width = kr_easer_process(&path->crop_width_easer,
     path->info.crop_width, NULL);
  }
  if (kr_easer_active(&path->crop_height_easer)) {
    path->info.crop_height = kr_easer_process(&path->crop_height_easer,
     path->info.crop_height, NULL);
  }
}

void path_output(kr_compositor_path *path, kr_image *image) {
  kr_compositor_path_frame_cb_arg cb_arg;
  cb_arg.user = path->frame_user;
  path->frame_cb(&cb_arg);
  memcpy(cb_arg.image.px, image->px, image->w * image->h * 4);
}

int path_render(kr_compositor_path *path, kr_image *dst, cairo_t *cr) {

  int ret;
  kr_image image;
  kr_compositor_path_frame_cb_arg cb_arg;
  cairo_surface_t *src;
  static uint8_t scratch[1920*1080*4]; /*FIXME*/

  cb_arg.user = path->frame_user;
  path_tick(path);
  /*if (path->info.controls.opacity == 0.0f) return 0; Hrmzor */
  path->frame_cb(&cb_arg);
  /* After the frame_cb if the parameters (crop, size)
   *  have not changed we should see if the image has also not changed
   *  in which case we can use a cached version -- this function can be
   *   used perhaps beforehand so output can wait on new input
   */
  if ((path->info.controls.x == 0)
   && (path->info.controls.y == 0)
   && (path->info.controls.w == 0)
   && (path->info.controls.h == 0)
   && (path->info.controls.opacity == 1.0f)
   && (path->info.controls.rotation == 0.0f)) {
  /*image = subimage(dst, params);*/
    image = *dst;
    ret = kr_image_convert(&path->converter, &image, &cb_arg.image);
    if (ret != 0) {
      printke("kr_image convert returned %d :/", ret);
      return -1;
    }
    return 0;
  }

/*image = subimage(path_scratch_image, params);*/
  image = *dst;
  image.px = scratch;
  image.ppx[0] = image.px;
  if (path->info.controls.w != 0) {
    image.w = path->info.controls.w;
  }
  if (path->info.controls.h != 0) {
    image.h = path->info.controls.h;
  }
  path->converter.crop.x = path->info.crop_x;
  path->converter.crop.y = path->info.crop_y;
  path->converter.crop.w = path->info.crop_width;
  path->converter.crop.h = path->info.crop_height;
  ret = kr_image_convert(&path->converter, &image, &cb_arg.image);
  if (ret != 0) {
    printke("kr_image convert returned %d :/", ret);
    return -1;
  }

  cairo_save(cr);
  src = cairo_image_surface_create_for_data(image.px, CAIRO_FORMAT_ARGB32,
   image.w, image.h, image.pps[0]);

  if (path->info.controls.rotation != 0.0f) {
    cairo_translate (cr, path->info.controls.x, path->info.controls.y);
    cairo_translate(cr, (int)(image.w) / 2.0, (int)(image.h) / 2.0);
    cairo_rotate(cr, kr_round3(path->info.controls.rotation * (M_PI/180.0)));
    cairo_translate(cr, - (int)(image.w) / 2.0, - (int)(image.h) / 2.0);
    cairo_translate (cr, -path->info.controls.x, -path->info.controls.y);
  }
  cairo_set_source_surface(cr, src, path->info.controls.x, path->info.controls.y);
  cairo_rectangle(cr, path->info.controls.x, path->info.controls.y, image.w, image.h);
  cairo_paint_with_alpha(cr, kr_round3(path->info.controls.opacity));
  cairo_restore(cr);
  cairo_surface_destroy(src);
  return 0;
}

int path_setup_check(kr_compositor_io_path_setup *setup) {
  kr_compositor_path_info *info;
  info = &setup->info;
  if ((setup->frame_user == NULL) || (setup->frame_cb == NULL)) {
    /* FIXME HRMMM */
  }
  if (setup->control_user == NULL) {
    /* FIXME HRMMM */
  }
  if ((info->width == 0) || (info->height == 0)) {
    return -1;
  }
  if ((info->type != KR_CMP_INPUT) && (info->type != KR_CMP_OUTPUT)) {
    return -2;
  }
  /* FIXME check more things out */
  return 0;
}

static void path_create(kr_compositor_path *path,
 kr_compositor_io_path_setup *setup) {
  kr_compositor_event event;
  path->info = setup->info;
  /* FIXME a silly default? */
  path->info.controls.opacity = 0.0f;
  kr_easer_set(&path->easers.opacity, 1.0f, 60, EASEINOUTSINE, NULL);
  /* End silly thing */
  path->frame_user = setup->frame_user;
  path->control_user = setup->control_user;
  path->frame_cb = setup->frame_cb;
  kr_image_convert_init(&path->converter);
  if (path->info.type == KR_CMP_INPUT) {
    path->compositor->info.inputs++;
  }
  if (path->info.type == KR_CMP_OUTPUT) {
    path->compositor->info.outputs++;
  }
  event.user = path->compositor->user;
  event.user_path = path->control_user;
  event.path = path;
  event.type = KR_COMP_CREATE;
  event.info = path->info;
  path->compositor->event_cb(&event);
}

int kr_compositor_mkbus(kr_compositor *c, kr_compositor_path_info *i, void *user) {
  return -1;
}

kr_compositor_path *kr_compositor_mkio(kr_compositor *compositor,
 kr_compositor_io_path_setup *setup) {
  kr_compositor_path *path;
  if ((compositor == NULL) || (setup == NULL)) return NULL;
  if (path_setup_check(setup)) {
    printke("compositor mkpath failed setup check");
    return NULL;
  }
  /*
  path = kr_mixer_find(mixer, setup->info.name);
  if (path != NULL) {
    printke("mixer mkpath path with that name already exists");
    return NULL;
  }
  */
  path = kr_pool_slice(compositor->path_pool);
  if (path == NULL) {
    printke("compositor mkpath could not slice new path");
    return NULL;
  }
  path->compositor = compositor;
  path_create(path, setup);
  path->graph_vrt = kr_graph_vertex_create(compositor->comp_graph,setup->info.type);
  return path;
}

void cmper_path_release(kr_compositor *compositor, kr_compositor_path *path) {
  if (path->perspective != NULL) {
    kr_perspective_destroy(&path->perspective);
  }
  kr_image_convert_clear(&path->converter);
  if (path->info.type == KR_CMP_INPUT) {
    compositor->info.inputs--;
  }
  if (path->info.type == KR_CMP_OUTPUT) {
    compositor->info.outputs--;
  }
  kr_graph_vertex_destroy(compositor->comp_graph,path->graph_vrt);
  kr_pool_recycle(path->compositor->path_pool, path);
}

int kr_compositor_unlink(kr_compositor_path *path) {
  if (path == NULL) return -1;
  /*FIXME*/
  /* do destroy event callback */
  return 0;
}

int kr_compositor_path_info_get(kr_compositor_path *path,
 kr_compositor_path_info *info) {
 if ((path == NULL) || (info == NULL)) return -1;
 *info = path->info;
 return 0;
}

int kr_compositor_path_ctl(kr_compositor_path *p, kr_compositor_path_patch *s) {
  if ((p == NULL) || (s == NULL)) return -1;
  switch (s->control) {
    case KR_NO:
      return -1;
    case KR_X:
      kr_easer_set(&p->easers.x, s->integer, s->duration, EASEINOUTSINE, NULL);
      break;
    case KR_Y:
      kr_easer_set(&p->easers.y, s->integer, s->duration, EASEINOUTSINE, NULL);
      break;
    case KR_Z:
     break;
    case KR_WIDTH:
      kr_easer_set(&p->easers.w, s->integer, s->duration, EASEINOUTSINE, NULL);
      break;
    case KR_HEIGHT:
      kr_easer_set(&p->easers.h, s->integer, s->duration, EASEINOUTSINE, NULL);
      break;
    case KR_ROTATION:
      kr_easer_set(&p->easers.rotation, s->real, s->duration, s->easing, NULL);
      break;
    case KR_OPACITY:
      kr_easer_set(&p->easers.opacity, s->real, s->duration, EASEINOUTSINE, NULL);
      break;
    case KR_RED:
    case KR_GREEN:
    case KR_BLUE:
    case KR_ALPHA:
    case KR_CROP_X:
      kr_easer_set(&p->crop_x_easer, s->integer, s->duration, EASEINOUTSINE, NULL);
      break;
    case KR_CROP_Y:
      kr_easer_set(&p->crop_y_easer, s->integer, s->duration, EASEINOUTSINE, NULL);
      break;
    case KR_CROP_W:
      kr_easer_set(&p->crop_width_easer, s->integer, s->duration, EASEINOUTSINE, NULL);
      break;
    case KR_CROP_H:
      kr_easer_set(&p->crop_height_easer, s->integer, s->duration, EASEINOUTSINE, NULL);
      break;
    case KR_VIEW_TL_X:
      p->info.view.top_left.x = s->real;
      break;
    case KR_VIEW_TL_Y:
      p->info.view.top_left.y = s->real;
      break;
    case KR_VIEW_TR_X:
      p->info.view.top_right.x = s->real;
      break;
    case KR_VIEW_TR_Y:
      p->info.view.top_right.y = s->real;
      break;
    case KR_VIEW_BL_X:
      p->info.view.bottom_left.x = s->real;
      break;
    case KR_VIEW_BL_Y:
      p->info.view.bottom_left.y = s->real;
      break;
    case KR_VIEW_BR_X:
      p->info.view.bottom_right.x = s->real;
      break;
    case KR_VIEW_BR_Y:
      p->info.view.bottom_right.y = s->real;
      break;
    default:
      break;
  }
  return 0;
}
