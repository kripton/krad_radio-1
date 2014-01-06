#include "gen.h"
int info_pack_to_json(char *json, uber_St *uber, int32_t max);
typedef int (*info_pack_to_json_func)(char *json, void *st, int max);

int info_pack_to_json(char *json, uber_St *uber, int32_t max) {
  const info_pack_to_json_func to_json_functions[62] = {kr_alsa_info_to_json,
  kr_alsa_path_info_to_json,kr_app_method_to_json,
  kr_crate2_to_json,kr_compositor_path_type_to_json,
  kr_compositor_subunit_type_to_json,kr_vector_type_to_json,
  kr_compositor_control_to_json,kr_compositor_controls_to_json,
  kr_sprite_info_to_json,kr_text_info_to_json,
  kr_vector_info_to_json,kr_compositor_path_info_to_json,
  kr_compositor_info_to_json,kr_pos_to_json,
  kr_perspective_view_to_json,kr_perspective_to_json,
  krad_codec_t_to_json,krad_container_type_t_to_json,
  kr_decklink_info_to_json,kr_decklink_path_info_to_json,
  kr_jack_direction_to_json,kr_jack_state_to_json,
  kr_jack_setup_info_to_json,kr_jack_info_to_json,
  kr_jack_path_info_to_json,kr_mixer_channels_to_json,
  kr_mixer_control_to_json,kr_mixer_path_type_to_json,
  kr_mixer_adv_ctl_to_json,kr_mixer_info_to_json,
  kr_mixer_path_info_to_json,kr_sfx_control_to_json,
  kr_sfx_effect_type_to_json,kr_sfx_effect_control_to_json,
  kr_eq_band_info_to_json,kr_eq_info_to_json,
  kr_lowpass_info_to_json,kr_highpass_info_to_json,
  kr_analog_info_to_json,kr_adapter_path_direction_to_json,
  kr_adapter_api_to_json,kr_adapter_api_info_to_json,
  kr_adapter_api_path_info_to_json,kr_adapter_info_to_json,
  kr_adapter_path_info_to_json,krad_link_av_mode_t_to_json,
  kr_txpdr_su_type_t_to_json,krad_link_transport_mode_t_to_json,
  kr_transponder_path_io_type_to_json,kr_transponder_info_to_json,
  kr_transponder_path_io_path_info_to_json,kr_transponder_path_io_info_to_json,
  kr_transponder_path_info_to_json,kr_v4l2_state_to_json,
  kr_v4l2_mode_to_json,kr_v4l2_info_to_json,
  kr_v4l2_open_info_to_json,kr_wayland_info_to_json,
  kr_wayland_path_info_to_json,kr_x11_info_to_json,
  kr_x11_path_info_to_json};
  return to_json_functions[uber->type-1](json , uber->actual, max);
}

