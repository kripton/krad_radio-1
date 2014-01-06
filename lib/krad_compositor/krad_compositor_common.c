#include "krad_compositor_common.h"

void kr_aspect_scale(int sw, int sh, int dw, int dh, int *w, int *h) {
  double scale_x, scale_y, scale;
  scale_x = (float)dw  / sw;
  scale_y = (float)dh / sh;
  scale = MIN(scale_x, scale_y);
  *w = sw * scale;
  *h = sh * scale;
}
