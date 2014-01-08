#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_ebmlx.h"
#include "krad_compositor_common.h"
int kr_compositor_path_type_to_ebml(kr_ebml *ebml, void *st);
int kr_compositor_subunit_type_to_ebml(kr_ebml *ebml, void *st);
int kr_vector_type_to_ebml(kr_ebml *ebml, void *st);
int kr_compositor_control_to_ebml(kr_ebml *ebml, void *st);
int kr_compositor_controls_to_ebml(kr_ebml *ebml, void *st);
int kr_sprite_info_to_ebml(kr_ebml *ebml, void *st);
int kr_text_info_to_ebml(kr_ebml *ebml, void *st);
int kr_vector_info_to_ebml(kr_ebml *ebml, void *st);
int kr_compositor_path_info_to_ebml(kr_ebml *ebml, void *st);
int kr_compositor_info_to_ebml(kr_ebml *ebml, void *st);
int kr_compositor_path_patch_to_ebml(kr_ebml *ebml, void *st);
