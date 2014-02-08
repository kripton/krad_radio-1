#ifndef COMMON_GEN_H
#define COMMON_GEN_H
#include "krad_ebmlx.h"

typedef struct {
  int type;
  void *actual;
} uber_St;

int info_pack_to_text(char *text, uber_St *uber, int32_t max);
typedef int (*info_pack_to_text_func)(char *text, void *st, int max);
int info_pack_to_json(char *json, uber_St *uber, int32_t max);
typedef int (*info_pack_to_json_func)(char *json, void *st, int max);
int info_unpack_fr_json(char *json, uber_St *uber);
typedef int (*info_unpack_fr_json_func)(char *json, void *st);
int info_pack_to_ebml(kr_ebml *ebml, uber_St *uber);
typedef int (*info_pack_to_ebml_func)(kr_ebml *ebml, void *st);
int info_unpack_fr_ebml(kr_ebml *ebml, uber_St *uber);
typedef int (*info_unpack_fr_ebml_func)(kr_ebml *ebml, void *st);

typedef enum {
  TEXT_KR_AUX_INFO = 1,
  TEXT_KR_AUX_PATH_INFO,
  TEXT_KR_ALSA_INFO,
  TEXT_KR_ALSA_PATH_INFO,
  TEXT_KR_METHOD,
  TEXT_KR_CRATE,
  TEXT_KR_EASING,
  TEXT_KR_COMPOSITOR_PATH_TYPE,
  TEXT_KR_COMPOSITOR_OVERLAY_TYPE,
  TEXT_KR_RECT,
  TEXT_KR_COMPOSITOR_OUTPUT_INFO,
  TEXT_KR_COMPOSITOR_BUS_INFO,
  TEXT_KR_COMPOSITOR_INPUT_INFO,
  TEXT_KR_COMPOSITOR_SOURCE_INFO,
  TEXT_KR_OVERLAY_TYPE_INFO,
  TEXT_KR_OVERLAY_INFO,
  TEXT_KR_COMPOSITOR_PATH_TYPE_INFO,
  TEXT_KR_COMPOSITOR_PATH_INFO,
  TEXT_KR_POS,
  TEXT_KR_PERSPECTIVE_VIEW,
  TEXT_KR_SPRITE_INFO,
  TEXT_KR_TEXT_INFO,
  TEXT_KR_VECTOR_TYPE,
  TEXT_KR_VECTOR_INFO,
  TEXT_KRAD_CODEC_T,
  TEXT_KRAD_CONTAINER_TYPE_T,
  TEXT_KR_DECKLINK_INFO,
  TEXT_KR_DECKLINK_PATH_INFO,
  TEXT_KR_JACK_DIRECTION,
  TEXT_KR_JACK_STATE,
  TEXT_KR_JACK_SETUP_INFO,
  TEXT_KR_JACK_INFO,
  TEXT_KR_JACK_PATH_INFO,
  TEXT_KR_MIXER_CHANNELS,
  TEXT_KR_MIXER_PATH_TYPE,
  TEXT_KR_MIXER_PATH_INFO,
  TEXT_KR_NIL,
  TEXT_KR_PAYLOAD_TYPE,
  TEXT_KR_PAYLOAD,
  TEXT_KR_SFX_CONTROL,
  TEXT_KR_SFX_EFFECT_TYPE,
  TEXT_KR_SFX_EFFECT_CONTROL,
  TEXT_KR_EQ_BAND_INFO,
  TEXT_KR_EQ_INFO,
  TEXT_KR_LOWPASS_INFO,
  TEXT_KR_HIGHPASS_INFO,
  TEXT_KR_VOLUME_INFO,
  TEXT_KR_ANALOG_INFO,
  TEXT_KR_V4L2_STATE,
  TEXT_KR_V4L2_MODE,
  TEXT_KR_V4L2_INFO,
  TEXT_KR_V4L2_OPEN_INFO,
  TEXT_KR_WAYLAND_INFO,
  TEXT_KR_WAYLAND_PATH_INFO,
  TEXT_KR_X11_INFO,
  TEXT_KR_X11_PATH_INFO,
  TEXT_KR_XPDR_TYPE,
  TEXT_KR_XPDR_TYPE_INFO,
  TEXT_KR_XPDR_PATH_INFO
} text_enum;

