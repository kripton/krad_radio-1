#include "bootstrapped.h"
#include <string.h>

char *codegen_enum_to_string(cgen_enum val) {
  switch (val) {
    case CGEN_KR_ALSA_INFO:
      return "kr_alsa_info";
    case CGEN_KR_ALSA_PATH_INFO:
      return "kr_alsa_path_info";
    case CGEN_KRAD_ALSA_SEQ_ST:
      return "krad_alsa_seq_St";
    case CGEN_KR_APP_METHOD:
      return "kr_app_method";
    case CGEN_KR_CRATE2:
      return "kr_crate2";
    case CGEN_KR_APP_SERVER_SETUP:
      return "kr_app_server_setup";
    case CGEN_KR_APP_SERVER_INFO:
      return "kr_app_server_info";
    case CGEN_KR_APP_CLIENT_TYPE:
      return "kr_app_client_type";
    case CGEN_KR_APP_SERVER_CLIENT_SETUP:
      return "kr_app_server_client_setup";
    case CGEN_KR_ROUTER_INFO:
      return "kr_router_info";
    case CGEN_KR_ROUTER_MAP_INFO:
      return "kr_router_map_info";
    case CGEN_KR_ROUTER_MAP_SETUP:
      return "kr_router_map_setup";
    case CGEN_KR_ROUTE_SETUP:
      return "kr_route_setup";
    case CGEN_KR_ROUTER_SETUP:
      return "kr_router_setup";
    case CGEN_KR_EASER_UPDATE:
      return "kr_easer_update";
    case CGEN_KR_EASER:
      return "kr_easer";
    case CGEN_KR_EASING:
      return "kr_easing";
    case CGEN_KR_TIMER:
      return "kr_timer";
    case CGEN_KR_MTYPE_T:
      return "kr_mtype_t";
    case CGEN_KR_CODEME_ST:
      return "kr_codeme_St";
    case CGEN_KR_IMAGE_ST:
      return "kr_image_St";
    case CGEN_KR_AUDIO_ST:
      return "kr_audio_St";
    case CGEN_KR_MEDIUM_ST:
      return "kr_medium_St";
    case CGEN_KR_ENCODER_MACHINE_SECTOR_ST:
      return "kr_encoder_machine_sector_St";
    case CGEN_KR_COMPOSITOR_CONTROL_EASERS:
      return "kr_compositor_control_easers";
    case CGEN_KR_COMPOSITOR_SETUP:
      return "kr_compositor_setup";
    case CGEN_KR_COMPOSITOR:
      return "kr_compositor";
    case CGEN_KR_COMPOSITOR_EVENT_TYPE:
      return "kr_compositor_event_type";
    case CGEN_KR_COMPOSITOR_EVENT:
      return "kr_compositor_event";
    case CGEN_KR_COMPOSITOR_SETTING:
      return "kr_compositor_setting";
    case CGEN_KR_COMPOSITOR_PATH_TYPE:
      return "kr_compositor_path_type";
    case CGEN_KR_COMPOSITOR_SUBUNIT_TYPE:
      return "kr_compositor_subunit_type";
    case CGEN_KR_VECTOR_TYPE:
      return "kr_vector_type";
    case CGEN_KR_COMPOSITOR_CONTROL:
      return "kr_compositor_control";
    case CGEN_KR_COMPOSITOR_CONTROLS:
      return "kr_compositor_controls";
    case CGEN_KR_SPRITE_INFO:
      return "kr_sprite_info";
    case CGEN_KR_TEXT_INFO:
      return "kr_text_info";
    case CGEN_KR_VECTOR_INFO:
      return "kr_vector_info";
    case CGEN_KR_COMPOSITOR_PATH_INFO:
      return "kr_compositor_path_info";
    case CGEN_KR_COMPOSITOR_INFO:
      return "kr_compositor_info";
    case CGEN_KR_COMPOSITOR_PATH_PATCH:
      return "kr_compositor_path_patch";
    case CGEN_COMPOSITOR_PATH_STATE:
      return "compositor_path_state";
    case CGEN_KR_COMPOSITOR_PATH_FRAME_CB_ARG:
      return "kr_compositor_path_frame_cb_arg";
    case CGEN_KR_COMPOSITOR_IO_PATH_SETUP:
      return "kr_compositor_io_path_setup";
    case CGEN_KR_CROP:
      return "kr_crop";
    case CGEN_KR_CONVERT:
      return "kr_convert";
    case CGEN_KR_POS:
      return "kr_pos";
    case CGEN_KR_PERSPECTIVE_VIEW:
      return "kr_perspective_view";
    case CGEN_KR_PERSPECTIVE:
      return "kr_perspective";
    case CGEN_KRAD_CODEC_T:
      return "krad_codec_t";
    case CGEN_KRAD_CONTAINER_TYPE_T:
      return "krad_container_type_t";
    case CGEN_KRAD_CONTAINER_ST:
      return "krad_container_St";
    case CGEN_KR_DECKLINK:
      return "kr_decklink";
    case CGEN_KRAD_DECKLINK_CAPTURE_ST:
      return "krad_decklink_capture_St";
    case CGEN_KR_DECKLINK_INFO:
      return "kr_decklink_info";
    case CGEN_KR_DECKLINK_PATH_INFO:
      return "kr_decklink_path_info";
    case CGEN_KR_EBML:
      return "kr_ebml";
    case CGEN_KR_FC2_ST:
      return "kr_fc2_St";
    case CGEN_KRAD_FLAC_ST:
      return "krad_flac_St";
    case CGEN_KR_FILE:
      return "kr_file";
    case CGEN_KRAD_IO_MODE_T:
      return "krad_io_mode_t";
    case CGEN_KR_IO2_ST:
      return "kr_io2_St";
    case CGEN_KR_STREAM:
      return "kr_stream";
    case CGEN_KR_JACK_EVENT:
      return "kr_jack_event";
    case CGEN_KR_JACK_PATH_EVENT_CB_ARG:
      return "kr_jack_path_event_cb_arg";
    case CGEN_KR_JACK_PATH_AUDIO_CB_ARG:
      return "kr_jack_path_audio_cb_arg";
    case CGEN_KR_JACK_EVENT_CB_ARG:
      return "kr_jack_event_cb_arg";
    case CGEN_KR_JACK_SETUP:
      return "kr_jack_setup";
    case CGEN_KR_JACK_PATH_SETUP:
      return "kr_jack_path_setup";
    case CGEN_KR_JACK_DIRECTION:
      return "kr_jack_direction";
    case CGEN_KR_JACK_STATE:
      return "kr_jack_state";
    case CGEN_KR_JACK_SETUP_INFO:
      return "kr_jack_setup_info";
    case CGEN_KR_JACK_INFO:
      return "kr_jack_info";
    case CGEN_KR_JACK_PATH_INFO:
      return "kr_jack_path_info";
    case CGEN_KRAD_LIST_FORMAT_T:
      return "krad_list_format_t";
    case CGEN_KRAD_LIST_PARSE_STATE_T:
      return "krad_list_parse_state_t";
    case CGEN_KRAD_LIST_T:
      return "krad_list_t";
    case CGEN_KRAD_WEBM_DASH_VOD_T:
      return "krad_webm_dash_vod_t";
    case CGEN_KR_POOL_SETUP:
      return "kr_pool_setup";
    case CGEN_KR_MIXER_EVENT_TYPE:
      return "kr_mixer_event_type";
    case CGEN_KR_MIXER_EVENT:
      return "kr_mixer_event";
    case CGEN_KR_MIXER_PATH_AUDIO_CB_ARG:
      return "kr_mixer_path_audio_cb_arg";
    case CGEN_KR_MIXER_IO_PATH_SETUP:
      return "kr_mixer_io_path_setup";
    case CGEN_KR_MIXER_SETUP:
      return "kr_mixer_setup";
    case CGEN_KR_MIXER_CHANNELS:
      return "kr_mixer_channels";
    case CGEN_KR_MIXER_CONTROL:
      return "kr_mixer_control";
    case CGEN_KR_MIXER_PATH_TYPE:
      return "kr_mixer_path_type";
    case CGEN_KR_MIXER_ADV_CTL:
      return "kr_mixer_adv_ctl";
    case CGEN_KR_MIXER_INFO:
      return "kr_mixer_info";
    case CGEN_KR_MIXER_PATH_INFO:
      return "kr_mixer_path_info";
    case CGEN_KR_MIXER_PATH_PATCH:
      return "kr_mixer_path_patch";
    case CGEN_KR_MKV_TRACK_ST:
      return "kr_mkv_track_St";
    case CGEN_KR_MKV_ST:
      return "kr_mkv_St";
    case CGEN_KR_MUXER_TRANSPORT_TYPE:
      return "kr_muxer_transport_type";
    case CGEN_KR_MUXER_FILE_OUTPUT_PARAMS_ST:
      return "kr_muxer_file_output_params_St";
    case CGEN_KR_MUXER_STREAM_OUTPUT_PARAMS_ST:
      return "kr_muxer_stream_output_params_St";
    case CGEN_KR_MUXER_TRANSMISSION_OUTPUT_PARAMS_ST:
      return "kr_muxer_transmission_output_params_St";
    case CGEN_KR_MUXER_TRANSPORT_PARAMS:
      return "kr_muxer_transport_params";
    case CGEN_KR_MUXER_OUTPUT_PARAMS_ST:
      return "kr_muxer_output_params_St";
    case CGEN_KRAD_OGG_TRACK_ST:
      return "krad_ogg_track_St";
    case CGEN_KRAD_OGG_ST:
      return "krad_ogg_St";
    case CGEN_KR_OGG:
      return "kr_ogg";
    case CGEN_KR_OGG_IO:
      return "kr_ogg_io";
    case CGEN_KRAD_OPUS_ST:
      return "krad_opus_St";
    case CGEN_KR_DECODER_STATE_T:
      return "kr_decoder_state_t";
    case CGEN_KR_DECODER_CMD_T:
      return "kr_decoder_cmd_t";
    case CGEN_KR_DDIR_T:
      return "kr_ddir_t";
    case CGEN_KR_DEMUXER_STATE_T:
      return "kr_demuxer_state_t";
    case CGEN_KR_DEMUXER_CMD_T:
      return "kr_demuxer_cmd_t";
    case CGEN_KR_DEMUXER_PARAMS_ST:
      return "kr_demuxer_params_St";
    case CGEN_KR_MACHINE_PARAMS_ST:
      return "kr_machine_params_St";
    case CGEN_KR_MACHINE_ST:
      return "kr_machine_St";
    case CGEN_KR_MSG_ST:
      return "kr_msg_St";
    case CGEN_KR_MSGPAIR_ST:
      return "kr_msgpair_St";
    case CGEN_KR_MSGSYS_ST:
      return "kr_msgsys_St";
    case CGEN_KR_DIRECTION:
      return "kr_direction";
    case CGEN_KR_PLAYER_PLAYBACK_STATE:
      return "kr_player_playback_state";
    case CGEN_KR_PLAYER_CMD:
      return "kr_player_cmd";
    case CGEN_KR_VIDEO_PARAMS_ST:
      return "kr_video_params_St";
    case CGEN_KR_AUDIO_PARAMS_ST:
      return "kr_audio_params_St";
    case CGEN_KR_MEDIUM_PARAMS_T:
      return "kr_medium_params_t";
    case CGEN_KR_TRACK_INFO_ST:
      return "kr_track_info_St";
    case CGEN_KR_UNIT_T:
      return "kr_unit_t";
    case CGEN_KR_MIXER_SUBUNIT_T:
      return "kr_mixer_subunit_t";
    case CGEN_KR_TRANSPONDER_SUBUNIT_TYPE_T:
      return "kr_transponder_subunit_type_t";
    case CGEN_KR_STATION_SUBUNIT_T:
      return "kr_station_subunit_t";
    case CGEN_KR_UNIT_CONTROL_DATA_T:
      return "kr_unit_control_data_t";
    case CGEN_KR_UNIT_CONTROL_VALUE_T:
      return "kr_unit_control_value_t";
    case CGEN_KR_TRANSPONDER_CONTROL_T:
      return "kr_transponder_control_t";
    case CGEN_KR_SUBUNIT_T:
      return "kr_subunit_t";
    case CGEN_KR_UNIT_CONTROL_NAME_T:
      return "kr_unit_control_name_t";
    case CGEN_KR_UNIT_PATH_ST:
      return "kr_unit_path_St";
    case CGEN_KR_UNIT_ID_T:
      return "kr_unit_id_t";
    case CGEN_KR_ADDRESS_ST:
      return "kr_address_St";
    case CGEN_KR_UNIT_CONTROL_ST:
      return "kr_unit_control_St";
    case CGEN_KR_REP_PTR_T:
      return "kr_rep_ptr_t";
    case CGEN_KR_REP_ACTUAL_T:
      return "kr_rep_actual_t";
    case CGEN_KR_CRATE:
      return "kr_crate";
    case CGEN_KR_CLIENT:
      return "kr_client";
    case CGEN_KR_NIL:
      return "kr_nil";
    case CGEN_KR_RADIO_PAYLOAD_TYPE:
      return "kr_radio_payload_type";
    case CGEN_KR_RADIO_PAYLOAD:
      return "kr_radio_payload";
    case CGEN_KRAD_RC_TYPE_T:
      return "krad_rc_type_t";
    case CGEN_KRAD_RC_ST:
      return "krad_rc_St";
    case CGEN_KRAD_RC_POLOLU_MAESTRO_ST:
      return "krad_rc_pololu_maestro_St";
    case CGEN_KRAD_RC_RX_ST:
      return "krad_rc_rx_St";
    case CGEN_KRAD_RC_SDL_JOY_ST:
      return "krad_rc_sdl_joy_St";
    case CGEN_KRAD_RC_TX_ST:
      return "krad_rc_tx_St";
    case CGEN_KRAD_RESAMPLE_RING_ST:
      return "krad_resample_ring_St";
    case CGEN_KR_PASS_INFO:
      return "kr_pass_info";
    case CGEN_KR_PASS_SETUP:
      return "kr_pass_setup";
    case CGEN_KR_PASS_CMD:
      return "kr_pass_cmd";
    case CGEN_KR_SFX_CMD:
      return "kr_sfx_cmd";
    case CGEN_KR_SFX_INFO_CB_ARG:
      return "kr_sfx_info_cb_arg";
    case CGEN_KR_SFX_SETUP:
      return "kr_sfx_setup";
    case CGEN_KR_SFX_CONTROL:
      return "kr_sfx_control";
    case CGEN_KR_SFX_EFFECT_TYPE:
      return "kr_sfx_effect_type";
    case CGEN_KR_SFX_EFFECT_CONTROL:
      return "kr_sfx_effect_control";
    case CGEN_KR_EQ_BAND_INFO:
      return "kr_eq_band_info";
    case CGEN_KR_EQ_INFO:
      return "kr_eq_info";
    case CGEN_KR_LOWPASS_INFO:
      return "kr_lowpass_info";
    case CGEN_KR_HIGHPASS_INFO:
      return "kr_highpass_info";
    case CGEN_KR_ANALOG_INFO:
      return "kr_analog_info";
    case CGEN_TONE_T:
      return "tone_t";
    case CGEN_KRAD_TONE_T:
      return "krad_tone_t";
    case CGEN_KRAD_CONTROL_ST:
      return "krad_control_St";
    case CGEN_KR_TAG:
      return "kr_tag";
    case CGEN_KR_TAGS:
      return "kr_tags";
    case CGEN_KRAD_THEORA_ENCODER_ST:
      return "krad_theora_encoder_St";
    case CGEN_KRAD_THEORA_DECODER_ST:
      return "krad_theora_decoder_St";
    case CGEN_KR_ADAPTER_PATH_EVENT_CB_ARG:
      return "kr_adapter_path_event_cb_arg";
    case CGEN_KR_ADAPTER_PATH_AV_CB_ARG:
      return "kr_adapter_path_av_cb_arg";
    case CGEN_KR_ADAPTER_EVENT_CB_ARG:
      return "kr_adapter_event_cb_arg";
    case CGEN_KR_ADAPTER_PATH_SETUP:
      return "kr_adapter_path_setup";
    case CGEN_KR_ADAPTER_SETUP:
      return "kr_adapter_setup";
    case CGEN_KR_ADAPTER_PATH_DIRECTION:
      return "kr_adapter_path_direction";
    case CGEN_KR_ADAPTER_API:
      return "kr_adapter_api";
    case CGEN_KR_ADAPTER_API_INFO:
      return "kr_adapter_api_info";
    case CGEN_KR_ADAPTER_API_PATH_INFO:
      return "kr_adapter_api_path_info";
    case CGEN_KR_ADAPTER_INFO:
      return "kr_adapter_info";
    case CGEN_KR_ADAPTER_PATH_INFO:
      return "kr_adapter_path_info";
    case CGEN_KR_IMAGE:
      return "kr_image";
    case CGEN_KR_AUDIO:
      return "kr_audio";
    case CGEN_KR_SLICE_ST:
      return "kr_slice_St";
    case CGEN_KR_TRANSPONDER_EVENT_TYPE:
      return "kr_transponder_event_type";
    case CGEN_KR_TRANSPONDER_EVENT:
      return "kr_transponder_event";
    case CGEN_KR_TRANSPONDER_SETUP:
      return "kr_transponder_setup";
    case CGEN_KRAD_LINK_AV_MODE_T:
      return "krad_link_av_mode_t";
    case CGEN_KR_TXPDR_SU_TYPE_T:
      return "kr_txpdr_su_type_t";
    case CGEN_KRAD_LINK_TRANSPORT_MODE_T:
      return "krad_link_transport_mode_t";
    case CGEN_KR_TRANSPONDER_PATH_IO_TYPE:
      return "kr_transponder_path_io_type";
    case CGEN_KR_TRANSPONDER_INFO:
      return "kr_transponder_info";
    case CGEN_KR_TRANSPONDER_PATH_IO_PATH_INFO:
      return "kr_transponder_path_io_path_info";
    case CGEN_KR_TRANSPONDER_PATH_IO_INFO:
      return "kr_transponder_path_io_info";
    case CGEN_KR_TRANSPONDER_PATH_INFO:
      return "kr_transponder_path_info";
    case CGEN_KR_TRANSPONDER_PATH_PATCH:
      return "kr_transponder_path_patch";
    case CGEN_KRAD_SLICE_TRACK_TYPE_T:
      return "krad_slice_track_type_t";
    case CGEN_KRAD_SUBSLICE_ST:
      return "krad_subslice_St";
    case CGEN_KRAD_SLICER_ST:
      return "krad_slicer_St";
    case CGEN_KRAD_REBUILDER_TRACK_ST:
      return "krad_rebuilder_track_St";
    case CGEN_KRAD_REBUILDER_ST:
      return "krad_rebuilder_St";
    case CGEN_KRAD_UDP_ST:
      return "krad_udp_St";
    case CGEN_KR_V4L2_SETUP:
      return "kr_v4l2_setup";
    case CGEN_KR_V4L2_STATE:
      return "kr_v4l2_state";
    case CGEN_KR_V4L2_MODE:
      return "kr_v4l2_mode";
    case CGEN_KR_V4L2_INFO:
      return "kr_v4l2_info";
    case CGEN_KR_V4L2_OPEN_INFO:
      return "kr_v4l2_open_info";
    case CGEN_KRAD_VORBIS_ST:
      return "krad_vorbis_St";
    case CGEN_KRAD_VPX_ENCODER_ST:
      return "krad_vpx_encoder_St";
    case CGEN_KRAD_VPX_DECODER_ST:
      return "krad_vpx_decoder_St";
    case CGEN_KR_WAYLAND_EVENT_TYPE:
      return "kr_wayland_event_type";
    case CGEN_KR_WAYLAND_POINTER_EVENT:
      return "kr_wayland_pointer_event";
    case CGEN_KR_WAYLAND_KEY_EVENT:
      return "kr_wayland_key_event";
    case CGEN_KR_WAYLAND_FRAME_EVENT:
      return "kr_wayland_frame_event";
    case CGEN_KR_WAYLAND_EVENT:
      return "kr_wayland_event";
    case CGEN_KR_WAYLAND_PATH_SETUP:
      return "kr_wayland_path_setup";
    case CGEN_KR_WAYLAND_SETUP:
      return "kr_wayland_setup";
    case CGEN_KR_WAYLAND_INFO:
      return "kr_wayland_info";
    case CGEN_KR_WAYLAND_PATH_INFO:
      return "kr_wayland_path_info";
    case CGEN_KRAD_INTERWEB_SHUTDOWN:
      return "krad_interweb_shutdown";
    case CGEN_KR_WEB_EVENT_TYPE:
      return "kr_web_event_type";
    case CGEN_KR_WEB_EVENT:
      return "kr_web_event";
    case CGEN_KR_WEB_SERVER_SETUP:
      return "kr_web_server_setup";
    case CGEN_KR_WEBRTC_USER:
      return "kr_webrtc_user";
    case CGEN_KR_WEBRTC_SIGNAL_TYPE:
      return "kr_webrtc_signal_type";
    case CGEN_KR_WEBRTC_SIGNAL:
      return "kr_webrtc_signal";
    case CGEN_KR_WEB_SERVER:
      return "kr_web_server";
    case CGEN_KR_WEB_CLIENT_TYPE:
      return "kr_web_client_type";
    case CGEN_KR_WEB_VERB:
      return "kr_web_verb";
    case CGEN_KR_WEBSOCKET_CLIENT:
      return "kr_websocket_client";
    case CGEN_KR_WEB_CLIENT:
      return "kr_web_client";
    case CGEN_KR_X11:
      return "kr_x11";
    case CGEN_KR_X11_PATH:
      return "kr_x11_path";
    case CGEN_KR_X11_INFO:
      return "kr_x11_info";
    case CGEN_KR_X11_PATH_INFO:
      return "kr_x11_path_info";
    case CGEN_KRAD_XMMS_PLAYBACK_CMD_T:
      return "krad_xmms_playback_cmd_t";
    case CGEN_KRAD_XMMS_ST:
      return "krad_xmms_St";
    case CGEN_KRAD_Y4M_ST:
      return "krad_y4m_St";
  }
  return "Unknown";
}

