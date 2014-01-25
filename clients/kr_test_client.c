#include "kr_client.h"

static int test_jack_input_create(kr_client *client);
static int test_jack_output_create(kr_client *client);
static int test_decklink_input_create(kr_client *client, int dev);
static int test_v4l2_input_create(kr_client *client, int dev);
static int test_wayland_output_create(kr_client *client);

static int test_jack_input_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  int channels;
  /* init function? */
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  channels = 2;
  name = "Music";
  info.input.type = KR_XPDR_ADAPTER;
  info.input.info.adapter_path_info.api = KR_ADP_JACK;
  strcpy(info.input.info.adapter_path_info.info.jack.name, name);
  info.input.info.adapter_path_info.info.jack.channels = channels;
  info.input.info.adapter_path_info.info.jack.direction = KR_JACK_INPUT;
  info.output.type = KR_XPDR_MIXER;
  info.output.info.mixer_path_info.channels = channels;
  info.output.info.mixer_path_info.type = KR_MXR_SOURCE;
  ret = kr_xpdr_mkpath(client, name, &info);
  return ret;
}

static int test_jack_output_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  int channels;
  /* init func? */
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  channels = 2;
  name = "Main";
  info.input.type = KR_XPDR_MIXER;
  info.input.info.mixer_path_info.channels = channels;
  info.input.info.mixer_path_info.type = KR_MXR_OUTPUT;
  info.output.type = KR_XPDR_ADAPTER;
  info.output.info.adapter_path_info.api = KR_ADP_JACK;
  strcpy(info.output.info.adapter_path_info.info.jack.name, name);
  info.output.info.adapter_path_info.info.jack.channels = channels;
  info.output.info.adapter_path_info.info.jack.direction = KR_JACK_OUTPUT;
  ret = kr_xpdr_mkpath(client, name, &info);
  return ret;
}

static int test_v4l2_input_create(kr_client *client, int dev) {
  int ret;
  kr_xpdr_path_info info;
  int device_num;
  char *name;
  int width;
  int height;
  int num;
  int den;
  name = "V4L2Test";
  device_num = 0;
  device_num = dev;
  width = 640;
  height = 360;
  num = 30;
  den = 1;
  /* init func? */
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  info.input.type = KR_XPDR_ADAPTER;
  info.input.info.adapter_path_info.api = KR_ADP_V4L2;
  info.input.info.adapter_path_info.info.v4l2.dev = device_num;
  info.input.info.adapter_path_info.info.v4l2.mode.num = num;
  info.input.info.adapter_path_info.info.v4l2.mode.den = den;
  info.input.info.adapter_path_info.info.v4l2.mode.width = width;
  info.input.info.adapter_path_info.info.v4l2.mode.height = height;
  info.output.type = KR_XPDR_COMPOSITOR;
  info.output.info.compositor_path_info.width = width;
  info.output.info.compositor_path_info.height = height;
  info.output.info.compositor_path_info.type = KR_CMP_INPUT;
  ret = kr_xpdr_mkpath(client, name, &info);
  return ret;
}

static int test_x11_input_create(kr_client *client, int dev) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  int width;
  int height;
  int num;
  int den;
  name = "X11Test";
  width = 640;
  height = 360;
  num = 30;
  den = 1;
  /* init func? */
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  info.input.type = KR_XPDR_ADAPTER;
  info.input.info.adapter_path_info.api = KR_ADP_X11;
  strcat(info.input.info.adapter_path_info.info.x11.display, "");
  info.input.info.adapter_path_info.info.x11.num = num;
  info.input.info.adapter_path_info.info.x11.den = den;
  info.input.info.adapter_path_info.info.x11.width = width;
  info.input.info.adapter_path_info.info.x11.height = height;
  info.output.type = KR_XPDR_COMPOSITOR;
  info.output.info.compositor_path_info.width = width;
  info.output.info.compositor_path_info.height = height;
  info.output.info.compositor_path_info.type = KR_CMP_INPUT;
  ret = kr_xpdr_mkpath(client, name, &info);
  return ret;
}

