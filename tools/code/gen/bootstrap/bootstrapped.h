typedef enum {
  CGEN_KR_AUX_INFO_MEMBER,
  CGEN_KR_AUX_INFO_PATCH_VALUE,
  CGEN_KR_AUX_INFO_PATCH,
  CGEN_KR_AUX_PATH_INFO_MEMBER,
  CGEN_KR_AUX_PATH_INFO_PATCH_VALUE,
  CGEN_KR_AUX_PATH_INFO_PATCH,
  CGEN_KR_AUX_INFO,
  CGEN_KR_AUX_PATH_INFO,
  CGEN_KR_ALSA_INFO_MEMBER,
  CGEN_KR_ALSA_INFO_PATCH_VALUE,
  CGEN_KR_ALSA_INFO_PATCH,
  CGEN_KR_ALSA_PATH_INFO_MEMBER,
  CGEN_KR_ALSA_PATH_INFO_PATCH_VALUE,
  CGEN_KR_ALSA_PATH_INFO_PATCH,
  CGEN_KR_ALSA_INFO,
  CGEN_KR_ALSA_PATH_INFO,
  CGEN_KRAD_ALSA_SEQ_ST,
  CGEN_KR_APP_SERVER_SETUP,
  CGEN_KR_APP_SERVER_INFO,
  CGEN_KR_APP_SERVER_CLIENT_SETUP,
  CGEN_KR_METHOD,
  CGEN_KR_CRATE,
  CGEN_KR_ROUTER_INFO,
  CGEN_KR_ROUTER_MAP_INFO,
  CGEN_KR_ROUTER_MAP_SETUP,
  CGEN_KR_ROUTE_SETUP,
  CGEN_KR_ROUTER_SETUP,
  CGEN_KR_VALUE_TYPE,
  CGEN_KR_VALUE,
  CGEN_KR_VARIANT,
  CGEN_KR_EASER_UPDATE,
  CGEN_KR_EASER,
  CGEN_KR_EASING,
  CGEN_KR_TIMER,
  CGEN_KR_MTYPE_T,
  CGEN_KR_CODEME_ST,
  CGEN_KR_IMAGE_ST,
  CGEN_KR_AUDIO_ST,
  CGEN_KR_MEDIUM_ST,
  CGEN_KR_ENCODER_MACHINE_SECTOR_ST,
  CGEN_KR_COMPOSITOR_OUTPUT_INFO_MEMBER,
  CGEN_KR_COMPOSITOR_OUTPUT_INFO_PATCH_VALUE,
  CGEN_KR_COMPOSITOR_OUTPUT_INFO_PATCH,
  CGEN_KR_COMPOSITOR_BUS_INFO_MEMBER,
  CGEN_KR_COMPOSITOR_BUS_INFO_PATCH_VALUE,
  CGEN_KR_COMPOSITOR_BUS_INFO_PATCH,
  CGEN_KR_COMPOSITOR_INPUT_INFO_MEMBER,
  CGEN_KR_COMPOSITOR_INPUT_INFO_PATCH_VALUE,
  CGEN_KR_COMPOSITOR_INPUT_INFO_PATCH,
  CGEN_KR_COMPOSITOR_SOURCE_INFO_MEMBER,
  CGEN_KR_COMPOSITOR_SOURCE_INFO_PATCH_VALUE,
  CGEN_KR_COMPOSITOR_SOURCE_INFO_PATCH,
  CGEN_KR_OVERLAY_INFO_MEMBER,
  CGEN_KR_OVERLAY_INFO_PATCH_VALUE,
  CGEN_KR_OVERLAY_INFO_PATCH,
  CGEN_KR_COMPOSITOR_PATH_INFO_MEMBER,
  CGEN_KR_COMPOSITOR_PATH_INFO_PATCH_VALUE,
  CGEN_KR_COMPOSITOR_PATH_INFO_PATCH,
  CGEN_KR_SPRITE_INFO_MEMBER,
  CGEN_KR_SPRITE_INFO_PATCH_VALUE,
  CGEN_KR_SPRITE_INFO_PATCH,
  CGEN_KR_TEXT_INFO_MEMBER,
  CGEN_KR_TEXT_INFO_PATCH_VALUE,
  CGEN_KR_TEXT_INFO_PATCH,
  CGEN_KR_VECTOR_INFO_MEMBER,
  CGEN_KR_VECTOR_INFO_PATCH_VALUE,
  CGEN_KR_VECTOR_INFO_PATCH,
  CGEN_KR_COMPOSITOR_SETUP,
  CGEN_KR_COMPOSITOR_EVENT_TYPE,
  CGEN_KR_COMPOSITOR_EVENT,
  CGEN_KR_COMPOSITOR_PATH_TYPE,
  CGEN_KR_COMPOSITOR_OVERLAY_TYPE,
  CGEN_KR_RECT,
  CGEN_KR_COMPOSITOR_OUTPUT_INFO,
  CGEN_KR_COMPOSITOR_BUS_INFO,
  CGEN_KR_COMPOSITOR_INPUT_INFO,
  CGEN_KR_COMPOSITOR_SOURCE_INFO,
  CGEN_KR_OVERLAY_TYPE_INFO,
  CGEN_KR_OVERLAY_INFO,
  CGEN_KR_COMPOSITOR_PATH_TYPE_INFO,
  CGEN_KR_COMPOSITOR_PATH_INFO,
  CGEN_KR_FRAME_EVENT,
  CGEN_KR_COMPOSITOR_PORT_SETUP,
  CGEN_KR_CROP,
  CGEN_KR_CONVERT,
  CGEN_KR_PERSPECTIVE,
  CGEN_KR_POS,
  CGEN_KR_PERSPECTIVE_VIEW,
  CGEN_KR_SPRITE_INFO,
  CGEN_KR_TEXT_INFO,
  CGEN_KR_VECTOR_TYPE,
  CGEN_KR_VECTOR_INFO,
  CGEN_KRAD_CODEC_T,
  CGEN_KRAD_CONTAINER_TYPE_T,
  CGEN_KRAD_CONTAINER_ST,
  CGEN_KR_DECKLINK_INFO_MEMBER,
  CGEN_KR_DECKLINK_INFO_PATCH_VALUE,
  CGEN_KR_DECKLINK_INFO_PATCH,
  CGEN_KR_DECKLINK_PATH_INFO_MEMBER,
  CGEN_KR_DECKLINK_PATH_INFO_PATCH_VALUE,
  CGEN_KR_DECKLINK_PATH_INFO_PATCH,
  CGEN_KR_DECKLINK,
  CGEN_KRAD_DECKLINK_CAPTURE_ST,
  CGEN_KR_DECKLINK_INFO,
  CGEN_KR_DECKLINK_PATH_INFO,
  CGEN_KR_EBML,
  CGEN_KR_FC2_ST,
  CGEN_KRAD_FLAC_ST,
  CGEN_KR_EDGE_DIR,
  CGEN_KR_VERTEX_TYPE,
  CGEN_KR_EDGE,
  CGEN_KR_VERTEX,
  CGEN_KR_GRAPH,
  CGEN_KR_GRAPH_SETUP,
  CGEN_KR_FILE,
  CGEN_KRAD_IO_MODE_T,
  CGEN_KR_IO2_ST,
  CGEN_KR_STREAM,
  CGEN_KR_JACK_SETUP_INFO_MEMBER,
  CGEN_KR_JACK_SETUP_INFO_PATCH_VALUE,
  CGEN_KR_JACK_SETUP_INFO_PATCH,
  CGEN_KR_JACK_INFO_MEMBER,
  CGEN_KR_JACK_INFO_PATCH_VALUE,
  CGEN_KR_JACK_INFO_PATCH,
  CGEN_KR_JACK_PATH_INFO_MEMBER,
  CGEN_KR_JACK_PATH_INFO_PATCH_VALUE,
  CGEN_KR_JACK_PATH_INFO_PATCH,
  CGEN_KR_JACK_EVENT_TYPE,
  CGEN_KR_JACK_PATH_EVENT_CB_ARG,
  CGEN_KR_JACK_PATH_AUDIO_CB_ARG,
  CGEN_KR_JACK_EVENT_CB_ARG,
  CGEN_KR_JACK_SETUP,
  CGEN_KR_JACK_PATH_SETUP,
  CGEN_KR_JACK_DIRECTION,
  CGEN_KR_JACK_STATE,
  CGEN_KR_JACK_SETUP_INFO,
  CGEN_KR_JACK_INFO,
  CGEN_KR_JACK_PATH_INFO,
  CGEN_KRAD_LIST_FORMAT_T,
  CGEN_KRAD_LIST_PARSE_STATE_T,
  CGEN_KRAD_LIST_T,
  CGEN_KRAD_WEBM_DASH_VOD_T,
  CGEN_KR_POOL_SETUP,
  CGEN_KR_MIXER_PATH_INFO_MEMBER,
  CGEN_KR_MIXER_PATH_INFO_PATCH_VALUE,
  CGEN_KR_MIXER_PATH_INFO_PATCH,
  CGEN_KR_MIXER_EVENT_TYPE,
  CGEN_KR_MIXER_EVENT,
  CGEN_KR_AUDIO_EVENT,
  CGEN_KR_MIXER_PORT_SETUP,
  CGEN_KR_MIXER_SETUP,
  CGEN_KR_MIXER_CHANNELS,
  CGEN_KR_MIXER_PATH_TYPE,
  CGEN_KR_MIXER_PATH_INFO,
  CGEN_KR_MKV_TRACK_ST,
  CGEN_KR_MKV_ST,
  CGEN_KR_MUXER_TRANSPORT_TYPE,
  CGEN_KR_MUXER_FILE_OUTPUT_PARAMS_ST,
  CGEN_KR_MUXER_STREAM_OUTPUT_PARAMS_ST,
  CGEN_KR_MUXER_TRANSMISSION_OUTPUT_PARAMS_ST,
  CGEN_KR_MUXER_TRANSPORT_PARAMS,
  CGEN_KR_MUXER_OUTPUT_PARAMS_ST,
  CGEN_KRAD_OGG_TRACK_ST,
  CGEN_KRAD_OGG_ST,
  CGEN_KR_OGG,
  CGEN_KR_OGG_IO,
  CGEN_KRAD_OPUS_ST,
  CGEN_KR_DECODER_STATE_T,
  CGEN_KR_DECODER_CMD_T,
  CGEN_KR_DDIR_T,
  CGEN_KR_DEMUXER_STATE_T,
  CGEN_KR_DEMUXER_CMD_T,
  CGEN_KR_DEMUXER_PARAMS_ST,
  CGEN_KR_MACHINE_PARAMS_ST,
  CGEN_KR_MACHINE_ST,
  CGEN_KR_MSG_ST,
  CGEN_KR_MSGPAIR_ST,
  CGEN_KR_MSGSYS_ST,
  CGEN_KR_DIRECTION,
  CGEN_KR_PLAYER_PLAYBACK_STATE,
  CGEN_KR_PLAYER_CMD,
  CGEN_KR_VIDEO_PARAMS_ST,
  CGEN_KR_AUDIO_PARAMS_ST,
  CGEN_KR_MEDIUM_PARAMS_T,
  CGEN_KR_TRACK_INFO_ST,
  CGEN_KR_NIL,
  CGEN_KR_PAYLOAD_TYPE,
  CGEN_KR_PAYLOAD,
  CGEN_KRAD_RC_TYPE_T,
  CGEN_KRAD_RC_ST,
  CGEN_KRAD_RC_POLOLU_MAESTRO_ST,
  CGEN_KRAD_RC_RX_ST,
  CGEN_KRAD_RC_SDL_JOY_ST,
  CGEN_KRAD_RC_TX_ST,
  CGEN_KRAD_RESAMPLE_RING_ST,
  CGEN_KR_EQ_BAND_INFO_MEMBER,
  CGEN_KR_EQ_BAND_INFO_PATCH_VALUE,
  CGEN_KR_EQ_BAND_INFO_PATCH,
  CGEN_KR_EQ_INFO_MEMBER,
  CGEN_KR_EQ_INFO_PATCH_VALUE,
  CGEN_KR_EQ_INFO_PATCH,
  CGEN_KR_LOWPASS_INFO_MEMBER,
  CGEN_KR_LOWPASS_INFO_PATCH_VALUE,
  CGEN_KR_LOWPASS_INFO_PATCH,
  CGEN_KR_HIGHPASS_INFO_MEMBER,
  CGEN_KR_HIGHPASS_INFO_PATCH_VALUE,
  CGEN_KR_HIGHPASS_INFO_PATCH,
  CGEN_KR_VOLUME_INFO_MEMBER,
  CGEN_KR_VOLUME_INFO_PATCH_VALUE,
  CGEN_KR_VOLUME_INFO_PATCH,
  CGEN_KR_ANALOG_INFO_MEMBER,
  CGEN_KR_ANALOG_INFO_PATCH_VALUE,
  CGEN_KR_ANALOG_INFO_PATCH,
  CGEN_KR_PASS_INFO,
  CGEN_KR_PASS_SETUP,
  CGEN_KR_PASS_CMD,
  CGEN_KR_SFX_CMD,
  CGEN_KR_SFX_INFO_CB_ARG,
  CGEN_KR_SFX_SETUP,
  CGEN_KR_SFX_CONTROL,
  CGEN_KR_SFX_EFFECT_TYPE,
  CGEN_KR_SFX_EFFECT_CONTROL,
  CGEN_KR_EQ_BAND_INFO,
  CGEN_KR_EQ_INFO,
  CGEN_KR_LOWPASS_INFO,
  CGEN_KR_HIGHPASS_INFO,
  CGEN_KR_VOLUME_INFO,
  CGEN_KR_ANALOG_INFO,
  CGEN_TONE_T,
  CGEN_KRAD_TONE_T,
  CGEN_KRAD_CONTROL_ST,
  CGEN_KR_TAG,
  CGEN_KR_TAGS,
  CGEN_KRAD_THEORA_ENCODER_ST,
  CGEN_KRAD_THEORA_DECODER_ST,
  CGEN_KRAD_SLICE_TRACK_TYPE_T,
  CGEN_KRAD_SUBSLICE_ST,
  CGEN_KRAD_SLICER_ST,
  CGEN_KRAD_REBUILDER_TRACK_ST,
  CGEN_KRAD_REBUILDER_ST,
  CGEN_KRAD_UDP_ST,
  CGEN_KR_V4L2_INFO_MEMBER,
  CGEN_KR_V4L2_INFO_PATCH_VALUE,
  CGEN_KR_V4L2_INFO_PATCH,
  CGEN_KR_V4L2_OPEN_INFO_MEMBER,
  CGEN_KR_V4L2_OPEN_INFO_PATCH_VALUE,
  CGEN_KR_V4L2_OPEN_INFO_PATCH,
  CGEN_KR_V4L2_SETUP,
  CGEN_KR_V4L2_STATE,
  CGEN_KR_V4L2_MODE,
  CGEN_KR_V4L2_INFO,
  CGEN_KR_V4L2_OPEN_INFO,
  CGEN_KRAD_VORBIS_ST,
  CGEN_KRAD_VPX_ENCODER_ST,
  CGEN_KRAD_VPX_DECODER_ST,
  CGEN_KR_WAYLAND_INFO_MEMBER,
  CGEN_KR_WAYLAND_INFO_PATCH_VALUE,
  CGEN_KR_WAYLAND_INFO_PATCH,
  CGEN_KR_WAYLAND_PATH_INFO_MEMBER,
  CGEN_KR_WAYLAND_PATH_INFO_PATCH_VALUE,
  CGEN_KR_WAYLAND_PATH_INFO_PATCH,
  CGEN_KR_WAYLAND_EVENT_TYPE,
  CGEN_KR_WAYLAND_POINTER_EVENT,
  CGEN_KR_WAYLAND_KEY_EVENT,
  CGEN_KR_WAYLAND_FRAME_EVENT,
  CGEN_KR_WAYLAND_EVENT,
  CGEN_KR_WAYLAND_PATH_SETUP,
  CGEN_KR_WAYLAND_SETUP,
  CGEN_KR_WAYLAND_INFO,
  CGEN_KR_WAYLAND_PATH_INFO,
  CGEN_KR_WEB_EVENT_TYPE,
  CGEN_KR_WEB_EVENT,
  CGEN_KR_WEB_SERVER_SETUP,
  CGEN_KR_X11_INFO_MEMBER,
  CGEN_KR_X11_INFO_PATCH_VALUE,
  CGEN_KR_X11_INFO_PATCH,
  CGEN_KR_X11_PATH_INFO_MEMBER,
  CGEN_KR_X11_PATH_INFO_PATCH_VALUE,
  CGEN_KR_X11_PATH_INFO_PATCH,
  CGEN_KR_X11,
  CGEN_KR_X11_PATH,
  CGEN_KR_X11_INFO,
  CGEN_KR_X11_PATH_INFO,
  CGEN_KRAD_XMMS_PLAYBACK_CMD_T,
  CGEN_KRAD_XMMS_ST,
  CGEN_KRAD_Y4M_ST,
  CGEN_KR_XPDR_PATH_INFO_MEMBER,
  CGEN_KR_XPDR_PATH_INFO_PATCH_VALUE,
  CGEN_KR_XPDR_PATH_INFO_PATCH,
  CGEN_KR_ADAPTER_EVENT_TYPE,
  CGEN_KR_ADAPTER_PATH_EVENT_TYPE,
  CGEN_KR_ADAPTER_PATH_EVENT_CB_ARG,
  CGEN_KR_ADAPTER_PATH_AV_CB_ARG,
  CGEN_KR_ADAPTER_EVENT,
  CGEN_KR_ADAPTER_PATH_SETUP,
  CGEN_KR_ADAPTER_SETUP,
  CGEN_KR_ADAPTER,
  CGEN_KR_ADAPTER_PATH,
  CGEN_KR_PATCHSET,
  CGEN_KR_ADAPTER_SPEC,
  CGEN_KR_XPDR_EVENT_TYPE,
  CGEN_KR_XPDR_EVENT,
  CGEN_KR_XPDR_SETUP,
  CGEN_KR_XPDR_TYPE,
  CGEN_KR_XPDR_TYPE_INFO,
  CGEN_KR_XPDR_PATH_INFO,
  CGEN_KR_IMAGE,
  CGEN_KR_AUDIO
} cgen_enum;

enum {
  CODEGEN_ENUM_LAST = 308
};
int codegen_is_union(char *type);
int codegen_is_enum(char *type);
cgen_enum codegen_string_to_enum(char *string);
char *codegen_enum_to_string(cgen_enum val);


typedef struct {
  int type;
  void *actual;
} uber_St;