cgen_enum codegen_string_to_enum(char *string) {
  if (!strcmp(string,"kr_alsa_info")) {
    return CGEN_KR_ALSA_INFO;
  }
  if (!strcmp(string,"kr_alsa_path_info")) {
    return CGEN_KR_ALSA_PATH_INFO;
  }
  if (!strcmp(string,"krad_alsa_seq_St")) {
    return CGEN_KRAD_ALSA_SEQ_ST;
  }
  if (!strcmp(string,"kr_app_method")) {
    return CGEN_KR_APP_METHOD;
  }
  if (!strcmp(string,"kr_crate2")) {
    return CGEN_KR_CRATE2;
  }
  if (!strcmp(string,"kr_app_server_setup")) {
    return CGEN_KR_APP_SERVER_SETUP;
  }
  if (!strcmp(string,"kr_app_server_info")) {
    return CGEN_KR_APP_SERVER_INFO;
  }
  if (!strcmp(string,"kr_app_client_type")) {
    return CGEN_KR_APP_CLIENT_TYPE;
  }
  if (!strcmp(string,"kr_app_server_client_setup")) {
    return CGEN_KR_APP_SERVER_CLIENT_SETUP;
  }
  if (!strcmp(string,"kr_router_info")) {
    return CGEN_KR_ROUTER_INFO;
  }
  if (!strcmp(string,"kr_router_map_info")) {
    return CGEN_KR_ROUTER_MAP_INFO;
  }
  if (!strcmp(string,"kr_router_map_setup")) {
    return CGEN_KR_ROUTER_MAP_SETUP;
  }
  if (!strcmp(string,"kr_route_setup")) {
    return CGEN_KR_ROUTE_SETUP;
  }
  if (!strcmp(string,"kr_router_setup")) {
    return CGEN_KR_ROUTER_SETUP;
  }
  if (!strcmp(string,"kr_easer_update")) {
    return CGEN_KR_EASER_UPDATE;
  }
  if (!strcmp(string,"kr_easer")) {
    return CGEN_KR_EASER;
  }
  if (!strcmp(string,"kr_easing")) {
    return CGEN_KR_EASING;
  }
  if (!strcmp(string,"kr_timer")) {
    return CGEN_KR_TIMER;
  }
  if (!strcmp(string,"kr_mtype_t")) {
    return CGEN_KR_MTYPE_T;
  }
  if (!strcmp(string,"kr_codeme_St")) {
    return CGEN_KR_CODEME_ST;
  }
  if (!strcmp(string,"kr_image_St")) {
    return CGEN_KR_IMAGE_ST;
  }
  if (!strcmp(string,"kr_audio_St")) {
    return CGEN_KR_AUDIO_ST;
  }
  if (!strcmp(string,"kr_medium_St")) {
    return CGEN_KR_MEDIUM_ST;
  }
  if (!strcmp(string,"kr_encoder_machine_sector_St")) {
    return CGEN_KR_ENCODER_MACHINE_SECTOR_ST;
  }
  if (!strcmp(string,"kr_compositor_control_easers")) {
    return CGEN_KR_COMPOSITOR_CONTROL_EASERS;
  }
  if (!strcmp(string,"kr_compositor_setup")) {
    return CGEN_KR_COMPOSITOR_SETUP;
  }
  if (!strcmp(string,"kr_compositor")) {
    return CGEN_KR_COMPOSITOR;
  }
  if (!strcmp(string,"kr_compositor_event_type")) {
    return CGEN_KR_COMPOSITOR_EVENT_TYPE;
  }
  if (!strcmp(string,"kr_compositor_event")) {
    return CGEN_KR_COMPOSITOR_EVENT;
  }
  if (!strcmp(string,"kr_compositor_setting")) {
    return CGEN_KR_COMPOSITOR_SETTING;
  }
  if (!strcmp(string,"kr_compositor_path_type")) {
    return CGEN_KR_COMPOSITOR_PATH_TYPE;
  }
  if (!strcmp(string,"kr_compositor_subunit_type")) {
    return CGEN_KR_COMPOSITOR_SUBUNIT_TYPE;
  }
  if (!strcmp(string,"kr_vector_type")) {
    return CGEN_KR_VECTOR_TYPE;
  }
  if (!strcmp(string,"kr_compositor_control")) {
    return CGEN_KR_COMPOSITOR_CONTROL;
  }
  if (!strcmp(string,"kr_compositor_controls")) {
    return CGEN_KR_COMPOSITOR_CONTROLS;
  }
  if (!strcmp(string,"kr_sprite_info")) {
    return CGEN_KR_SPRITE_INFO;
  }
  if (!strcmp(string,"kr_text_info")) {
    return CGEN_KR_TEXT_INFO;
  }
  if (!strcmp(string,"kr_vector_info")) {
    return CGEN_KR_VECTOR_INFO;
  }
  if (!strcmp(string,"kr_compositor_path_info")) {
    return CGEN_KR_COMPOSITOR_PATH_INFO;
  }
  if (!strcmp(string,"kr_compositor_info")) {
    return CGEN_KR_COMPOSITOR_INFO;
  }
  if (!strcmp(string,"kr_compositor_path_patch")) {
    return CGEN_KR_COMPOSITOR_PATH_PATCH;
  }
  if (!strcmp(string,"compositor_path_state")) {
    return CGEN_COMPOSITOR_PATH_STATE;
  }
  if (!strcmp(string,"kr_compositor_path_frame_cb_arg")) {
    return CGEN_KR_COMPOSITOR_PATH_FRAME_CB_ARG;
  }
  if (!strcmp(string,"kr_compositor_io_path_setup")) {
    return CGEN_KR_COMPOSITOR_IO_PATH_SETUP;
  }
  if (!strcmp(string,"kr_crop")) {
    return CGEN_KR_CROP;
  }
  if (!strcmp(string,"kr_convert")) {
    return CGEN_KR_CONVERT;
  }
  if (!strcmp(string,"kr_pos")) {
    return CGEN_KR_POS;
  }
  if (!strcmp(string,"kr_perspective_view")) {
    return CGEN_KR_PERSPECTIVE_VIEW;
  }
  if (!strcmp(string,"kr_perspective")) {
    return CGEN_KR_PERSPECTIVE;
  }
  if (!strcmp(string,"krad_codec_t")) {
    return CGEN_KRAD_CODEC_T;
  }
  if (!strcmp(string,"krad_container_type_t")) {
    return CGEN_KRAD_CONTAINER_TYPE_T;
  }
  if (!strcmp(string,"krad_container_St")) {
    return CGEN_KRAD_CONTAINER_ST;
  }
  if (!strcmp(string,"kr_decklink")) {
    return CGEN_KR_DECKLINK;
  }
  if (!strcmp(string,"krad_decklink_capture_St")) {
    return CGEN_KRAD_DECKLINK_CAPTURE_ST;
  }
  if (!strcmp(string,"kr_decklink_info")) {
    return CGEN_KR_DECKLINK_INFO;
  }
  if (!strcmp(string,"kr_decklink_path_info")) {
    return CGEN_KR_DECKLINK_PATH_INFO;
  }
  if (!strcmp(string,"kr_ebml")) {
    return CGEN_KR_EBML;
  }
  if (!strcmp(string,"kr_fc2_St")) {
    return CGEN_KR_FC2_ST;
  }
  if (!strcmp(string,"krad_flac_St")) {
    return CGEN_KRAD_FLAC_ST;
  }
  if (!strcmp(string,"kr_file")) {
    return CGEN_KR_FILE;
  }
  if (!strcmp(string,"krad_io_mode_t")) {
    return CGEN_KRAD_IO_MODE_T;
  }
  if (!strcmp(string,"kr_io2_St")) {
    return CGEN_KR_IO2_ST;
  }
  if (!strcmp(string,"kr_stream")) {
    return CGEN_KR_STREAM;
  }
  if (!strcmp(string,"kr_jack_event")) {
    return CGEN_KR_JACK_EVENT;
  }
  if (!strcmp(string,"kr_jack_path_event_cb_arg")) {
    return CGEN_KR_JACK_PATH_EVENT_CB_ARG;
  }
  if (!strcmp(string,"kr_jack_path_audio_cb_arg")) {
    return CGEN_KR_JACK_PATH_AUDIO_CB_ARG;
  }
  if (!strcmp(string,"kr_jack_event_cb_arg")) {
    return CGEN_KR_JACK_EVENT_CB_ARG;
  }
  if (!strcmp(string,"kr_jack_setup")) {
    return CGEN_KR_JACK_SETUP;
  }
  if (!strcmp(string,"kr_jack_path_setup")) {
    return CGEN_KR_JACK_PATH_SETUP;
  }
  if (!strcmp(string,"kr_jack_direction")) {
    return CGEN_KR_JACK_DIRECTION;
  }
  if (!strcmp(string,"kr_jack_state")) {
    return CGEN_KR_JACK_STATE;
  }
  if (!strcmp(string,"kr_jack_setup_info")) {
    return CGEN_KR_JACK_SETUP_INFO;
  }
  if (!strcmp(string,"kr_jack_info")) {
    return CGEN_KR_JACK_INFO;
  }
  if (!strcmp(string,"kr_jack_path_info")) {
    return CGEN_KR_JACK_PATH_INFO;
  }
  if (!strcmp(string,"krad_list_format_t")) {
    return CGEN_KRAD_LIST_FORMAT_T;
  }
  if (!strcmp(string,"krad_list_parse_state_t")) {
    return CGEN_KRAD_LIST_PARSE_STATE_T;
  }
  if (!strcmp(string,"krad_list_t")) {
    return CGEN_KRAD_LIST_T;
  }
  if (!strcmp(string,"krad_webm_dash_vod_t")) {
    return CGEN_KRAD_WEBM_DASH_VOD_T;
  }
  if (!strcmp(string,"kr_pool_setup")) {
    return CGEN_KR_POOL_SETUP;
  }
  if (!strcmp(string,"kr_mixer_event_type")) {
    return CGEN_KR_MIXER_EVENT_TYPE;
  }
  if (!strcmp(string,"kr_mixer_event")) {
    return CGEN_KR_MIXER_EVENT;
  }
  if (!strcmp(string,"kr_mixer_path_audio_cb_arg")) {
    return CGEN_KR_MIXER_PATH_AUDIO_CB_ARG;
  }
  if (!strcmp(string,"kr_mixer_io_path_setup")) {
    return CGEN_KR_MIXER_IO_PATH_SETUP;
  }
  if (!strcmp(string,"kr_mixer_setup")) {
    return CGEN_KR_MIXER_SETUP;
  }
  if (!strcmp(string,"kr_mixer_channels")) {
    return CGEN_KR_MIXER_CHANNELS;
  }
  if (!strcmp(string,"kr_mixer_control")) {
    return CGEN_KR_MIXER_CONTROL;
  }
  if (!strcmp(string,"kr_mixer_path_type")) {
    return CGEN_KR_MIXER_PATH_TYPE;
  }
  if (!strcmp(string,"kr_mixer_adv_ctl")) {
    return CGEN_KR_MIXER_ADV_CTL;
  }
  if (!strcmp(string,"kr_mixer_info")) {
    return CGEN_KR_MIXER_INFO;
  }
  if (!strcmp(string,"kr_mixer_path_info")) {
    return CGEN_KR_MIXER_PATH_INFO;
  }
  if (!strcmp(string,"kr_mixer_path_patch")) {
    return CGEN_KR_MIXER_PATH_PATCH;
  }
  if (!strcmp(string,"kr_mkv_track_St")) {
    return CGEN_KR_MKV_TRACK_ST;
  }
  if (!strcmp(string,"kr_mkv_St")) {
    return CGEN_KR_MKV_ST;
  }
  if (!strcmp(string,"kr_muxer_transport_type")) {
    return CGEN_KR_MUXER_TRANSPORT_TYPE;
  }
  if (!strcmp(string,"kr_muxer_file_output_params_St")) {
    return CGEN_KR_MUXER_FILE_OUTPUT_PARAMS_ST;
  }
  if (!strcmp(string,"kr_muxer_stream_output_params_St")) {
    return CGEN_KR_MUXER_STREAM_OUTPUT_PARAMS_ST;
  }
  if (!strcmp(string,"kr_muxer_transmission_output_params_St")) {
    return CGEN_KR_MUXER_TRANSMISSION_OUTPUT_PARAMS_ST;
  }
  if (!strcmp(string,"kr_muxer_transport_params")) {
    return CGEN_KR_MUXER_TRANSPORT_PARAMS;
  }
  if (!strcmp(string,"kr_muxer_output_params_St")) {
    return CGEN_KR_MUXER_OUTPUT_PARAMS_ST;
  }
  if (!strcmp(string,"krad_ogg_track_St")) {
    return CGEN_KRAD_OGG_TRACK_ST;
  }
  if (!strcmp(string,"krad_ogg_St")) {
    return CGEN_KRAD_OGG_ST;
  }
  if (!strcmp(string,"kr_ogg")) {
    return CGEN_KR_OGG;
  }
  if (!strcmp(string,"kr_ogg_io")) {
    return CGEN_KR_OGG_IO;
  }
  if (!strcmp(string,"krad_opus_St")) {
    return CGEN_KRAD_OPUS_ST;
  }
  if (!strcmp(string,"kr_decoder_state_t")) {
    return CGEN_KR_DECODER_STATE_T;
  }
  if (!strcmp(string,"kr_decoder_cmd_t")) {
    return CGEN_KR_DECODER_CMD_T;
  }
  if (!strcmp(string,"kr_ddir_t")) {
    return CGEN_KR_DDIR_T;
  }
  if (!strcmp(string,"kr_demuxer_state_t")) {
    return CGEN_KR_DEMUXER_STATE_T;
  }
  if (!strcmp(string,"kr_demuxer_cmd_t")) {
    return CGEN_KR_DEMUXER_CMD_T;
  }
  if (!strcmp(string,"kr_demuxer_params_St")) {
    return CGEN_KR_DEMUXER_PARAMS_ST;
  }
  if (!strcmp(string,"kr_machine_params_St")) {
    return CGEN_KR_MACHINE_PARAMS_ST;
  }
  if (!strcmp(string,"kr_machine_St")) {
    return CGEN_KR_MACHINE_ST;
  }
  if (!strcmp(string,"kr_msg_St")) {
    return CGEN_KR_MSG_ST;
  }
  if (!strcmp(string,"kr_msgpair_St")) {
    return CGEN_KR_MSGPAIR_ST;
  }
  if (!strcmp(string,"kr_msgsys_St")) {
    return CGEN_KR_MSGSYS_ST;
  }
  if (!strcmp(string,"kr_direction")) {
    return CGEN_KR_DIRECTION;
  }
  if (!strcmp(string,"kr_player_playback_state")) {
    return CGEN_KR_PLAYER_PLAYBACK_STATE;
  }
  if (!strcmp(string,"kr_player_cmd")) {
    return CGEN_KR_PLAYER_CMD;
  }
  if (!strcmp(string,"kr_video_params_St")) {
    return CGEN_KR_VIDEO_PARAMS_ST;
  }
  if (!strcmp(string,"kr_audio_params_St")) {
    return CGEN_KR_AUDIO_PARAMS_ST;
  }
  if (!strcmp(string,"kr_medium_params_t")) {
    return CGEN_KR_MEDIUM_PARAMS_T;
  }
  if (!strcmp(string,"kr_track_info_St")) {
    return CGEN_KR_TRACK_INFO_ST;
  }
  if (!strcmp(string,"kr_unit_t")) {
    return CGEN_KR_UNIT_T;
  }
  if (!strcmp(string,"kr_mixer_subunit_t")) {
    return CGEN_KR_MIXER_SUBUNIT_T;
  }
  if (!strcmp(string,"kr_transponder_subunit_type_t")) {
    return CGEN_KR_TRANSPONDER_SUBUNIT_TYPE_T;
  }
  if (!strcmp(string,"kr_station_subunit_t")) {
    return CGEN_KR_STATION_SUBUNIT_T;
  }
  if (!strcmp(string,"kr_unit_control_data_t")) {
    return CGEN_KR_UNIT_CONTROL_DATA_T;
  }
  if (!strcmp(string,"kr_unit_control_value_t")) {
    return CGEN_KR_UNIT_CONTROL_VALUE_T;
  }
  if (!strcmp(string,"kr_transponder_control_t")) {
    return CGEN_KR_TRANSPONDER_CONTROL_T;
  }
  if (!strcmp(string,"kr_subunit_t")) {
    return CGEN_KR_SUBUNIT_T;
  }
  if (!strcmp(string,"kr_unit_control_name_t")) {
    return CGEN_KR_UNIT_CONTROL_NAME_T;
  }
  if (!strcmp(string,"kr_unit_path_St")) {
    return CGEN_KR_UNIT_PATH_ST;
  }
  if (!strcmp(string,"kr_unit_id_t")) {
    return CGEN_KR_UNIT_ID_T;
  }
  if (!strcmp(string,"kr_address_St")) {
    return CGEN_KR_ADDRESS_ST;
  }
  if (!strcmp(string,"kr_unit_control_St")) {
    return CGEN_KR_UNIT_CONTROL_ST;
  }
  if (!strcmp(string,"kr_rep_ptr_t")) {
    return CGEN_KR_REP_PTR_T;
  }
  if (!strcmp(string,"kr_rep_actual_t")) {
    return CGEN_KR_REP_ACTUAL_T;
  }
  if (!strcmp(string,"kr_crate")) {
    return CGEN_KR_CRATE;
  }
  if (!strcmp(string,"kr_client")) {
    return CGEN_KR_CLIENT;
  }
  if (!strcmp(string,"kr_nil")) {
    return CGEN_KR_NIL;
  }
  if (!strcmp(string,"kr_radio_payload_type")) {
    return CGEN_KR_RADIO_PAYLOAD_TYPE;
  }
  if (!strcmp(string,"kr_radio_payload")) {
    return CGEN_KR_RADIO_PAYLOAD;
  }
  if (!strcmp(string,"krad_rc_type_t")) {
    return CGEN_KRAD_RC_TYPE_T;
  }
  if (!strcmp(string,"krad_rc_St")) {
    return CGEN_KRAD_RC_ST;
  }
  if (!strcmp(string,"krad_rc_pololu_maestro_St")) {
    return CGEN_KRAD_RC_POLOLU_MAESTRO_ST;
  }
  if (!strcmp(string,"krad_rc_rx_St")) {
    return CGEN_KRAD_RC_RX_ST;
  }
  if (!strcmp(string,"krad_rc_sdl_joy_St")) {
    return CGEN_KRAD_RC_SDL_JOY_ST;
  }
  if (!strcmp(string,"krad_rc_tx_St")) {
    return CGEN_KRAD_RC_TX_ST;
  }
  if (!strcmp(string,"krad_resample_ring_St")) {
    return CGEN_KRAD_RESAMPLE_RING_ST;
  }
  if (!strcmp(string,"kr_pass_info")) {
    return CGEN_KR_PASS_INFO;
  }
  if (!strcmp(string,"kr_pass_setup")) {
    return CGEN_KR_PASS_SETUP;
  }
  if (!strcmp(string,"kr_pass_cmd")) {
    return CGEN_KR_PASS_CMD;
  }
  if (!strcmp(string,"kr_sfx_cmd")) {
    return CGEN_KR_SFX_CMD;
  }
  if (!strcmp(string,"kr_sfx_info_cb_arg")) {
    return CGEN_KR_SFX_INFO_CB_ARG;
  }
  if (!strcmp(string,"kr_sfx_setup")) {
    return CGEN_KR_SFX_SETUP;
  }
  if (!strcmp(string,"kr_sfx_control")) {
    return CGEN_KR_SFX_CONTROL;
  }
  if (!strcmp(string,"kr_sfx_effect_type")) {
    return CGEN_KR_SFX_EFFECT_TYPE;
  }
  if (!strcmp(string,"kr_sfx_effect_control")) {
    return CGEN_KR_SFX_EFFECT_CONTROL;
  }
  if (!strcmp(string,"kr_eq_band_info")) {
    return CGEN_KR_EQ_BAND_INFO;
  }
  if (!strcmp(string,"kr_eq_info")) {
    return CGEN_KR_EQ_INFO;
  }
  if (!strcmp(string,"kr_lowpass_info")) {
    return CGEN_KR_LOWPASS_INFO;
  }
  if (!strcmp(string,"kr_highpass_info")) {
    return CGEN_KR_HIGHPASS_INFO;
  }
  if (!strcmp(string,"kr_analog_info")) {
    return CGEN_KR_ANALOG_INFO;
  }
  if (!strcmp(string,"tone_t")) {
    return CGEN_TONE_T;
  }
  if (!strcmp(string,"krad_tone_t")) {
    return CGEN_KRAD_TONE_T;
  }
  if (!strcmp(string,"krad_control_St")) {
    return CGEN_KRAD_CONTROL_ST;
  }
  if (!strcmp(string,"kr_tag")) {
    return CGEN_KR_TAG;
  }
  if (!strcmp(string,"kr_tags")) {
    return CGEN_KR_TAGS;
  }
  if (!strcmp(string,"krad_theora_encoder_St")) {
    return CGEN_KRAD_THEORA_ENCODER_ST;
  }
  if (!strcmp(string,"krad_theora_decoder_St")) {
    return CGEN_KRAD_THEORA_DECODER_ST;
  }
  if (!strcmp(string,"kr_adapter_path_event_cb_arg")) {
    return CGEN_KR_ADAPTER_PATH_EVENT_CB_ARG;
  }
  if (!strcmp(string,"kr_adapter_path_av_cb_arg")) {
    return CGEN_KR_ADAPTER_PATH_AV_CB_ARG;
  }
  if (!strcmp(string,"kr_adapter_event_cb_arg")) {
    return CGEN_KR_ADAPTER_EVENT_CB_ARG;
  }
  if (!strcmp(string,"kr_adapter_path_setup")) {
    return CGEN_KR_ADAPTER_PATH_SETUP;
  }
  if (!strcmp(string,"kr_adapter_setup")) {
    return CGEN_KR_ADAPTER_SETUP;
  }
  if (!strcmp(string,"kr_adapter_path_direction")) {
    return CGEN_KR_ADAPTER_PATH_DIRECTION;
  }
  if (!strcmp(string,"kr_adapter_api")) {
    return CGEN_KR_ADAPTER_API;
  }
  if (!strcmp(string,"kr_adapter_api_info")) {
    return CGEN_KR_ADAPTER_API_INFO;
  }
  if (!strcmp(string,"kr_adapter_api_path_info")) {
    return CGEN_KR_ADAPTER_API_PATH_INFO;
  }
  if (!strcmp(string,"kr_adapter_info")) {
    return CGEN_KR_ADAPTER_INFO;
  }
  if (!strcmp(string,"kr_adapter_path_info")) {
    return CGEN_KR_ADAPTER_PATH_INFO;
  }
  if (!strcmp(string,"kr_image")) {
    return CGEN_KR_IMAGE;
  }
  if (!strcmp(string,"kr_audio")) {
    return CGEN_KR_AUDIO;
  }
  if (!strcmp(string,"kr_slice_St")) {
    return CGEN_KR_SLICE_ST;
  }
  if (!strcmp(string,"kr_transponder_event_type")) {
    return CGEN_KR_TRANSPONDER_EVENT_TYPE;
  }
  if (!strcmp(string,"kr_transponder_event")) {
    return CGEN_KR_TRANSPONDER_EVENT;
  }
  if (!strcmp(string,"kr_transponder_setup")) {
    return CGEN_KR_TRANSPONDER_SETUP;
  }
  if (!strcmp(string,"krad_link_av_mode_t")) {
    return CGEN_KRAD_LINK_AV_MODE_T;
  }
  if (!strcmp(string,"kr_txpdr_su_type_t")) {
    return CGEN_KR_TXPDR_SU_TYPE_T;
  }
  if (!strcmp(string,"krad_link_transport_mode_t")) {
    return CGEN_KRAD_LINK_TRANSPORT_MODE_T;
  }
  if (!strcmp(string,"kr_transponder_path_io_type")) {
    return CGEN_KR_TRANSPONDER_PATH_IO_TYPE;
  }
  if (!strcmp(string,"kr_transponder_info")) {
    return CGEN_KR_TRANSPONDER_INFO;
  }
  if (!strcmp(string,"kr_transponder_path_io_path_info")) {
    return CGEN_KR_TRANSPONDER_PATH_IO_PATH_INFO;
  }
  if (!strcmp(string,"kr_transponder_path_io_info")) {
    return CGEN_KR_TRANSPONDER_PATH_IO_INFO;
  }
  if (!strcmp(string,"kr_transponder_path_info")) {
    return CGEN_KR_TRANSPONDER_PATH_INFO;
  }
  if (!strcmp(string,"kr_transponder_path_patch")) {
    return CGEN_KR_TRANSPONDER_PATH_PATCH;
  }
  if (!strcmp(string,"krad_slice_track_type_t")) {
    return CGEN_KRAD_SLICE_TRACK_TYPE_T;
  }
  if (!strcmp(string,"krad_subslice_St")) {
    return CGEN_KRAD_SUBSLICE_ST;
  }
  if (!strcmp(string,"krad_slicer_St")) {
    return CGEN_KRAD_SLICER_ST;
  }
  if (!strcmp(string,"krad_rebuilder_track_St")) {
    return CGEN_KRAD_REBUILDER_TRACK_ST;
  }
  if (!strcmp(string,"krad_rebuilder_St")) {
    return CGEN_KRAD_REBUILDER_ST;
  }
  if (!strcmp(string,"krad_udp_St")) {
    return CGEN_KRAD_UDP_ST;
  }
  if (!strcmp(string,"kr_v4l2_setup")) {
    return CGEN_KR_V4L2_SETUP;
  }
  if (!strcmp(string,"kr_v4l2_state")) {
    return CGEN_KR_V4L2_STATE;
  }
  if (!strcmp(string,"kr_v4l2_mode")) {
    return CGEN_KR_V4L2_MODE;
  }
  if (!strcmp(string,"kr_v4l2_info")) {
    return CGEN_KR_V4L2_INFO;
  }
  if (!strcmp(string,"kr_v4l2_open_info")) {
    return CGEN_KR_V4L2_OPEN_INFO;
  }
  if (!strcmp(string,"krad_vorbis_St")) {
    return CGEN_KRAD_VORBIS_ST;
  }
  if (!strcmp(string,"krad_vpx_encoder_St")) {
    return CGEN_KRAD_VPX_ENCODER_ST;
  }
  if (!strcmp(string,"krad_vpx_decoder_St")) {
    return CGEN_KRAD_VPX_DECODER_ST;
  }
  if (!strcmp(string,"kr_wayland_event_type")) {
    return CGEN_KR_WAYLAND_EVENT_TYPE;
  }
  if (!strcmp(string,"kr_wayland_pointer_event")) {
    return CGEN_KR_WAYLAND_POINTER_EVENT;
  }
  if (!strcmp(string,"kr_wayland_key_event")) {
    return CGEN_KR_WAYLAND_KEY_EVENT;
  }
  if (!strcmp(string,"kr_wayland_frame_event")) {
    return CGEN_KR_WAYLAND_FRAME_EVENT;
  }
  if (!strcmp(string,"kr_wayland_event")) {
    return CGEN_KR_WAYLAND_EVENT;
  }
  if (!strcmp(string,"kr_wayland_path_setup")) {
    return CGEN_KR_WAYLAND_PATH_SETUP;
  }
  if (!strcmp(string,"kr_wayland_setup")) {
    return CGEN_KR_WAYLAND_SETUP;
  }
  if (!strcmp(string,"kr_wayland_info")) {
    return CGEN_KR_WAYLAND_INFO;
  }
  if (!strcmp(string,"kr_wayland_path_info")) {
    return CGEN_KR_WAYLAND_PATH_INFO;
  }
  if (!strcmp(string,"krad_interweb_shutdown")) {
    return CGEN_KRAD_INTERWEB_SHUTDOWN;
  }
  if (!strcmp(string,"kr_web_event_type")) {
    return CGEN_KR_WEB_EVENT_TYPE;
  }
  if (!strcmp(string,"kr_web_event")) {
    return CGEN_KR_WEB_EVENT;
  }
  if (!strcmp(string,"kr_web_server_setup")) {
    return CGEN_KR_WEB_SERVER_SETUP;
  }
  if (!strcmp(string,"kr_webrtc_user")) {
    return CGEN_KR_WEBRTC_USER;
  }
  if (!strcmp(string,"kr_webrtc_signal_type")) {
    return CGEN_KR_WEBRTC_SIGNAL_TYPE;
  }
  if (!strcmp(string,"kr_webrtc_signal")) {
    return CGEN_KR_WEBRTC_SIGNAL;
  }
  if (!strcmp(string,"kr_web_server")) {
    return CGEN_KR_WEB_SERVER;
  }
  if (!strcmp(string,"kr_web_client_type")) {
    return CGEN_KR_WEB_CLIENT_TYPE;
  }
  if (!strcmp(string,"kr_web_verb")) {
    return CGEN_KR_WEB_VERB;
  }
  if (!strcmp(string,"kr_websocket_client")) {
    return CGEN_KR_WEBSOCKET_CLIENT;
  }
  if (!strcmp(string,"kr_web_client")) {
    return CGEN_KR_WEB_CLIENT;
  }
  if (!strcmp(string,"kr_x11")) {
    return CGEN_KR_X11;
  }
  if (!strcmp(string,"kr_x11_path")) {
    return CGEN_KR_X11_PATH;
  }
  if (!strcmp(string,"kr_x11_info")) {
    return CGEN_KR_X11_INFO;
  }
  if (!strcmp(string,"kr_x11_path_info")) {
    return CGEN_KR_X11_PATH_INFO;
  }
  if (!strcmp(string,"krad_xmms_playback_cmd_t")) {
    return CGEN_KRAD_XMMS_PLAYBACK_CMD_T;
  }
  if (!strcmp(string,"krad_xmms_St")) {
    return CGEN_KRAD_XMMS_ST;
  }
  if (!strcmp(string,"krad_y4m_St")) {
    return CGEN_KRAD_Y4M_ST;
  }
  return 0;
}