enum {
  TEXT_ENUM_LAST = 59
};

typedef enum {
  JSON_KR_AUX_INFO = 1,
  JSON_KR_AUX_PATH_INFO,
  JSON_KR_ALSA_INFO,
  JSON_KR_ALSA_PATH_INFO,
  JSON_KR_METHOD,
  JSON_KR_CRATE,
  JSON_KR_EASING,
  JSON_KR_COMPOSITOR_PATH_TYPE,
  JSON_KR_COMPOSITOR_OVERLAY_TYPE,
  JSON_KR_RECT,
  JSON_KR_COMPOSITOR_OUTPUT_INFO,
  JSON_KR_COMPOSITOR_BUS_INFO,
  JSON_KR_COMPOSITOR_INPUT_INFO,
  JSON_KR_COMPOSITOR_SOURCE_INFO,
  JSON_KR_OVERLAY_TYPE_INFO,
  JSON_KR_OVERLAY_INFO,
  JSON_KR_COMPOSITOR_PATH_TYPE_INFO,
  JSON_KR_COMPOSITOR_PATH_INFO,
  JSON_KR_POS,
  JSON_KR_PERSPECTIVE_VIEW,
  JSON_KR_SPRITE_INFO,
  JSON_KR_TEXT_INFO,
  JSON_KR_VECTOR_TYPE,
  JSON_KR_VECTOR_INFO,
  JSON_KRAD_CODEC_T,
  JSON_KRAD_CONTAINER_TYPE_T,
  JSON_KR_DECKLINK_INFO,
  JSON_KR_DECKLINK_PATH_INFO,
  JSON_KR_JACK_DIRECTION,
  JSON_KR_JACK_STATE,
  JSON_KR_JACK_SETUP_INFO,
  JSON_KR_JACK_INFO,
  JSON_KR_JACK_PATH_INFO,
  JSON_KR_MIXER_CHANNELS,
  JSON_KR_MIXER_PATH_TYPE,
  JSON_KR_MIXER_PATH_INFO,
  JSON_KR_NIL,
  JSON_KR_PAYLOAD_TYPE,
  JSON_KR_PAYLOAD,
  JSON_KR_SFX_CONTROL,
  JSON_KR_SFX_EFFECT_TYPE,
  JSON_KR_SFX_EFFECT_CONTROL,
  JSON_KR_EQ_BAND_INFO,
  JSON_KR_EQ_INFO,
  JSON_KR_LOWPASS_INFO,
  JSON_KR_HIGHPASS_INFO,
  JSON_KR_VOLUME_INFO,
  JSON_KR_ANALOG_INFO,
  JSON_KR_V4L2_STATE,
  JSON_KR_V4L2_MODE,
  JSON_KR_V4L2_INFO,
  JSON_KR_V4L2_OPEN_INFO,
  JSON_KR_WAYLAND_INFO,
  JSON_KR_WAYLAND_PATH_INFO,
  JSON_KR_X11_INFO,
  JSON_KR_X11_PATH_INFO,
  JSON_KR_XPDR_TYPE,
  JSON_KR_XPDR_TYPE_INFO,
  JSON_KR_XPDR_PATH_INFO
} json_enum;

enum {
  JSON_ENUM_LAST = 59
};

