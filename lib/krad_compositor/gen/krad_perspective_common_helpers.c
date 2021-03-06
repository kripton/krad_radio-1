#include "krad_perspective_common_helpers.h"

int kr_pos_init(struct kr_pos *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_pos_valid(struct kr_pos *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_pos_random(struct kr_pos *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_perspective_view_init(struct kr_perspective_view *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_perspective_view));
  kr_pos_init(&st->top_left);
  kr_pos_init(&st->top_right);
  kr_pos_init(&st->bottom_left);
  kr_pos_init(&st->bottom_right);

  return 0;
}

int kr_perspective_view_valid(struct kr_perspective_view *st) {
  if (st == NULL) {
    return -1;
  }

  kr_pos_valid(&st->top_left);
  kr_pos_valid(&st->top_right);
  kr_pos_valid(&st->bottom_left);
  kr_pos_valid(&st->bottom_right);

  return 0;
}

int kr_perspective_view_random(struct kr_perspective_view *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_perspective_view));
  kr_pos_random(&st->top_left);
  kr_pos_random(&st->top_right);
  kr_pos_random(&st->bottom_left);
  kr_pos_random(&st->bottom_right);

  return 0;
}

