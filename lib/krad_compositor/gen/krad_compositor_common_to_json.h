#include "gen.h"
#include "krad_compositor_common_helpers.h"
#include "krad_compositor_common.h"
int kr_compositor_path_type_to_json(char *json, void *st, int32_t max);
int kr_compositor_overlay_type_to_json(char *json, void *st, int32_t max);
int kr_rect_to_json(char *json, void *st, int32_t max);
int kr_compositor_output_info_to_json(char *json, void *st, int32_t max);
int kr_compositor_bus_info_to_json(char *json, void *st, int32_t max);
int kr_compositor_input_info_to_json(char *json, void *st, int32_t max);
int kr_compositor_source_info_to_json(char *json, void *st, int32_t max);
int kr_overlay_type_info_to_json(char *json, void *st, int32_t max);
int kr_overlay_info_to_json(char *json, void *st, int32_t max);
int kr_compositor_path_type_info_to_json(char *json, void *st, int32_t max);
int kr_compositor_path_info_to_json(char *json, void *st, int32_t max);