static int test_decklink_input_create(kr_client *client, int dev) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  char *video_connector;
  char *audio_connector;
  int width;
  int height;
  int num;
  int den;
  name = "DecklinkTest";
  video_connector = "hdmi";
  audio_connector = "hdmi";
  width = 1280;
  height = 720;
  num = 60000;
  den = 1001;
  /* init func? */
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  info.input.type = KR_XPDR_ADAPTER;
  info.input.info.adapter_path_info.api = KR_ADP_DECKLINK;
  snprintf(info.input.info.adapter_path_info.info.decklink.device,
   sizeof(info.input.info.adapter_path_info.info.decklink.device), "%d", dev);
  strcpy(info.input.info.adapter_path_info.info.decklink.video_connector,
   video_connector);
  strcpy(info.input.info.adapter_path_info.info.decklink.audio_connector,
   audio_connector);
  info.input.info.adapter_path_info.info.decklink.num = num;
  info.input.info.adapter_path_info.info.decklink.den = den;
  info.input.info.adapter_path_info.info.decklink.width = width;
  info.input.info.adapter_path_info.info.decklink.height = height;
  info.output.type = KR_XPDR_COMPOSITOR;
  info.output.info.compositor_path_info.width = width;
  info.output.info.compositor_path_info.height = height;
  info.output.info.compositor_path_info.type = KR_CMP_INPUT;
  ret = kr_xpdr_mkpath(client, name, &info);
  return ret;
}

static int test_wayland_output_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  char *display_name;
  int width;
  int height;
  int fullscreen;
  display_name = "";
  name = "WaylandTest";
  width = 640;
  height = 360;
  fullscreen = 0;
  /* init func ? */
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  info.input.type = KR_XPDR_COMPOSITOR;
  info.input.info.compositor_path_info.width = width;
  info.input.info.compositor_path_info.height = height;
  info.input.info.compositor_path_info.type = KR_CMP_OUTPUT;
  info.output.type = KR_XPDR_ADAPTER;
  info.output.info.adapter_path_info.api = KR_ADP_WAYLAND;
  strcpy(info.output.info.adapter_path_info.info.wayland.display_name,
   display_name);
  info.output.info.adapter_path_info.info.wayland.width = width;
  info.output.info.adapter_path_info.info.wayland.height = height;
  info.output.info.adapter_path_info.info.wayland.fullscreen = fullscreen;
  ret = kr_xpdr_mkpath(client, name, &info);
  return ret;
}

int test_x11_get(kr_client *client) {
  int ret;
  ret = kr_get(client, "/compositor/X11Test");
  kr_poll(client, 250);
  kr_delivery_recv(client);
  kr_streamer45(client);
  return ret;
}

int make_masterbus(kr_client *client) {
  int ret;
  kr_crate2 crate;
  memset(&crate, 0, sizeof(crate));
  kr_mixer_path_info *masterbus;
  strcpy(crate.address, "/mixer/Master");
  crate.method = KR_PUT;
  crate.payload_type = PL_KR_MIXER_PATH_INFO;
  masterbus = &crate.payload.mixer_path_info;
  masterbus->channels = 2;
  masterbus->volume[0] = 50.0;
  masterbus->volume[1] = 50.0;
  masterbus->type = KR_MXR_BUS;
  ret = kr_crate_send(client, &crate);
  return ret;
}

int make_musicbus(kr_client *client) {
  int ret;
  kr_crate2 crate;
  memset(&crate, 0, sizeof(crate));
  kr_mixer_path_info *bus;
  strcpy(crate.address, "/mixer/Music");
  crate.method = KR_PUT;
  crate.payload_type = PL_KR_MIXER_PATH_INFO;
  bus = &crate.payload.mixer_path_info;
  bus->channels = 2;
  bus->volume[0] = 55.0;
  bus->volume[1] = 55.0;
  bus->type = KR_MXR_BUS;
  ret = kr_crate_send(client, &crate);
  return ret;
}

