#include "kr_client.h"

void krad_radio_command_help() {
  printf("kr [STATION_NAME] [COMMAND | ADDRESS | OPTIONS]");
  printf("\n");
}

int kr_cmd_handle(kr_client *client, char *argv[], int argc) {
  int ret;
  ret = -1;
  if ((client == NULL) || (argc < 1)) return -1;
  if (argc == 1) {
    ret = kr_get(client, argv[0]);
    kr_poll(client, 250);
    kr_delivery_recv(client);
    kr_streamer45(client);
  }
  if (argc == 2) {
    if ((strlen(argv[0]) == 2) && (strncmp(argv[0], "rm", 2) == 0)) {
      ret = kr_delete(client, argv[1]);
      printf("sending delete %s\n", argv[1]);
      kr_poll(client, 250);
      kr_delivery_recv(client);
      kr_streamer45(client);
    }
  }
  return ret;
}

int main(int argc, char *argv[]) {
  kr_client *client;
  char *sysname;
  int ret;
  sysname = NULL;
  client = NULL;
  ret = 0;
  if ((argc == 1) || (argc == 2)) {
    if (argc == 2) {
      if (((strlen(argv[1]) == 2) && (strncmp(argv[1], "vn", 2) == 0)) ||
          ((strlen(argv[1]) == 3) && (strncmp(argv[1], "-vn", 3) == 0)) ||
          ((strlen(argv[1]) == 4) && (strncmp(argv[1], "--vn", 4) == 0))) {
          printf("%d\n", KR_VERSION);
          return 0;
      }
      if (((strlen(argv[1]) == 2) && (strncmp(argv[1], "gv", 2) == 0)) ||
          ((strlen(argv[1]) == 2) && (strncmp(argv[1], "vg", 2) == 0)) ||
          ((strlen(argv[1]) == 3) && (strncmp(argv[1], "-vg", 3) == 0)) ||
          ((strlen(argv[1]) == 3) && (strncmp(argv[1], "git", 3) == 0)) ||
          ((strlen(argv[1]) == 4) && (strncmp(argv[1], "--vg", 4) == 0)) ||
          ((strlen(argv[1]) == 6) && (strncmp(argv[1], "gitver", 6) == 0)) ||
          ((strlen(argv[1]) == 10) && (strncmp(argv[1], "gitversion", 10) == 0))) {
          printf("%s\n", KR_GIT_VERSION);
          return 0;
      }
      if (((strlen(argv[1]) == 1) && (strncmp(argv[1], "v", 1) == 0)) ||
          ((strlen(argv[1]) == 2) && (strncmp(argv[1], "-v", 2) == 0)) ||
          ((strlen(argv[1]) >= 3) && (strncmp(argv[1], "--v", 3) == 0))) {
          printf(KR_VERSION_STR_FULL "\n");
          return 0;
      }
      if ((strlen(argv[1]) == 2) && (strncmp(argv[1], "ls", 2) == 0)) {
        if (printf("%s", krad_radio_running_stations())) {
          printf("\n");
        }
        return 0;
      }
    }
    krad_radio_command_help();
    return 0;
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
  if ((strncmp(argv[2], "launch", 6) == 0) || (strncmp(argv[2], "load", 4) == 0)) {
    krad_radio_launch(sysname);
    return 0;
  }
  if ((strncmp(argv[2], "destroy", 7) == 0) || (strncmp(argv[2], "kill", 4) == 0)) {
    ret = krad_radio_destroy(sysname);
    if (ret == 1) {
      fprintf(stderr, "Daemon was killed\n");
    }
    if (ret == -1) {
      fprintf(stderr, "Daemon was not running\n");
    }
    return 0;
  }
  client = kr_client_create("krad command line client");
  if(client == NULL) {
    fprintf(stderr, "Could create client\n");
    return 1;
  }
  if (!kr_connect(client, sysname)) {
    fprintf(stderr, "Could not connect to %s krad radio daemon\n", sysname);
    kr_client_destroy(&client);
    return 1;
  }
  kr_cmd_handle(client, argv + 2, argc - 2);
  kr_client_destroy(&client);
  return 0;
}