typedef enum {
  DEJSON_KR_AUX_INFO = 1,
  DEJSON_KR_AUX_PATH_INFO,
  DEJSON_KR_ALSA_INFO,
  DEJSON_KR_ALSA_PATH_INFO,
  DEJSON_KR_METHOD,
  DEJSON_KR_CRATE,
  DEJSON_KR_EASING,
  DEJSON_KR_COMPOSITOR_PATH_TYPE,
  DEJSON_KR_COMPOSITOR_OVERLAY_TYPE,
  DEJSON_KR_RECT,
  DEJSON_KR_COMPOSITOR_OUTPUT_INFO,
  DEJSON_KR_COMPOSITOR_BUS_INFO,
  DEJSON_KR_COMPOSITOR_INPUT_INFO,
  DEJSON_KR_COMPOSITOR_SOURCE_INFO,
  DEJSON_KR_OVERLAY_TYPE_INFO,
  DEJSON_KR_OVERLAY_INFO,
  DEJSON_KR_COMPOSITOR_PATH_TYPE_INFO,
  DEJSON_KR_COMPOSITOR_PATH_INFO,
  DEJSON_KR_POS,
  DEJSON_KR_PERSPECTIVE_VIEW,
  DEJSON_KR_SPRITE_INFO,
  DEJSON_KR_TEXT_INFO,
  DEJSON_KR_VECTOR_TYPE,
  DEJSON_KR_VECTOR_INFO,
  DEJSON_KRAD_CODEC_T,
  DEJSON_KRAD_CONTAINER_TYPE_T,
  DEJSON_KR_DECKLINK_INFO,
  DEJSON_KR_DECKLINK_PATH_INFO,
  DEJSON_KR_JACK_DIRECTION,
  DEJSON_KR_JACK_STATE,
  DEJSON_KR_JACK_SETUP_INFO,
  DEJSON_KR_JACK_INFO,
  DEJSON_KR_JACK_PATH_INFO,
  DEJSON_KR_MIXER_CHANNELS,
  DEJSON_KR_MIXER_PATH_TYPE,
  DEJSON_KR_MIXER_PATH_INFO,
  DEJSON_KR_NIL,
  DEJSON_KR_PAYLOAD_TYPE,
  DEJSON_KR_PAYLOAD,
  DEJSON_KR_SFX_CONTROL,
  DEJSON_KR_SFX_EFFECT_TYPE,
  DEJSON_KR_SFX_EFFECT_CONTROL,
  DEJSON_KR_EQ_BAND_INFO,
  DEJSON_KR_EQ_INFO,
  DEJSON_KR_LOWPASS_INFO,
  DEJSON_KR_HIGHPASS_INFO,
  DEJSON_KR_VOLUME_INFO,
  DEJSON_KR_ANALOG_INFO,
  DEJSON_KR_V4L2_STATE,
  DEJSON_KR_V4L2_MODE,
  DEJSON_KR_V4L2_INFO,
  DEJSON_KR_V4L2_OPEN_INFO,
  DEJSON_KR_WAYLAND_INFO,
  DEJSON_KR_WAYLAND_PATH_INFO,
  DEJSON_KR_X11_INFO,
  DEJSON_KR_X11_PATH_INFO,
  DEJSON_KR_XPDR_TYPE,
  DEJSON_KR_XPDR_TYPE_INFO,
  DEJSON_KR_XPDR_PATH_INFO
} dejson_enum;

enum {
  DEJSON_ENUM_LAST = 59
};

typedef enum {
  EBML_KR_AUX_INFO = 1,
  EBML_KR_AUX_PATH_INFO,
  EBML_KR_ALSA_INFO,
  EBML_KR_ALSA_PATH_INFO,
  EBML_KR_METHOD,
  EBML_KR_CRATE,
  EBML_KR_EASING,
  EBML_KR_COMPOSITOR_PATH_TYPE,
  EBML_KR_COMPOSITOR_OVERLAY_TYPE,
  EBML_KR_RECT,
  EBML_KR_COMPOSITOR_OUTPUT_INFO,
  EBML_KR_COMPOSITOR_BUS_INFO,
  EBML_KR_COMPOSITOR_INPUT_INFO,
  EBML_KR_COMPOSITOR_SOURCE_INFO,
  EBML_KR_OVERLAY_TYPE_INFO,
  EBML_KR_OVERLAY_INFO,
  EBML_KR_COMPOSITOR_PATH_TYPE_INFO,
  EBML_KR_COMPOSITOR_PATH_INFO,
  EBML_KR_POS,
  EBML_KR_PERSPECTIVE_VIEW,
  EBML_KR_SPRITE_INFO,
  EBML_KR_TEXT_INFO,
  EBML_KR_VECTOR_TYPE,
  EBML_KR_VECTOR_INFO,
  EBML_KRAD_CODEC_T,
  EBML_KRAD_CONTAINER_TYPE_T,
  EBML_KR_DECKLINK_INFO,
  EBML_KR_DECKLINK_PATH_INFO,
  EBML_KRAD_IO_MODE_T,
  EBML_KR_IO2_ST,
  EBML_KR_JACK_DIRECTION,
  EBML_KR_JACK_STATE,
  EBML_KR_JACK_SETUP_INFO,
  EBML_KR_JACK_INFO,
  EBML_KR_JACK_PATH_INFO,
  EBML_KR_MIXER_CHANNELS,
  EBML_KR_MIXER_PATH_TYPE,
  EBML_KR_MIXER_PATH_INFO,
  EBML_KR_NIL,
  EBML_KR_PAYLOAD_TYPE,
  EBML_KR_PAYLOAD,
  EBML_KR_SFX_CONTROL,
  EBML_KR_SFX_EFFECT_TYPE,
  EBML_KR_SFX_EFFECT_CONTROL,
  EBML_KR_EQ_BAND_INFO,
  EBML_KR_EQ_INFO,
  EBML_KR_LOWPASS_INFO,
  EBML_KR_HIGHPASS_INFO,
  EBML_KR_VOLUME_INFO,
  EBML_KR_ANALOG_INFO,
  EBML_KR_V4L2_STATE,
  EBML_KR_V4L2_MODE,
  EBML_KR_V4L2_INFO,
  EBML_KR_V4L2_OPEN_INFO,
  EBML_KR_WAYLAND_INFO,
  EBML_KR_WAYLAND_PATH_INFO,
  EBML_KR_X11_INFO,
  EBML_KR_X11_PATH_INFO,
  EBML_KR_XPDR_TYPE,
  EBML_KR_XPDR_TYPE_INFO,
  EBML_KR_XPDR_PATH_INFO
} ebml_enum;

