#include "krad_easing_common_helpers.h"

int kr_easing_to_index(int val) {
  switch (val) {
    case LINEAR:
      return 0;
    case EASEINSINE:
      return 1;
    case EASEOUTSINE:
      return 2;
    case EASEINOUTSINE:
      return 3;
    case EASEINCUBIC:
      return 4;
    case EASEOUTCUBIC:
      return 5;
    case EASEINOUTCUBIC:
      return 6;
    case EASEINOUTELASTIC:
      return 7;
  }
  return -1;
}

char *kr_strfr_kr_easing(int val) {
  switch (val) {
    case LINEAR:
      return "linear";
    case EASEINSINE:
      return "easeinsine";
    case EASEOUTSINE:
      return "easeoutsine";
    case EASEINOUTSINE:
      return "easeinoutsine";
    case EASEINCUBIC:
      return "easeincubic";
    case EASEOUTCUBIC:
      return "easeoutcubic";
    case EASEINOUTCUBIC:
      return "easeinoutcubic";
    case EASEINOUTELASTIC:
      return "easeinoutelastic";
  }
  return NULL;
}

int kr_strto_kr_easing(char *string) {
  if (!strcmp(string,"linear")) {
    return LINEAR;
  }
  if (!strcmp(string,"easeinsine")) {
    return EASEINSINE;
  }
  if (!strcmp(string,"easeoutsine")) {
    return EASEOUTSINE;
  }
  if (!strcmp(string,"easeinoutsine")) {
    return EASEINOUTSINE;
  }
  if (!strcmp(string,"easeincubic")) {
    return EASEINCUBIC;
  }
  if (!strcmp(string,"easeoutcubic")) {
    return EASEOUTCUBIC;
  }
  if (!strcmp(string,"easeinoutcubic")) {
    return EASEINOUTCUBIC;
  }
  if (!strcmp(string,"easeinoutelastic")) {
    return EASEINOUTELASTIC;
  }

  return -1;
}

