/**
 * @file krad_compositor_client.h
 * @brief Krad Radio Compositor Controller API
 */

#include "krad_compositor_common.h"

#ifndef KRAD_COMPOSITOR_CLIENT_H
#define KRAD_COMPOSITOR_CLIENT_H


/** @defgroup krad_compositor_client Krad Radio Compositor Control
  @{
  */

typedef struct kr_videoport_St kr_videoport_t;

int kr_compositor_crate_to_info (kr_crate_t *crate);
int kr_compositor_crate_to_string (kr_crate_t *crate, char **string);

// Public

void kr_compositor_subunit_list (kr_client_t *client);
int kr_compositor_subunit_create (kr_client_t *client,
                                  kr_compositor_subunit_type type,
                                  char *option,
                                  char *option2);
void kr_compositor_subunit_destroy (kr_client_t *client, kr_address_t *address);
void kr_compositor_subunit_info (kr_client_t *client, kr_address_t *address);
void kr_compositor_info_request(kr_client_t *client);
void kr_compositor_set_frame_rate (kr_client_t *client, int numerator, int denominator);
int kr_compositor_set_resolution (kr_client_t *client, uint32_t width, uint32_t height);

void kr_compositor_close_display (kr_client_t *client);
void kr_compositor_open_display (kr_client_t *client, int width, int height);

void kr_compositor_snapshot(kr_client_t *client);
void kr_compositor_snapshot_jpeg(kr_client_t *client);

int kr_compositor_get_info_wait(kr_client_t *client,
                            uint32_t *width,
                            uint32_t *height,
                            uint32_t *fps_num,
                            uint32_t *fps_den);

/* Compositor Local Video Ports */
int kr_videoport_error (kr_videoport_t *videoport);
void kr_videoport_set_callback (kr_videoport_t *kr_videoport, int callback (void *, void *), void *pointer);
void kr_videoport_activate (kr_videoport_t *kr_videoport);
void kr_videoport_deactivate (kr_videoport_t *kr_videoport);
kr_videoport_t *kr_videoport_create (kr_client_t *client, int32_t direction);
void kr_videoport_destroy (kr_videoport_t *kr_videoport);

/**@}*/
#endif
