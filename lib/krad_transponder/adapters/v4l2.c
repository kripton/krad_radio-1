#ifndef __MACH__
/*
void v4l2_loopout_unit_create (void *arg) {

  krad_link_t *krad_link = (krad_link_t *)arg;

  krad_system_set_thread_name ("kr_v4l2_lo");

  printk ("V4L2 Loop Output thread begins");


}

int v4l2_loopout_unit_process (void *arg) {

  krad_link_t *krad_link = (krad_link_t *)arg;

  return 0;
}

void v4l2_loopout_unit_destroy (void *arg) {

  krad_link_t *krad_link = (krad_link_t *)arg;



  printk ("V4L2 Loop Output thread exited");

}
*/

void v4l2_capture_unit_create (void *arg) {
  //krad_system_set_thread_name ("kr_cap_v4l2");

  krad_link_t *krad_link = (krad_link_t *)arg;

  printk ("Video capture creating..");

  //krad_link->video_codec = MJPEG;

  krad_link->krad_v4l2 = krad_v4l2_create ();

  if (krad_link->video_codec != NOCODEC) {
    if (krad_link->video_codec == MJPEG) {
      krad_v4l2_mjpeg_mode (krad_link->krad_v4l2);
    }
    if (krad_link->video_codec == H264) {
      krad_v4l2_h264_mode (krad_link->krad_v4l2);
    }
  }

  if ((krad_link->fps_numerator == 0) || (krad_link->fps_denominator == 0)) {
    krad_compositor_get_frame_rate (krad_link->krad_radio->compositor,
                                    &krad_link->fps_numerator,
                                    &krad_link->fps_denominator);
  }

  if ((krad_link->capture_width == 0) || (krad_link->capture_height == 0)) {
    krad_compositor_get_resolution (krad_link->krad_radio->compositor,
                                    &krad_link->capture_width,
                                    &krad_link->capture_height);
  }

  krad_v4l2_open (krad_link->krad_v4l2, krad_link->device, krad_link->capture_width,
           krad_link->capture_height, 30);

  if ((krad_link->capture_width != krad_link->krad_v4l2->width) ||
      (krad_link->capture_height != krad_link->krad_v4l2->height)) {

    printke ("Got a different resolution from V4L2 than requested.");
    printk ("Wanted: %dx%d Got: %dx%d",
        krad_link->capture_width, krad_link->capture_height,
        krad_link->krad_v4l2->width, krad_link->krad_v4l2->height
        );

    krad_link->capture_width = krad_link->krad_v4l2->width;
    krad_link->capture_height = krad_link->krad_v4l2->height;
  }

  if (krad_link->video_passthru == 1) {
    krad_link->krad_framepool = krad_framepool_create_for_passthru (350000, DEFAULT_CAPTURE_BUFFER_FRAMES * 3);
  } else {

    krad_link->krad_framepool = krad_framepool_create_for_upscale ( krad_link->capture_width,
                              krad_link->capture_height,
                              DEFAULT_CAPTURE_BUFFER_FRAMES,
                              krad_link->composite_width, krad_link->composite_height);
  }

  if (krad_link->video_passthru == 1) {
    //FIXME
  } else {
    krad_link->krad_compositor_port =
    krad_compositor_port_create (krad_link->krad_radio->compositor, "V4L2In", KR_VIN,
                   krad_link->capture_width, krad_link->capture_height);
  }

  krad_v4l2_start_capturing (krad_link->krad_v4l2);

  printk ("Video capture started..");
}

int v4l2_capture_unit_process (void *arg) {

  krad_link_t *krad_link = (krad_link_t *)arg;

  void *captured_frame;
  krad_frame_t *krad_frame;

  captured_frame = NULL;
  krad_frame = NULL;

  captured_frame = krad_v4l2_read (krad_link->krad_v4l2);

  if (captured_frame != NULL) {
    krad_frame = krad_framepool_getframe (krad_link->krad_framepool);
    if (krad_frame != NULL) {
      if (krad_link->video_passthru == 1) {
        memcpy (krad_frame->pixels, captured_frame, krad_link->krad_v4l2->encoded_size);
        krad_frame->encoded_size = krad_link->krad_v4l2->encoded_size;
        krad_compositor_port_push_frame (krad_link->krad_compositor_port, krad_frame);
      } else {
        if (krad_link->video_codec == MJPEG) {
          krad_v4l2_mjpeg_to_rgb (krad_link->krad_v4l2, (unsigned char *)krad_frame->pixels,
                                 captured_frame, krad_link->krad_v4l2->encoded_size);
          krad_compositor_port_push_rgba_frame (krad_link->krad_compositor_port, krad_frame);
        } else {
          krad_frame->format = PIX_FMT_YUYV422;
          krad_frame->yuv_pixels[0] = captured_frame;
          krad_frame->yuv_pixels[1] = NULL;
          krad_frame->yuv_pixels[2] = NULL;
          krad_frame->yuv_strides[0] = krad_link->capture_width + (krad_link->capture_width/2) * 2;
          krad_frame->yuv_strides[1] = 0;
          krad_frame->yuv_strides[2] = 0;
          krad_frame->yuv_strides[3] = 0;
          krad_compositor_port_push_yuv_frame (krad_link->krad_compositor_port, krad_frame);
        }
      }
      krad_framepool_unref_frame (krad_frame);
    }
    krad_v4l2_frame_done (krad_link->krad_v4l2);
  }

  return 0;
}

void v4l2_capture_unit_destroy (void *arg) {

  krad_link_t *krad_link = (krad_link_t *)arg;

  krad_v4l2_stop_capturing (krad_link->krad_v4l2);
  krad_v4l2_close(krad_link->krad_v4l2);
  krad_v4l2_destroy(krad_link->krad_v4l2);

  krad_compositor_port_destroy (krad_link->krad_radio->compositor, krad_link->krad_compositor_port);

  printk ("v4l2 capture unit destroy");
}

#endif