int make_musicmaster(kr_client *client) {
  int ret;
  kr_crate2 crate;
  memset(&crate, 0, sizeof(crate));
  kr_mixer_path_info *in;
  strcpy(crate.address, "/mixer/Master/Music");
  crate.method = KR_PUT;
  crate.payload_type = PL_KR_MIXER_PATH_INFO;
  in = &crate.payload.mixer_path_info;
  in->channels = 2;
  in->volume[0] = 65.0;
  in->volume[1] = 65.0;
  in->type = KR_MXR_INPUT;
  ret = kr_crate_send(client, &crate);
  return ret;
}

int run_test(kr_client *client, char *test) {
  int ret;
  ret = -1;
  if ((strlen(test) == strlen("wayland")) && (strcmp(test, "wayland") == 0)) {
    ret = test_wayland_output_create(client);
  }
  if ((strlen(test) == strlen("x11")) && (strcmp(test, "x11") == 0)) {
    ret = test_x11_input_create(client, 0);
    if (ret != 0) return ret;
  }
  if ((strlen(test) == strlen("masterbus")) && (strcmp(test, "masterbus") == 0)) {
    ret = make_masterbus(client);
    if (ret != 0) return ret;
  }
  if ((strlen(test) == strlen("musicbus")) && (strcmp(test, "musicbus") == 0)) {
    ret = make_musicbus(client);
    if (ret != 0) return ret;
  }
  if ((strlen(test) == strlen("musicmaster")) && (strcmp(test, "musicmaster") == 0)) {
    ret = make_musicmaster(client);
    if (ret != 0) return ret;
  }
  if ((strlen(test) == strlen("getx11")) && (strcmp(test, "getx11") == 0)) {
    ret = test_x11_get(client);
    if (ret != 0) return ret;
  }
  if ((strlen(test) == strlen("jackin")) && (strcmp(test, "jackin") == 0)) {
    ret = test_jack_input_create(client);
    if (ret != 0) return ret;
  }
  if ((strlen(test) == strlen("jackout")) && (strcmp(test, "jackout") == 0)) {
    ret = test_jack_output_create(client);
    if (ret != 0) return ret;
  }
  if ((strlen(test) == strlen("v4l2")) && (strcmp(test, "v4l2") == 0)) {
      ret = test_v4l2_input_create(client, 0);
    if (ret != 0) return ret;
  }
  if ((strlen(test) == strlen("decklink")) && (strcmp(test, "decklink") == 0)) {
      ret = test_decklink_input_create(client, 0);
    if (ret != 0) return ret;
  }
  if ((strlen(test) == strlen("alsa")) && (strcmp(test, "alsa") == 0)) {
    if (ret != 0) return ret;
  }
  return ret;
}

int main(int argc, char *argv[]) {
  kr_client *client;
  char *sysname;
  const char *tests;
  int ret;
  int i;
  tests = "jack decklink wayland x11 v4l2 alsa";
  client = NULL;
  sysname = NULL;
  ret = 0;
  if (argc < 3) {
    fprintf(stderr, "Usage: %s [STATION] [TESTS]\nTests: %s\n",
     "kr_test_client", tests);
    return 1;
  }
  if (krad_valid_host_and_port(argv[1])) {
    sysname = argv[1];
  } else {
    if (!kr_sysname_valid(argv[1])) {
      fprintf(stderr, "Invalid station sysname!\n");
      return 1;
    } else {
      sysname = argv[1];
    }
  }
  client = kr_client_create("krad simple client");
  if (client == NULL) {
    fprintf(stderr, "Could create client\n");
    return 1;
  }
  if (!kr_connect(client, sysname)) {
    fprintf(stderr, "Could not connect to %s krad radio daemon\n", sysname);
    kr_client_destroy(&client);
    return 1;
  }
  printf("Connected to %s!\n", sysname);
  printf("Running Tests\n");
  for (i = 0; i < (argc - 2); i++) {
    ret = run_test(client, argv[2 + i]);
    if (ret != 0) break;
  }
  printf("Disconnecting from %s..\n", sysname);
  kr_disconnect(client);
  printf("Disconnected from %s.\n", sysname);
  printf("Destroying client..\n");
  kr_client_destroy(&client);
  printf("Client Destroyed.\n");
  return ret;
}