enum {
  EBML_ENUM_LAST = 61
};

typedef enum {
  DEBML_KR_AUX_INFO = 1,
  DEBML_KR_AUX_PATH_INFO,
  DEBML_KR_ALSA_INFO,
  DEBML_KR_ALSA_PATH_INFO,
  DEBML_KR_METHOD,
  DEBML_KR_CRATE,
  DEBML_KR_EASING,
  DEBML_KR_COMPOSITOR_PATH_TYPE,
  DEBML_KR_COMPOSITOR_OVERLAY_TYPE,
  DEBML_KR_RECT,
  DEBML_KR_COMPOSITOR_OUTPUT_INFO,
  DEBML_KR_COMPOSITOR_BUS_INFO,
  DEBML_KR_COMPOSITOR_INPUT_INFO,
  DEBML_KR_COMPOSITOR_SOURCE_INFO,
  DEBML_KR_OVERLAY_TYPE_INFO,
  DEBML_KR_OVERLAY_INFO,
  DEBML_KR_COMPOSITOR_PATH_TYPE_INFO,
  DEBML_KR_COMPOSITOR_PATH_INFO,
  DEBML_KR_POS,
  DEBML_KR_PERSPECTIVE_VIEW,
  DEBML_KR_SPRITE_INFO,
  DEBML_KR_TEXT_INFO,
  DEBML_KR_VECTOR_TYPE,
  DEBML_KR_VECTOR_INFO,
  DEBML_KRAD_CODEC_T,
  DEBML_KRAD_CONTAINER_TYPE_T,
  DEBML_KR_DECKLINK_INFO,
  DEBML_KR_DECKLINK_PATH_INFO,
  DEBML_KR_JACK_DIRECTION,
  DEBML_KR_JACK_STATE,
  DEBML_KR_JACK_SETUP_INFO,
  DEBML_KR_JACK_INFO,
  DEBML_KR_JACK_PATH_INFO,
  DEBML_KR_MIXER_CHANNELS,
  DEBML_KR_MIXER_PATH_TYPE,
  DEBML_KR_MIXER_PATH_INFO,
  DEBML_KR_NIL,
  DEBML_KR_PAYLOAD_TYPE,
  DEBML_KR_PAYLOAD,
  DEBML_KR_SFX_CONTROL,
  DEBML_KR_SFX_EFFECT_TYPE,
  DEBML_KR_SFX_EFFECT_CONTROL,
  DEBML_KR_EQ_BAND_INFO,
  DEBML_KR_EQ_INFO,
  DEBML_KR_LOWPASS_INFO,
  DEBML_KR_HIGHPASS_INFO,
  DEBML_KR_VOLUME_INFO,
  DEBML_KR_ANALOG_INFO,
  DEBML_KR_V4L2_STATE,
  DEBML_KR_V4L2_MODE,
  DEBML_KR_V4L2_INFO,
  DEBML_KR_V4L2_OPEN_INFO,
  DEBML_KR_WAYLAND_INFO,
  DEBML_KR_WAYLAND_PATH_INFO,
  DEBML_KR_X11_INFO,
  DEBML_KR_X11_PATH_INFO,
  DEBML_KR_XPDR_TYPE,
  DEBML_KR_XPDR_TYPE_INFO,
  DEBML_KR_XPDR_PATH_INFO
} debml_enum;

