include $(top_srcdir)/common.mk

bin_PROGRAMS = kr_cbor_test kr_pool_test kr_v4l2_test kr_alsa_test
#bin_PROGRAMS += kr_helpers_test kr_ebml_serial_test kr_json_serial_test
bin_PROGRAMS += kr_graph_test
bin_PROGRAMS += kr_thread_test kr_crypto_test kr_jack_test

if HAVE_WAYLAND
  bin_PROGRAMS += kr_wayland_test
endif

kr_crypto_test_SOURCES = kr_crypto_test.c
kr_crypto_test_CFLAGS =
kr_crypto_test_LDFLAGS = -lssl

kr_thread_test_SOURCES = kr_thread_test.c
kr_thread_test_CFLAGS =
kr_thread_test_LDFLAGS =

kr_cbor_test_SOURCES = kr_cbor_test.c
kr_cbor_test_SOURCES += ../lib/krad_cbor/krad_cbor.c ../lib/krad_mem/krad_mem.c
kr_cbor_test_CFLAGS =
kr_cbor_test_LDFLAGS =

kr_pool_test_SOURCES = kr_pool_test.c
kr_pool_test_SOURCES += ../lib/krad_mem/krad_pool.c ../lib/krad_system/krad_system.c
kr_pool_test_CFLAGS =
kr_pool_test_LDFLAGS =

kr_v4l2_test_SOURCES = kr_v4l2_test.c
kr_v4l2_test_SOURCES += ../lib/krad_v4l2/krad_v4l2.c ../lib/krad_system/krad_system.c ../lib/krad_mem/krad_mem.c
kr_v4l2_test_CFLAGS = -pthread
kr_v4l2_test_LDFLAGS = -lm

kr_alsa_test_SOURCES = kr_alsa_test.c
kr_alsa_test_SOURCES += ../lib/krad_alsa/krad_alsa.c ../lib/krad_system/krad_system.c ../lib/krad_mem/krad_mem.c
kr_alsa_test_CFLAGS = $(ALSA_LIBS)
kr_alsa_test_LDFLAGS = -lm

kr_wayland_test_SOURCES = kr_wayland_test.c
kr_wayland_test_SOURCES += ../lib/krad_wayland/krad_wayland.c ../lib/krad_mem/krad_mem.c
kr_wayland_test_CFLAGS =
kr_wayland_test_LDFLAGS = $(WAYLAND_LIBS) $(XKBCOMMON_LIBS)

kr_helpers_test_SOURCES = kr_helpers_test.c
kr_helpers_test_CFLAGS =
kr_helpers_test_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_helpers_test_LDFLAGS = ../lib/krad_radio/libkradclient.la

kr_ebml_serial_test_SOURCES = kr_ebml_serial_test.c
kr_ebml_serial_test_CFLAGS =
kr_ebml_serial_test_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_ebml_serial_test_LDFLAGS = ../lib/krad_radio/libkradclient.la

kr_json_serial_test_SOURCES = kr_json_serial_test.c
kr_json_serial_test_CFLAGS =
kr_json_serial_test_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_json_serial_test_LDFLAGS = ../lib/krad_radio/libkradclient.la

kr_jack_test_SOURCES = kr_jack_test.c
kr_jack_test_CFLAGS =
kr_jack_test_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_jack_test_LDFLAGS = ../lib/krad_radio/libkradclient.la ../lib/krad_radio/libkradradio.la

kr_graph_test_SOURCES = ../lib/krad_system/krad_system.c \
  ../lib/krad_mem/krad_mem.c \
  ../lib/krad_chronometer/krad_timer.c \
  ../lib/krad_graph/krad_graph.c kr_graph_test.c

.PHONY: all
