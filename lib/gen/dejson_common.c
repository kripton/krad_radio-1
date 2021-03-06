#include "gen.h"
int info_unpack_fr_json(char *json, uber_St *uber) {
  const info_unpack_fr_json_func fr_json_functions[59] = {kr_aux_info_fr_json,
  kr_aux_path_info_fr_json,kr_alsa_info_fr_json,
  kr_alsa_path_info_fr_json,kr_method_fr_json,
  kr_crate_fr_json,kr_easing_fr_json,
  kr_compositor_path_type_fr_json,kr_compositor_overlay_type_fr_json,
  kr_rect_fr_json,kr_compositor_output_info_fr_json,
  kr_compositor_bus_info_fr_json,kr_compositor_input_info_fr_json,
  kr_compositor_source_info_fr_json,kr_overlay_type_info_fr_json,
  kr_overlay_info_fr_json,kr_compositor_path_type_info_fr_json,
  kr_compositor_path_info_fr_json,kr_pos_fr_json,
  kr_perspective_view_fr_json,kr_sprite_info_fr_json,
  kr_text_info_fr_json,kr_vector_type_fr_json,
  kr_vector_info_fr_json,krad_codec_t_fr_json,
  krad_container_type_t_fr_json,kr_decklink_info_fr_json,
  kr_decklink_path_info_fr_json,kr_jack_direction_fr_json,
  kr_jack_state_fr_json,kr_jack_setup_info_fr_json,
  kr_jack_info_fr_json,kr_jack_path_info_fr_json,
  kr_mixer_channels_fr_json,kr_mixer_path_type_fr_json,
  kr_mixer_path_info_fr_json,kr_nil_fr_json,
  kr_payload_type_fr_json,kr_payload_fr_json,
  kr_sfx_control_fr_json,kr_sfx_effect_type_fr_json,
  kr_sfx_effect_control_fr_json,kr_eq_band_info_fr_json,
  kr_eq_info_fr_json,kr_lowpass_info_fr_json,
  kr_highpass_info_fr_json,kr_volume_info_fr_json,
  kr_analog_info_fr_json,kr_v4l2_state_fr_json,
  kr_v4l2_path_info_fr_json,kr_v4l2_info_fr_json,
  kr_wayland_state_fr_json,kr_wayland_info_fr_json,
  kr_wayland_path_info_fr_json,kr_x11_info_fr_json,
  kr_x11_path_info_fr_json,kr_xpdr_type_fr_json,
  kr_xpdr_type_info_fr_json,kr_xpdr_path_info_fr_json
  };
  return fr_json_functions[uber->type-1](json , uber->actual);
}

