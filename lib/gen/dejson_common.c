#include "gen.h"
int info_unpack_fr_json(char *json, uber_St *uber) {
  const info_unpack_fr_json_func fr_json_functions[63] = {kr_alsa_info_fr_json,
  kr_alsa_path_info_fr_json,kr_app_method_fr_json,
  kr_crate2_fr_json,kr_easing_fr_json,
  kr_compositor_path_type_fr_json,kr_compositor_subunit_type_fr_json,
  kr_vector_type_fr_json,kr_compositor_control_fr_json,
  kr_compositor_controls_fr_json,kr_sprite_info_fr_json,
  kr_text_info_fr_json,kr_vector_info_fr_json,
  kr_compositor_path_info_fr_json,kr_pos_fr_json,
  kr_perspective_view_fr_json,kr_perspective_fr_json,
  krad_codec_t_fr_json,krad_container_type_t_fr_json,
  kr_decklink_info_fr_json,kr_decklink_path_info_fr_json,
  kr_jack_direction_fr_json,kr_jack_state_fr_json,
  kr_jack_setup_info_fr_json,kr_jack_info_fr_json,
  kr_jack_path_info_fr_json,kr_mixer_channels_fr_json,
  kr_mixer_path_type_fr_json,kr_mixer_path_info_fr_json,
  kr_nil_fr_json,kr_radio_payload_type_fr_json,
  kr_radio_payload_fr_json,kr_sfx_control_fr_json,
  kr_sfx_effect_type_fr_json,kr_sfx_effect_control_fr_json,
  kr_eq_band_info_fr_json,kr_eq_info_fr_json,
  kr_lowpass_info_fr_json,kr_highpass_info_fr_json,
  kr_volume_info_fr_json,kr_analog_info_fr_json,
  kr_adapter_path_direction_fr_json,kr_adapter_api_fr_json,
  kr_adapter_api_info_fr_json,kr_adapter_api_path_info_fr_json,
  kr_adapter_info_fr_json,kr_adapter_path_info_fr_json,
  krad_link_av_mode_t_fr_json,kr_txpdr_su_type_t_fr_json,
  krad_link_transport_mode_t_fr_json,kr_transponder_path_io_type_fr_json,
  kr_transponder_info_fr_json,kr_transponder_path_io_path_info_fr_json,
  kr_transponder_path_io_info_fr_json,kr_transponder_path_info_fr_json,
  kr_v4l2_state_fr_json,kr_v4l2_mode_fr_json,
  kr_v4l2_info_fr_json,kr_v4l2_open_info_fr_json,
  kr_wayland_info_fr_json,kr_wayland_path_info_fr_json,
  kr_x11_info_fr_json,kr_x11_path_info_fr_json
  };
  return fr_json_functions[uber->type-1](json , uber->actual);
}

