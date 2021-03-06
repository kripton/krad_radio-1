#include "gen.h"
int info_pack_to_json(char *json, uber_St *uber, int32_t max) {
  const info_pack_to_json_func to_json_functions[59] = {kr_aux_info_to_json,
  kr_aux_path_info_to_json,kr_alsa_info_to_json,
  kr_alsa_path_info_to_json,kr_method_to_json,
  kr_crate_to_json,kr_easing_to_json,
  kr_compositor_path_type_to_json,kr_compositor_overlay_type_to_json,
  kr_rect_to_json,kr_compositor_output_info_to_json,
  kr_compositor_bus_info_to_json,kr_compositor_input_info_to_json,
  kr_compositor_source_info_to_json,kr_overlay_type_info_to_json,
  kr_overlay_info_to_json,kr_compositor_path_type_info_to_json,
  kr_compositor_path_info_to_json,kr_pos_to_json,
  kr_perspective_view_to_json,kr_sprite_info_to_json,
  kr_text_info_to_json,kr_vector_type_to_json,
  kr_vector_info_to_json,krad_codec_t_to_json,
  krad_container_type_t_to_json,kr_decklink_info_to_json,
  kr_decklink_path_info_to_json,kr_jack_direction_to_json,
  kr_jack_state_to_json,kr_jack_setup_info_to_json,
  kr_jack_info_to_json,kr_jack_path_info_to_json,
  kr_mixer_channels_to_json,kr_mixer_path_type_to_json,
  kr_mixer_path_info_to_json,kr_nil_to_json,
  kr_payload_type_to_json,kr_payload_to_json,
  kr_sfx_control_to_json,kr_sfx_effect_type_to_json,
  kr_sfx_effect_control_to_json,kr_eq_band_info_to_json,
  kr_eq_info_to_json,kr_lowpass_info_to_json,
  kr_highpass_info_to_json,kr_volume_info_to_json,
  kr_analog_info_to_json,kr_v4l2_state_to_json,
  kr_v4l2_path_info_to_json,kr_v4l2_info_to_json,
  kr_wayland_state_to_json,kr_wayland_info_to_json,
  kr_wayland_path_info_to_json,kr_x11_info_to_json,
  kr_x11_path_info_to_json,kr_xpdr_type_to_json,
  kr_xpdr_type_info_to_json,kr_xpdr_path_info_to_json
  };
  return to_json_functions[uber->type-1](json , uber->actual, max);
}

