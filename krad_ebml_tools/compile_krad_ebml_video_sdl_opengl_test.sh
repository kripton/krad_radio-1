gcc -g -Wall krad_ebml_video_sdl_opengl_test.c krad_sdl_opengl_display.c krad_vpx.c krad_ebml_player.c krad_opus.c krad_audio.c krad_jack.c krad_pulse.c krad_alsa.c /home/oneman/kode/kradradio/util/kradtone.c ../../kradebml/krad_ebml.c ../../opus-tools/src/opus_header.c ../../opus-sup/.libs/libopus.a /usr/lib/libspeexdsp.a -I../../opus/include -I../../opus-tools/src ../../kradebml/halloc/src/halloc.c -I../../kradebml/ -I/usr/local/include/SDL -I../../kradebml/halloc -I. -I /home/oneman/kode/kradradio/include/util/ ../../libvpx/libvpx_g.a /usr/lib/libswscale.a /usr/lib/libavutil.a -o krad_ebml_video_sdl_opengl `sdl-config --cflags --libs`  -lXext -lX11 -lXmu -lXi -lGL -lGLU -lm -ljack -lpulse -lasound -lSDL
