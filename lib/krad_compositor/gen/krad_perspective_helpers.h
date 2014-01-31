#ifndef KRAD_PERSPECTIVE_HELPERS_GEN_H
#define KRAD_PERSPECTIVE_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_perspective.h"
int kr_pos_init(void *st);
int kr_pos_valid(void *st);
int kr_pos_random(void *st);
int kr_perspective_view_init(void *st);
int kr_perspective_view_valid(void *st);
int kr_perspective_view_random(void *st);
int kr_perspective_init(void *st);
int kr_perspective_valid(void *st);
int kr_perspective_random(void *st);
#endif
