#include "krad_av.h"

int kr_image_ref(kr_image *image) {
  if (image == NULL) return -1;
  /* FIXME */
  return 0;
}

int kr_image_unref(kr_image *image) {
  if (image == NULL) return -1;
  /* FIXME */
  if ((image->owner != NULL) && (image->release != NULL)) {
   image->release(image->owner);
  }
  return 0;
}