enum {
  DEBML_ENUM_LAST = 59
};

typedef enum {
  HELPERS_KR_AUX_INFO = 1,
  HELPERS_KR_AUX_PATH_INFO,
  HELPERS_KR_ALSA_INFO,
  HELPERS_KR_ALSA_PATH_INFO,
  HELPERS_KR_METHOD,
  HELPERS_KR_CRATE,
  HELPERS_KR_EASING,
  HELPERS_KR_COMPOSITOR_PATH_TYPE,
  HELPERS_KR_COMPOSITOR_OVERLAY_TYPE,
  HELPERS_KR_RECT,
  HELPERS_KR_COMPOSITOR_OUTPUT_INFO,
  HELPERS_KR_COMPOSITOR_BUS_INFO,
  HELPERS_KR_COMPOSITOR_INPUT_INFO,
  HELPERS_KR_COMPOSITOR_SOURCE_INFO,
  HELPERS_KR_OVERLAY_TYPE_INFO,
  HELPERS_KR_OVERLAY_INFO,
  HELPERS_KR_COMPOSITOR_PATH_TYPE_INFO,
  HELPERS_KR_COMPOSITOR_PATH_INFO,
  HELPERS_KR_POS,
  HELPERS_KR_PERSPECTIVE_VIEW,
  HELPERS_KR_SPRITE_INFO,
  HELPERS_KR_TEXT_INFO,
  HELPERS_KR_VECTOR_TYPE,
  HELPERS_KR_VECTOR_INFO,
  HELPERS_KRAD_CODEC_T,
  HELPERS_KRAD_CONTAINER_TYPE_T,
  HELPERS_KR_DECKLINK_INFO,
  HELPERS_KR_DECKLINK_PATH_INFO,
  HELPERS_KR_JACK_DIRECTION,
  HELPERS_KR_JACK_STATE,
  HELPERS_KR_JACK_SETUP_INFO,
  HELPERS_KR_JACK_INFO,
  HELPERS_KR_JACK_PATH_INFO,
  HELPERS_KR_MIXER_CHANNELS,
  HELPERS_KR_MIXER_PATH_TYPE,
  HELPERS_KR_MIXER_PATH_INFO,
  HELPERS_KR_NIL,
  HELPERS_KR_PAYLOAD_TYPE,
  HELPERS_KR_PAYLOAD,
  HELPERS_KR_SFX_CONTROL,
  HELPERS_KR_SFX_EFFECT_TYPE,
  HELPERS_KR_SFX_EFFECT_CONTROL,
  HELPERS_KR_EQ_BAND_INFO,
  HELPERS_KR_EQ_INFO,
  HELPERS_KR_LOWPASS_INFO,
  HELPERS_KR_HIGHPASS_INFO,
  HELPERS_KR_VOLUME_INFO,
  HELPERS_KR_ANALOG_INFO,
  HELPERS_KR_V4L2_STATE,
  HELPERS_KR_V4L2_MODE,
  HELPERS_KR_V4L2_INFO,
  HELPERS_KR_V4L2_OPEN_INFO,
  HELPERS_KR_WAYLAND_INFO,
  HELPERS_KR_WAYLAND_PATH_INFO,
  HELPERS_KR_X11_INFO,
  HELPERS_KR_X11_PATH_INFO,
  HELPERS_KR_XPDR_TYPE,
  HELPERS_KR_XPDR_TYPE_INFO,
  HELPERS_KR_XPDR_PATH_INFO
} helpers_enum;

enum {
  HELPERS_ENUM_LAST = 59
};


#endif

