static int handle_json(kr_web_client *client, char *json, size_t len) {
  return 0;
}

static int crate_to_json(kr_web_client *client, kr_crate *crate) {
  uber_St uber;
  char json[2048];
  char final_json[4096];
  char *ctrl;
  char *com;
  int res = 0;
  ctrl = "";
  com = "";
  switch (crate->contains) {
    case 0:
      return 0;
    case KR_MIXER:
      uber.actual = crate->inside.mixer;
      uber.type = JSON_KR_MIXER_INFO;
      com = "kradmixer";
      ctrl = "set_mixer_params";
      break;
    case KR_PORTGROUP:
      uber.actual = crate->inside.mixer;
      uber.type = JSON_KR_MIXER_PATH_INFO;
      com = "kradmixer";
      ctrl = "add_portgroup";
      break;
    case KR_SPRITE:
      uber.actual = crate->inside.sprite;
      uber.type = JSON_KR_SPRITE_INFO;
      com = "kradcompositor";
      ctrl = "add_sprite";
      break;
    case KR_VECTOR:
      uber.actual = crate->inside.vector;
      uber.type = JSON_KR_VECTOR_INFO;
      com = "kradcompositor";
      ctrl = "add_vector";
      break;
    case KR_TEXT:
      uber.actual = crate->inside.text;
      uber.type = JSON_KR_TEXT_INFO;
      com = "kradcompositor";
      ctrl = "add_text";
      break;
    case KR_VIDEOPORT:
      uber.actual = crate->inside.text;
      uber.type = JSON_KR_COMPOSITOR_PATH_INFO;
      com = "kradcompositor";
      ctrl = "add_videoport";
      break;
  }
  res += info_pack_to_json(&json[res],&uber,sizeof(json));
  if (res) {
    snprintf(final_json, sizeof(final_json), "[{\"com\":\"%s\","
      "\"ctrl\":\"%s\",\"content\": %s }]",com,ctrl,json);
    interweb_ws_pack(client, (uint8_t *)final_json, strlen(final_json));
    sprintf(json,"{\"debug\" : \"Ok\", \"res\" : %d}",res);
    interweb_ws_pack(client, (uint8_t *)json, strlen(json));
    return 1;
  }
  return 0;
}

static int krad_delivery_handler(kr_web_client *client) {
  kr_crate *crate;
  char *string;
  string = NULL;
  crate = NULL;
  kr_delivery_recv(client->ws.krclient);
  while ((kr_delivery_get(client->ws.krclient, &crate) > 0)
   && (crate != NULL)) {
      crate_to_json(client, crate);
      kr_crate_recycle(&crate);
      continue;
  }
  return 0;
}
