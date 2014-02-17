#include "gen.h"
#include "krad_ebmlx.h"
int info_unpack_fr_ebml(kr_ebml *ebml, uber_St *uber) {
  const info_unpack_fr_ebml_func fr_ebml_functions[59] = {kr_aux_info_fr_ebml,
  kr_aux_path_info_fr_ebml,kr_alsa_info_fr_ebml,
  kr_alsa_path_info_fr_ebml,kr_method_fr_ebml,
  kr_crate_fr_ebml,kr_easing_fr_ebml,
  kr_compositor_path_type_fr_ebml,kr_compositor_overlay_type_fr_ebml,
  kr_rect_fr_ebml,kr_compositor_output_info_fr_ebml,
  kr_compositor_bus_info_fr_ebml,kr_compositor_input_info_fr_ebml,
  kr_compositor_source_info_fr_ebml,kr_overlay_type_info_fr_ebml,
  kr_overlay_info_fr_ebml,kr_compositor_path_type_info_fr_ebml,
  kr_compositor_path_info_fr_ebml,kr_pos_fr_ebml,
  kr_perspective_view_fr_ebml,kr_sprite_info_fr_ebml,
  kr_text_info_fr_ebml,kr_vector_type_fr_ebml,
  kr_vector_info_fr_ebml,krad_codec_t_fr_ebml,
  krad_container_type_t_fr_ebml,kr_decklink_info_fr_ebml,
  kr_decklink_path_info_fr_ebml,kr_jack_direction_fr_ebml,
  kr_jack_state_fr_ebml,kr_jack_setup_info_fr_ebml,
  kr_jack_info_fr_ebml,kr_jack_path_info_fr_ebml,
  kr_mixer_channels_fr_ebml,kr_mixer_path_type_fr_ebml,
  kr_mixer_path_info_fr_ebml,kr_nil_fr_ebml,
  kr_payload_type_fr_ebml,kr_payload_fr_ebml,
  kr_sfx_control_fr_ebml,kr_sfx_effect_type_fr_ebml,
  kr_sfx_effect_control_fr_ebml,kr_eq_band_info_fr_ebml,
  kr_eq_info_fr_ebml,kr_lowpass_info_fr_ebml,
  kr_highpass_info_fr_ebml,kr_volume_info_fr_ebml,
  kr_analog_info_fr_ebml,kr_v4l2_state_fr_ebml,
  kr_v4l2_path_info_fr_ebml,kr_v4l2_info_fr_ebml,
  kr_wayland_state_fr_ebml,kr_wayland_info_fr_ebml,
  kr_wayland_path_info_fr_ebml,kr_x11_info_fr_ebml,
  kr_x11_path_info_fr_ebml,kr_xpdr_type_fr_ebml,
  kr_xpdr_type_info_fr_ebml,kr_xpdr_path_info_fr_ebml
  };
  return fr_ebml_functions[uber->type-1](ebml , uber->actual);
}

