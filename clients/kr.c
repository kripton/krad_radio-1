#include "kr_client.h"

void krad_radio_command_help() {
  printf("kr [STATION_NAME] [COMMAND | ADDRESS | OPTIONS]");
  printf("\n");
}

int main (int argc, char *argv[]) {
  kr_client *client;
  char *sysname;
  int ret;
  kr_unit_control_t uc;
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
  /* Krad Radio Commands */
  if ((strncmp(argv[2], "ls", 2) == 0) && (strlen(argv[2]) == 2)) {
    if (argc == 3) {
      kr_mixer_portgroup_list(client);
      kr_delivery_accept_and_report(client);
      kr_compositor_subunit_list(client);
      kr_delivery_accept_and_report(client);
      //kr_transponder_subunit_list(client);
      //kr_delivery_accept_and_report(client);
    }
  }
  if ((((strncmp(argv[2], "lsv", 3) == 0) ||
        (strncmp(argv[2], "lsd", 3) == 0) ||
        (strncmp(argv[2], "lsa", 3) == 0)) &&
        (strlen(argv[2]) == 3)) || (strncmp(argv[2], "adapters", 8) == 0)) {
    if (argc == 3) {
      //kr_transponder_adapters(client);
      //kr_delivery_accept_and_report(client);
    }
  }
  if ((strlen(argv[2]) == 4) && (strncmp(argv[2], "info", 4) == 0)) {
    kr_system_info(client);
    kr_delivery_accept_and_report(client);
    printf("\n");
    kr_mixer_info_get(client);
    kr_delivery_accept_and_report(client);
    printf("\n");
    kr_compositor_info_request(client);
    kr_delivery_accept_and_report(client);
    printf("\n");
    //kr_transponder_info_get(client);
    //kr_delivery_accept_and_report(client);
  }
  if ((strlen(argv[2]) == 7) && (strncmp(argv[2], "sysinfo", 7) == 0)) {
    kr_system_info(client);
    kr_delivery_accept_and_report(client);
  }
  if (strncmp(argv[2], "tags", 4) == 0) {
    if (argc == 3) {
      kr_get_tags(client, NULL);
      kr_delivery_accept_and_report(client);
    }
    if (argc == 4) {
      kr_get_tags(client, argv[3]);
      kr_delivery_accept_and_report(client);
    }
  } else {
    if (strncmp(argv[2], "tag", 3) == 0) {
      if (argc == 4) {
        kr_get_tag(client, NULL, argv[3]);
        kr_delivery_accept_and_report(client);
      }
      if (argc == 5) {
        kr_get_tag(client, argv[3], argv[4]);
        kr_delivery_accept_and_report(client);
      }
    }
  }
  if (strncmp(argv[2], "stag", 4) == 0) {
    if (argc == 5) {
      kr_set_tag(client, NULL, argv[3], argv[4]);
    }
    if (argc == 6) {
      kr_set_tag(client, argv[3], argv[4], argv[5]);
    }
  }
  if (((strlen(argv[2]) == 6) && (strncmp(argv[2], "remote", 6) == 0)) ||
      ((strlen(argv[2]) == 7) && (strncmp(argv[2], "remotes", 7) == 0))) {
    kr_remote_list(client);
    kr_delivery_accept_and_report(client);
  }
  if ((strncmp(argv[2], "remoteon", 8) == 0) || (strncmp(argv[2], "remote_on", 9) == 0)) {
    ret = 0;
    if (argc == 4) {
      ret = kr_remote_on(client, NULL, atoi(argv[3]));
    }
    if (argc == 5) {
      ret = kr_remote_on(client, argv[3], atoi(argv[4]));
    }
    if (ret == -1) {
      printf ("Invalid Port Specified\n");
    }
  }
  if ((strncmp(argv[2], "remoteoff", 9) == 0) || (strncmp(argv[2], "remote_off", 10) == 0)) {
    ret = 0;
    if (argc == 3) {
      ret = kr_remote_off(client, NULL, 0);
    }
    if (argc == 4) {
      ret = kr_remote_off(client, NULL, atoi(argv[3]));
    }
    if (argc == 5) {
      ret = kr_remote_off(client, argv[3], atoi(argv[4]));
    }
    if (ret == -1) {
      printf ("Invalid Port Specified\n");
    }
  }
  if ((strncmp(argv[2], "webon", 5) == 0) ||
      (strncmp(argv[2], "interweb", 5) == 0)) {
    if (argc == 4) {
      kr_web_enable(client, atoi(argv[3]), "", "", "");
    }
    if (argc == 5) {
      kr_web_enable(client, atoi(argv[3]), argv[4], "", "");
    }
    if (argc == 6) {
      kr_web_enable(client, atoi(argv[3]), argv[4], argv[5], "");
    }
    if (argc == 7) {
      kr_web_enable(client, atoi(argv[3]), argv[4], argv[5], argv[6]);
    }
  }
  if (strncmp(argv[2], "weboff", 6) == 0) {
    if (argc == 3) {
      kr_web_disable(client);
    }
  }
  if (strncmp(argv[2], "oscon", 5) == 0) {
    if (argc == 4) {
      kr_osc_enable(client, atoi(argv[3]));
    }
  }
  if (strncmp(argv[2], "oscoff", 6) == 0) {
    if (argc == 3) {
      kr_osc_disable(client);
    }
  }
  /* Krad Mixer Commands */
  if (strncmp(argv[2], "lm", 2) == 0) {
    if (argc == 3) {
      kr_mixer_portgroup_list(client);
      kr_delivery_accept_and_report(client);
    }
  }
  if (((strncmp(argv[2], "m", 3) == 0) && (strlen(argv[2]) == 1)) ||
      ((strncmp(argv[2], "mix", 3) == 0) && (strlen(argv[2]) == 3)) ||
      ((strncmp(argv[2], "mixer", 5) == 0) && (strlen(argv[2]) == 5))) {
    if (argc == 3) {
      kr_mixer_info_get(client);
      kr_delivery_accept_and_report(client);
    }
  }
  if (strncmp(argv[2], "setrate", 7) == 0) {
    if (argc == 4) {
      kr_mixer_set_sample_rate(client, atoi(argv[3]));
    }
  }
  if (strncmp(argv[2], "tone", 4) == 0) {
    if (argc == 4) {
      kr_mixer_push_tone(client, argv[3][0]);
    }
  }
  if (strncmp(argv[2], "portinfo", 8) == 0) {
    if (argc == 4) {
      kr_mixer_portgroup_info(client, argv[3]);
      kr_delivery_accept_and_report(client);
    }
  }
  if (strncmp(argv[2], "input", 5) == 0) {
    if (argc == 4) {
      kr_mixer_create_portgroup(client, argv[3], "input", "jack", 2);
    }
    if (argc == 5) {
      kr_mixer_create_portgroup(client, argv[3], "input", "jack", atoi(argv[4]));
    }
  }
  if (strncmp(argv[2], "output", 6) == 0) {
    if (argc == 4) {
      kr_mixer_create_portgroup(client, argv[3], "output", "jack", 2);
    }
    if (argc == 5) {
      kr_mixer_create_portgroup(client, argv[3], "output", "jack", atoi(argv[4]));
    }
  }
  if (strncmp(argv[2], "auxout", 6) == 0) {
    if (argc == 4) {
      kr_mixer_create_portgroup(client, argv[3], "auxout", "jack", 2);
    }
    if (argc == 5) {
      kr_mixer_create_portgroup(client, argv[3], "auxout", "jack", atoi(argv[4]));
    }
  }
  if (strncmp(argv[2], "plug", 4) == 0) {
    if (argc == 5) {
      kr_mixer_plug_portgroup(client, argv[3], argv[4]);
    }
  }
  if (strncmp(argv[2], "unplug", 6) == 0) {
    if (argc == 4) {
      kr_mixer_unplug_portgroup(client, argv[3], "");
    }
    if (argc == 5) {
      kr_mixer_unplug_portgroup(client, argv[3], argv[4]);
    }
  }
  if (strncmp(argv[2], "map", 3) == 0) {
    if (argc == 6) {
      kr_mixer_update_portgroup_map_channel(client, argv[3], atoi(argv[4]), atoi(argv[5]));
    }
  }
  if (strncmp(argv[2], "mixmap", 3) == 0) {
    if (argc == 6) {
      kr_mixer_update_portgroup_mixmap_channel(client, argv[3], atoi(argv[4]), atoi(argv[5]));
    }
  }
  if (strncmp(argv[2], "crossfade", 9) == 0) {
    if (argc == 4) {
      kr_mixer_set_portgroup_crossfade_group(client, argv[3], "");
    }
    if (argc == 5) {
      kr_mixer_set_portgroup_crossfade_group(client, argv[3], argv[4]);
    }
  }
  if ((strncmp(argv[2], "rmcrossfade", 11) == 0) || (strncmp(argv[2], "nocrossfade", 11) == 0)) {
    if (argc == 4) {
      kr_mixer_set_portgroup_crossfade_group(client, argv[3], "");
    }
  }
  if (strncmp(argv[2], "xmms2", 5) == 0) {
    if (argc == 5) {
      if ((strncmp(argv[4], "play", 4) == 0) || (strncmp(argv[4], "pause", 5) == 0) ||
        (strncmp(argv[4], "stop", 4) == 0) || (strncmp(argv[4], "next", 4) == 0) ||
        (strncmp(argv[4], "prev", 4) == 0)) {
        kr_mixer_portgroup_xmms2_cmd(client, argv[3], argv[4]);
        return 0;
      } else {
        kr_mixer_bind_portgroup_xmms2(client, argv[3], argv[4]);
      }
    }
  }
  if (strncmp(argv[2], "noxmms2", 7) == 0) {
    if (argc == 4) {
      kr_mixer_unbind_portgroup_xmms2(client, argv[3]);
    }
  }
  if (((argc == 5) || (argc == 6)) &&
      (((strlen(argv[2]) == 1) && (strncmp(argv[2], "s", 1) == 0)) ||
       ((strlen(argv[2]) == 3) && (strncmp(argv[2], "set", 3) == 0)))) {
    memset (&uc, 0, sizeof (uc));
    if (kr_string_to_address (argv[3], &uc.address)) {
      if (argc == 5) {
        uc.value.real = atof(argv[4]);
        uc.duration = 0;
      }
      if (argc == 6) {
        uc.value.real = atof(argv[4]);
        uc.duration = atoi(argv[5]);
      }
      kr_unit_control_set(client, &uc);
    }
  }
  if (((argc == 3) || (argc == 4) || (argc == 5)) &&
      (((strlen(argv[2]) > 2) && (strchr(argv[2], '/') != NULL)) ||
       (0))) {
    memset (&uc, 0, sizeof (uc));
    if (kr_string_to_address (argv[2], &uc.address)) {
      if (argc == 3) {
        if (kr_address_has_control(&uc.address)) {
          /* FIXME not implemented */
          /* kr_unit_control_get(client, &uc); */
          kr_unit_info(client, &uc.address);
        } else {
          kr_unit_info(client, &uc.address);
        }
        kr_delivery_accept_and_report(client);
      }
      if ((argc == 4) || (argc == 5)) {
        if (kr_address_has_control (&uc.address)) {
          kr_unit_control_data_type_from_address(&uc.address, &uc.data_type);
          if (uc.data_type == KR_FLOAT) {
            uc.value.real = atof(argv[3]);
          }
          if (uc.data_type == KR_INT32) {
            uc.value.integer = atoi(argv[3]);
          }
          if (uc.data_type == KR_STRING) {
            uc.value.string = argv[3];
          }
          if (uc.data_type == KR_CHAR) {
            uc.value.byte = argv[3][0];
          }
          if (argc == 4) {
            uc.duration = 0;
          }
          if (argc == 5) {
            uc.duration = atoi(argv[4]);
          }
          kr_unit_control_set(client, &uc);
        }
      }
    }
  }
  if ((argc == 4) && ((strncmp(argv[2], "rm", 2) == 0) && (strlen(argv[2]) == 2)) &&
      (((strlen(argv[3]) > 2) && (strchr(argv[3], '/') != NULL)) ||
       (0))) {
    memset (&uc, 0, sizeof (uc));
    if (kr_string_to_address (argv[3], &uc.address)) {
      kr_unit_destroy(client, &uc.address);
    }
  }
  /* Krad Compositor Commands */
  if ((strncmp(argv[2], "lc", 2) == 0) && (strlen(argv[2]) == 2)) {
    if (argc == 3) {
      kr_compositor_subunit_list(client);
      kr_delivery_accept_and_report(client);
    }
  }
  if ((strlen(argv[2]) == 9) && (strncmp(argv[2], "addsprite", 9) == 0)) {
    if (argc == 4) {
      kr_compositor_subunit_create(client, KR_SPRITE, argv[3], NULL);
    }
  }
  if ((strlen(argv[2]) == 7) && (strncmp(argv[2], "addtext", 7) == 0)) {
    if (argc == 4) {
      kr_compositor_subunit_create(client, KR_TEXT, argv[3], NULL);
    }
    if (argc == 5) {
      kr_compositor_subunit_create(client, KR_TEXT, argv[3], argv[4]);
    }
  }
  if ((strlen(argv[2]) > 5) && (strncmp(argv[2], "addvec", 6) == 0)) {
    if (argc == 4) {
      kr_compositor_subunit_create(client, KR_VECTOR, argv[3], NULL);
    }
  }
  if ((strncmp(argv[2], "comp", 4) == 0) ||
      ((strncmp(argv[2], "c", 1) == 0) && (strlen(argv[2]) == 1))) {
    if (argc == 3) {
      kr_compositor_info_request(client);
      kr_delivery_accept_and_report(client);
    }
  }
  if (strncmp(argv[2], "res", 3) == 0) {
    if (argc == 5) {
      kr_compositor_set_resolution(client, atoi(argv[3]), atoi(argv[4]));
    }
  }
  if (strncmp(argv[2], "fps", 3) == 0) {
    if (argc == 4) {
      kr_compositor_set_frame_rate(client, atoi(argv[3]) * 1000, 1000);
    }
    if (argc == 5) {
      kr_compositor_set_frame_rate(client, atoi(argv[3]), atoi(argv[4]));
    }
  }
  kr_client_destroy(&client);
  return 0;
}