int codegen_is_union(char *type) {
  if (!strcmp(type,"kr_muxer_transport_params")) {
    return 1;
  }
  if (!strcmp(type,"kr_medium_params_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_unit_control_value_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_subunit_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_unit_control_name_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_unit_id_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_rep_ptr_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_rep_actual_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_radio_payload")) {
    return 1;
  }
  if (!strcmp(type,"kr_adapter_api_info")) {
    return 1;
  }
  if (!strcmp(type,"kr_adapter_api_path_info")) {
    return 1;
  }
  if (!strcmp(type,"kr_transponder_path_io_path_info")) {
    return 1;
  }
  return 0;
}

int codegen_is_enum(char *type) {
  if (!strcmp(type,"kr_app_method")) {
    return 1;
  }
  if (!strcmp(type,"kr_app_client_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_easing")) {
    return 1;
  }
  if (!strcmp(type,"kr_mtype_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_compositor_event_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_compositor_path_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_compositor_subunit_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_vector_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_compositor_control")) {
    return 1;
  }
  if (!strcmp(type,"compositor_path_state")) {
    return 1;
  }
  if (!strcmp(type,"krad_codec_t")) {
    return 1;
  }
  if (!strcmp(type,"krad_container_type_t")) {
    return 1;
  }
  if (!strcmp(type,"krad_io_mode_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_jack_event")) {
    return 1;
  }
  if (!strcmp(type,"kr_jack_direction")) {
    return 1;
  }
  if (!strcmp(type,"kr_jack_state")) {
    return 1;
  }
  if (!strcmp(type,"krad_list_format_t")) {
    return 1;
  }
  if (!strcmp(type,"krad_list_parse_state_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_mixer_event_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_mixer_channels")) {
    return 1;
  }
  if (!strcmp(type,"kr_mixer_control")) {
    return 1;
  }
  if (!strcmp(type,"kr_mixer_path_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_mixer_adv_ctl")) {
    return 1;
  }
  if (!strcmp(type,"kr_muxer_transport_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_decoder_state_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_decoder_cmd_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_ddir_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_demuxer_state_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_demuxer_cmd_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_direction")) {
    return 1;
  }
  if (!strcmp(type,"kr_player_playback_state")) {
    return 1;
  }
  if (!strcmp(type,"kr_player_cmd")) {
    return 1;
  }
  if (!strcmp(type,"kr_unit_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_mixer_subunit_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_transponder_subunit_type_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_station_subunit_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_unit_control_data_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_transponder_control_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_radio_payload_type")) {
    return 1;
  }
  if (!strcmp(type,"krad_rc_type_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_sfx_control")) {
    return 1;
  }
  if (!strcmp(type,"kr_sfx_effect_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_sfx_effect_control")) {
    return 1;
  }
  if (!strcmp(type,"kr_adapter_path_direction")) {
    return 1;
  }
  if (!strcmp(type,"kr_adapter_api")) {
    return 1;
  }
  if (!strcmp(type,"kr_transponder_event_type")) {
    return 1;
  }
  if (!strcmp(type,"krad_link_av_mode_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_txpdr_su_type_t")) {
    return 1;
  }
  if (!strcmp(type,"krad_link_transport_mode_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_transponder_path_io_type")) {
    return 1;
  }
  if (!strcmp(type,"krad_slice_track_type_t")) {
    return 1;
  }
  if (!strcmp(type,"kr_v4l2_state")) {
    return 1;
  }
  if (!strcmp(type,"kr_wayland_event_type")) {
    return 1;
  }
  if (!strcmp(type,"krad_interweb_shutdown")) {
    return 1;
  }
  if (!strcmp(type,"kr_web_event_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_webrtc_signal_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_web_client_type")) {
    return 1;
  }
  if (!strcmp(type,"kr_web_verb")) {
    return 1;
  }
  if (!strcmp(type,"krad_xmms_playback_cmd_t")) {
    return 1;
  }
  return 0;
}

