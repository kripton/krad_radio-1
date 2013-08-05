#include "krad_easing.h"

void kr_easer_destroy(kr_easer *easer) {
	free(easer);
}

kr_easer *kr_easer_create() {

  kr_easer *easer;

  easer = calloc(1, sizeof(kr_easer));
  return easer;
}

kr_easing kr_easing_random() {
	return rand () % (LASTEASING - FIRSTEASING) + FIRSTEASING;
}

float kr_easer_process(kr_easer *easer, float current, void **ptr) {

  float value;

  if (easer->active == 0) {
    return current;
  } else if (easer->active == 2) {
    while (!__sync_bool_compare_and_swap(&easer->updating, 0, 1));
    easer->target = easer->new_target;
    if (easer->new_duration < 2) {
      if (ptr != NULL) {
        *ptr = easer->new_ptr;
      }
      easer->active = 0;
      while (!__sync_bool_compare_and_swap(&easer->updating, 1, 0));
      return easer->target;
    } else {
      easer->active = 1;
      easer->elapsed_time = 1;
      easer->duration = easer->new_duration;
      easer->ptr = easer->new_ptr;
      easer->start_value = current;
      easer->change_amount = easer->target - easer->start_value;
      easer->easing = easer->new_easing;
      while (!__sync_bool_compare_and_swap(&easer->updating, 1, 0));
    }
  }

  if (easer->elapsed_time == easer->duration) {
    value = easer->target;
    if (ptr != NULL) {
      *ptr = easer->ptr;
    }
    while (!__sync_bool_compare_and_swap(&easer->updating, 0, 1));
    if (easer->active == 1) {
      easer->active = 0;
    }
    while (!__sync_bool_compare_and_swap(&easer->updating, 1, 0));
  } else {
    value = kr_ease(easer->easing, easer->elapsed_time, easer->start_value,
     easer->change_amount, easer->duration);
    value = floorf(value * 100.0f + 0.5f) / 100.0f;
    easer->elapsed_time++;
  }

  return value;
}

void kr_easer_set(kr_easer *easer, float target, int duration,
 kr_easing easing, void *ptr) {
  while (!__sync_bool_compare_and_swap(&easer->updating, 0, 1));
  easer->new_ptr = ptr;
  easer->new_target = target;
  easer->new_duration = duration;
  easer->new_easing = easing;
  easer->active = 2;
  while (!__sync_bool_compare_and_swap(&easer->updating, 1, 0));
}

float kr_ease(kr_easing easing, float time_now, float start_pos,
 float change_amt, float duration) {

  float s, p, a;

	switch (easing) {
		case LINEAR:
			return change_amt*time_now/duration + start_pos;
		case EASEINSINE:
			return -change_amt * cos(time_now/duration * (M_PI/2.0f)) + change_amt + start_pos;
		case EASEOUTSINE:
			return change_amt * sin(time_now/duration * (M_PI/2.0f)) + start_pos;
		case EASEINOUTSINE:
			return -change_amt/2.0f * (cos(M_PI*time_now/duration) - 1.0f) + start_pos;
		case EASEINCUBIC:
			time_now /= duration;
			return change_amt*(time_now)*time_now*time_now + start_pos;
		case EASEOUTCUBIC:
			time_now /= duration;
			time_now -= 1.0f;
			return change_amt*((time_now)*time_now*time_now + 1.0f) + start_pos;
		case EASEINOUTCUBIC:
			time_now /= duration/2.0f;
			if ((time_now) < 1.0f) {
				return change_amt/2.0f*time_now*time_now*time_now + start_pos;
			} else {
				time_now -= 2.0f;
				return change_amt/2.0f*((time_now)*time_now*time_now + 2.0f) + start_pos;
			}
    case EASEINOUTELASTIC:
      s = 1.70158;
      p = 0;
      a = change_amt;
      if (change_amt == 0) return start_pos;
      if (time_now == 0) return start_pos;
      if (time_now == duration) return start_pos + change_amt;
      time_now /= (duration/2.0f);
      if (time_now >= 2.0) return start_pos + change_amt;

      if (p == 0) p = duration * 0.3;
      if (a < fabsf(change_amt)) {
        a = change_amt;
        s= p/4;
      } else {
        s = p/(2.0f*M_PI) * asin (change_amt/a);
      }
      if (time_now < 1) {
        time_now -= 1.0f;
        return -.5f*(a*pow(2,10*time_now) * sin( (time_now*duration-s)*(2.0f*M_PI)/p )) + start_pos;
      }
      time_now -= 1.0f;
      return a*pow(2,-10*time_now) * sin( (time_now*duration-s)*(2.0f*M_PI)/p )*.5f + change_amt + start_pos;
    default:
	    return change_amt*time_now/duration + start_pos;
  }
}
