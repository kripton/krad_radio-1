include $(top_srcdir)/common.mk

#bin_PROGRAMS = kr kr_snapshot
bin_PROGRAMS = kr

if KR_IZ_US
noinst_PROGRAMS = kr_client_example kr_test_client
endif

kr_SOURCES = kr.c
kr_CFLAGS = $(OPUS_CFLAGS)
kr_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_LDFLAGS = ../lib/krad_radio/libkradclient.la

#kr_snapshot_SOURCES = kr_snapshot.c
#kr_snapshot_CFLAGS = $(OPUS_CFLAGS) $(CAIRO_CFLAGS)
#kr_snapshot_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
#kr_snapshot_LDFLAGS = ../lib/krad_radio/libkradclient.la $(CAIRO_LIBS)

kr_client_example_SOURCES = kr_client_example.c
kr_client_example_CFLAGS = $(OPUS_CFLAGS)
kr_client_example_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_client_example_LDFLAGS = ../lib/krad_radio/libkradclient.la

kr_test_client_SOURCES = kr_test_client.c
kr_test_client_CFLAGS = $(OPUS_CFLAGS)
kr_test_client_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_test_client_LDFLAGS = ../lib/krad_radio/libkradclient.la
