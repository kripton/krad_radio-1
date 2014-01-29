#include "krad_volume.h"
#include "biquad.h"

struct kr_volume {
  float sample_rate;
  float level;
  float level_actual;
  float new_level_actual;
  int last_sign;
  kr_easer level_easer;
};

void kr_volume_set_level(kr_volume *volume, float level, int duration,
 kr_easing easing, void *user) {
  level = LIMIT(level, 0.0f, 100.0f);
  kr_easer_set(&volume->level_easer, level, duration, easing, user);
}

void kr_volume_set_sample_rate(kr_volume *volume, int sample_rate) {
  volume->sample_rate = sample_rate;
}

kr_volume *kr_volume_create(int sample_rate) {
  kr_volume *volume;
  volume = kr_allocz(1, sizeof(kr_volume));
  volume->sample_rate = sample_rate;
  kr_volume_set_level(volume, 100.0f, 100, EASEINOUTSINE, NULL);
  return volume;
}

void kr_volume_destroy(kr_volume *volume) {
  free(volume);
}

void kr_volume_process2(kr_volume *volume, float *src, float *dst, int ns, int b) {
  void *ptr;
  int s;
  int sign;
  ptr = NULL;
  if (kr_easer_active(&volume->level_easer)) {
    volume->level = kr_easer_process(&volume->level_easer, volume->level, &ptr);
    volume->new_level_actual = volume->level/100.0f;
    volume->new_level_actual *= volume->new_level_actual;
  }
  sign = 0;
  if (volume->new_level_actual == volume->level_actual) {
    for (s = 0; s < ns; s++) {
      dst[s] = src[s] * volume->level_actual;
    }
    return;
  }

  /* The way the volume change is set up here, the volume can only
   * change once per callback, but thats allways plenty
   * last_sign: 0 = unset, -1 neg, +1 pos */
  if (volume->last_sign == 0) {
    if (src[0] > 0.0f) {
      volume->last_sign = 1;
    } else {
      /* Zero counts as negative here, but its moot */
      volume->last_sign = -1;
    }
  }
  for (s = 0; s < ns; s++) {
    if (volume->last_sign != 0) {
      if (src[s] > 0.0f) {
        sign = 1;
      } else {
        sign = -1;
      }
      if ((sign != volume->last_sign) || (src[s] == 0.0f)) {
        volume->level_actual = volume->new_level_actual;
        volume->last_sign = 0;
      }
    }
    dst[s] = src[s] * volume->level_actual;
  }
  if (volume->last_sign != 0) {
    volume->last_sign = sign;
  }
}

int kr_volume_info_get(kr_volume *volume, kr_volume_info *info) {
  if ((volume == NULL) || (info == NULL)) return -1;
  info->level = volume->level;
  return 0;
}
