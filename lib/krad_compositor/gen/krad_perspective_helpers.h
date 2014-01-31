#ifndef KRAD_PERSPECTIVE_HELPERS_GEN_H
#define KRAD_PERSPECTIVE_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_perspective.h"
int kr_pos_init(struct kr_pos *st);
int kr_pos_valid(struct kr_pos *st);
int kr_pos_random(struct kr_pos *st);
int kr_perspective_view_init(struct kr_perspective_view *st);
int kr_perspective_view_valid(struct kr_perspective_view *st);
int kr_perspective_view_random(struct kr_perspective_view *st);
int kr_perspective_init(struct kr_perspective *st);
int kr_perspective_valid(struct kr_perspective *st);
int kr_perspective_random(struct kr_perspective *st);
#endif
