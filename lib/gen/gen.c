#include "../krad_compositor/gen/krad_compositor_to_json.c"
#include "../krad_compositor/gen/krad_compositor_to_ebml.c"
#include "../krad_compositor/gen/krad_compositor_from_ebml.c"
#include "../krad_compositor/gen/krad_compositor_to_text.c"
#include "../krad_compositor/gen/krad_compositor_helpers.c"
#include "../krad_compositor/gen/krad_perspective_to_json.c"
#include "../krad_compositor/gen/krad_perspective_to_ebml.c"
#include "../krad_compositor/gen/krad_perspective_from_ebml.c"
#include "../krad_compositor/gen/krad_perspective_to_text.c"
#include "../krad_compositor/gen/krad_perspective_helpers.c"
#include "../krad_container/gen/krad_container_to_ebml.c"
#include "../krad_container/gen/krad_container_from_ebml.c"
#include "../krad_container/gen/krad_container_to_text.c"
#include "../krad_container/gen/krad_container_helpers.c"
#include "../krad_decklink/gen/krad_decklink_to_ebml.c"
#include "../krad_decklink/gen/krad_decklink_from_ebml.c"
#include "../krad_decklink/gen/krad_decklink_to_text.c"
#include "../krad_decklink/gen/krad_decklink_helpers.c"
#include "../krad_io/gen/krad_io_to_ebml.c"
#include "../krad_jack/gen/krad_jack_to_ebml.c"
#include "../krad_jack/gen/krad_jack_from_ebml.c"
#include "../krad_jack/gen/krad_jack_to_text.c"
#include "../krad_jack/gen/krad_jack_helpers.c"
#include "../krad_mixer/gen/krad_mixer_to_json.c"
#include "../krad_mixer/gen/krad_mixer_from_json.c"
#include "../krad_mixer/gen/krad_mixer_to_ebml.c"
#include "../krad_mixer/gen/krad_mixer_from_ebml.c"
#include "../krad_mixer/gen/krad_mixer_to_text.c"
#include "../krad_mixer/gen/krad_mixer_helpers.c"
#include "../krad_sfx/gen/krad_sfx_to_json.c"
#include "../krad_sfx/gen/krad_sfx_from_json.c"
#include "../krad_sfx/gen/krad_sfx_to_ebml.c"
#include "../krad_sfx/gen/krad_sfx_from_ebml.c"
#include "../krad_sfx/gen/krad_sfx_to_text.c"
#include "../krad_sfx/gen/krad_sfx_helpers.c"
#include "../krad_transponder/gen/krad_transponder_to_ebml.c"
#include "../krad_transponder/gen/krad_transponder_from_ebml.c"
#include "../krad_transponder/gen/krad_transponder_to_text.c"
#include "../krad_transponder/gen/krad_transponder_helpers.c"
#include "../krad_v4l2/gen/krad_v4l2_to_text.c"
#include "../krad_v4l2/gen/krad_v4l2_to_json.c"
#include "../krad_v4l2/gen/krad_v4l2_from_json.c"
#include "../krad_v4l2/gen/krad_v4l2_to_ebml.c"
#include "../krad_v4l2/gen/krad_v4l2_from_ebml.c"
#include "../krad_v4l2/gen/krad_v4l2_helpers.c"
#include "../krad_wayland/gen/krad_wayland_to_ebml.c"
#include "../krad_wayland/gen/krad_wayland_from_ebml.c"
#include "../krad_wayland/gen/krad_wayland_to_text.c"
#include "../krad_wayland/gen/krad_wayland_helpers.c"
#include "ebml_common.c"
#include "text_common.c"
#include "debml_common.c"
#include "json_common.c"
#include "jsmn/jsmn.c"
#include "dejson_common.c"

