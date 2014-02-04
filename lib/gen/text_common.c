#include "gen.h"
int info_pack_to_text(char *text, uber_St *uber, int32_t max) {
  const info_pack_to_text_func to_text_functions[65] = {kr_alsa_info_to_text,
  kr_alsa_path_info_to_text,kr_method_to_text,
  kr_crate_to_text,kr_easing_to_text,
  kr_compositor_path_type_to_text,kr_compositor_overlay_type_to_text,
  kr_rect_to_text,kr_compositor_output_info_to_text,
  kr_compositor_bus_info_to_text,kr_compositor_input_info_to_text,
  kr_compositor_source_info_to_text,kr_compositor_path_type_info_to_text,
  kr_compositor_path_info_to_text,kr_sprite_info_to_text,
  kr_text_info_to_text,kr_pos_to_text,
  kr_perspective_view_to_text,kr_perspective_to_text,
  krad_codec_t_to_text,krad_container_type_t_to_text,
  kr_decklink_info_to_text,kr_decklink_path_info_to_text,
  kr_jack_direction_to_text,kr_jack_state_to_text,
  kr_jack_setup_info_to_text,kr_jack_info_to_text,
  kr_jack_path_info_to_text,kr_mixer_channels_to_text,
  kr_mixer_path_type_to_text,kr_mixer_path_info_to_text,
  kr_nil_to_text,kr_payload_type_to_text,
  kr_payload_to_text,kr_sfx_control_to_text,
  kr_sfx_effect_type_to_text,kr_sfx_effect_control_to_text,
  kr_eq_band_info_to_text,kr_eq_info_to_text,
  kr_lowpass_info_to_text,kr_highpass_info_to_text,
  kr_volume_info_to_text,kr_analog_info_to_text,
  kr_adapter_path_direction_to_text,kr_adapter_api_to_text,
  kr_adapter_api_info_to_text,kr_adapter_api_path_info_to_text,
  kr_adapter_info_to_text,kr_adapter_path_info_to_text,
  krad_link_av_mode_t_to_text,kr_txpdr_su_type_t_to_text,
  krad_link_transport_mode_t_to_text,kr_transponder_path_io_type_to_text,
  kr_transponder_info_to_text,kr_transponder_path_io_path_info_to_text,
  kr_transponder_path_io_info_to_text,kr_transponder_path_info_to_text,
  kr_v4l2_state_to_text,kr_v4l2_mode_to_text,
  kr_v4l2_info_to_text,kr_v4l2_open_info_to_text,
  kr_wayland_info_to_text,kr_wayland_path_info_to_text,
  kr_x11_info_to_text,kr_x11_path_info_to_text
  };
  return to_text_functions[uber->type-1](text , uber->actual, max);
}

