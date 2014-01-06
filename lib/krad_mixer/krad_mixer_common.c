#include "krad_mixer_common.h"

char *kr_mixer_channeltostr(int channel) {
  switch (channel) {
    case 0:
      return "Left";
    case 1:
      return "Right";
    case 2:
      return "RearLeft";
    case 3:
      return "RearRight";
    case 4:
      return "Center";
    case 5:
      return "Sub";
    case 6:
      return "BackLeft";
    case 7:
      return "BackRight";
    default:
      return "Unknown";
  }
}

char *kr_mixer_pathtypetostr(kr_mixer_path_type type) {
  switch (type) {
   case KR_MXR_INPUT:
      return "INPUT";
    case KR_MXR_BUS:
      return "BUS";
    case KR_MXR_OUTPUT:
      return "OUTPUT";
  }
  return "Unknown";
}

char *kr_mixer_ctltostr(kr_mixer_control control) {
  switch (control) {
    case KR_VOLUME:
      return "volume";
    case KR_CROSSFADE:
      return "crossfade";
    case KR_PEAK:
      return "peak";
    case KR_CROSSFADE_GROUP:
      return "crossfade_group";
  }
  return "Unknown";
}
