#ifndef KRAD_PERSPECTIVE_COMMON_HELPERS_GEN_H
#define KRAD_PERSPECTIVE_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_perspective_common.h"
int kr_pos_init(struct kr_pos *st);
int kr_pos_valid(struct kr_pos *st);
int kr_pos_random(struct kr_pos *st);
int kr_perspective_view_init(struct kr_perspective_view *st);
int kr_perspective_view_valid(struct kr_perspective_view *st);
int kr_perspective_view_random(struct kr_perspective_view *st);
#endif
