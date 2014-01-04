static int json_to_crate(kr_iws_client_t *client, char *json) {
  kr_adapter_path_info pinfo;
  int res;
  char *json_payload = "\"payload\":";
  char js[2048];
  char *p;
  char *payload_pos;

  memset(js,0,sizeof(js));

  p = strchr(json,'"');

  if (!p) {
    return -1;
  }

  p[0] = '\0';

  if (!strncmp(json,"add_adapter",11)) {
    p[0] = '"';
    json[strlen(json)-1] = '\0';
    payload_pos = strstr(json,json_payload);
    if (payload_pos) {
      memset(&pinfo,0,sizeof(pinfo));
      //res = kr_adapter_path_info_fr_json();
      sprintf(js,"%s",&payload_pos[strlen(payload_pos)]);
      interweb_ws_pack(client, (uint8_t *)js, sizeof(js));
    }
  }
  
  p[0] = '"';
  
  return 0;
}

static int handle_json(kr_iws_client_t *client, char *json, size_t len) {

  int pos;
  int got_register;
  int got_unregister;
  int got_call;
  int got_answer;
  int got_candidate;
  static const int shortest_json_len = 14;
  static const int longest_json_len = 4096;
  static const char *json_pre = "{\"ctrl\":\"";
  static const char *json_pre_rtc = "{\"rtc\":\"";
  kr_unit_control_t uc;
  int cmplen;
  size_t addr_len;
  size_t dur_len;
  char *addr_str;
  char message[4096];
  char name[KR_WEBRTC_NAME_MAX];
  int i;
  int name_len;

  got_register = 0;
  got_unregister = 0;
  got_call = 0;
  got_answer = 0;
  got_candidate = 0;
  dur_len = 0;
  addr_len = 0;
  pos = 0;

  if (len < shortest_json_len) {
    printk("JSON IN is[%zu]: %s", len, json);
    printke("JSON json is too short");
    return -1;
  }
  if (len > longest_json_len) {
    printk("JSON IN is[%zu]: %s", len, json);
    printke("JSON json is too long");
    return -1;
  }
  if (json[len - 1] != '}') {
    printk("JSON IN is[%zu]: %s", len, json);
    printke("JSON json doesn't end well");
    return -1;
  }
  cmplen = strlen(json_pre);
  if (strncmp(json, json_pre, cmplen) == 0) {

    json_to_crate(client,json + cmplen);

    memset (&uc, 0, sizeof (uc));
    pos = cmplen;
    addr_len = strcspn(json + pos, " ");
    if (addr_len == 0) {
      printk("JSON IN is[%zu]: %s", len, json);
      printke("JSON json addr err");
      return -1;
    }
    json[pos + addr_len] = '\0';
    addr_str = json + pos;
    //printk("address string is: %s", addr_str);
    if (!(kr_string_to_address(addr_str, &uc.address))) {
      printke("Could not parse address");
      return -1;
    }
    pos += addr_len + 1;
    printk("rest is: %s", json + pos);
    if ((pos + 3) > len) {
      printke("could not find value part");
      return -1;
    }
    if (kr_unit_control_data_type_from_address(&uc.address, &uc.data_type) != 1) {
      printke("could not determine data type of control");
      return -1;
    }
    if (uc.data_type == KR_FLOAT) {
      uc.value.real = atof(json + pos);
    }
    if (uc.data_type == KR_INT32) {
      uc.value.integer = atoi(json + pos);
    }
    if (uc.data_type == KR_CHAR) {
      uc.value.byte = json[pos];
    }
    dur_len = strcspn(json + pos + 1, " ");
    if (dur_len != 0) {
      //printk("duration found: %s", json + pos + dur_len + 1);
      uc.duration = atoi(json + pos + dur_len + 1);
      //printk("duration: %d", uc.duration);
    }
    if (kr_unit_control_set(client->ws.krclient, &uc) != 0) {
      printke("could not set control");
      return -1;
    }
  } else {
    cmplen = strlen(json_pre_rtc);
    if (strncmp(json, json_pre_rtc, cmplen) == 0) {
      pos = cmplen;
      if (strncmp(json + pos, "register", 8) == 0) {
        pos += 11;
        if (strncmp(json + pos, "name", 4) == 0) {
          pos += 7;
          got_register = 1;
          strncpy(name, json + pos, KR_WEBRTC_NAME_MAX);
          cmplen = strnlen(name, KR_WEBRTC_NAME_MAX);
          name[cmplen - 2] = '\0';
          printk("got register");
        }
      } else if (strncmp(json + pos, "unregister", 10) == 0) {
        pos += 13;
        got_unregister = 1;
        printk("got unregister");
      } else if (strncmp(json + pos, "call", 4) == 0) {
        pos += 7;
        printk("parsed call");
        if (strncmp(json + pos, "name", 4) == 0) {
          pos += 7;
          name_len = strcspn(json + pos, "\"");
          strncpy(name, json + pos, name_len);
          name[name_len] = '\0';
          pos += name_len + 3;
          printk("parsed name: <%s>", name);
          if (strncmp(json + pos, "sdp", 3) == 0) {
            pos += 6;
            got_call = 1;
            addr_len = 4096;
            strncpy(message, json + pos, addr_len);
            cmplen = strnlen(message, addr_len);
            message[cmplen - 2] = '\0';
            printk("parsed spd: <%s>", message);
          }
        }

      } else if (strncmp(json + pos, "answer", 6) == 0) {
        pos += 9;
        printk("parsed answer");
        if (strncmp(json + pos, "name", 4) == 0) {
          pos += 7;
          name_len = strcspn(json + pos, "\"");
          strncpy(name, json + pos, name_len);
          name[name_len] = '\0';
          pos += name_len + 3;
          printk("parsed name: <%s>", name);
          if (strncmp(json + pos, "sdp", 3) == 0) {
            pos += 6;
            got_answer = 1;
            addr_len = 4096;
            strncpy(message, json + pos, addr_len);
            cmplen = strnlen(message, addr_len);
            message[cmplen - 2] = '\0';
            printk("parsed spd: <%s>", message);
          }
        }
      } else if (strncmp(json + pos, "candidate", 9) == 0) {
        pos += 12;
        printk("parsed candidate");
        if (strncmp(json + pos, "user", 4) == 0) {
          pos += 7;
          name_len = strcspn(json + pos, "\"");
          strncpy(name, json + pos, name_len);
          name[name_len] = '\0';
          pos += name_len + 2;
          printk("parsed name: <%s>", name);
          got_candidate = 1;
          addr_len = 4096;
          strncpy(message, json + pos, addr_len);
          cmplen = strnlen(message, addr_len);
          message[cmplen - 1] = '\0';
          printk("parsed candidate: <%s>", message);
        }
      }
    } else {
      printk("JSON IN is[%zu]: %s", len, json);
      printke("JSON json seems to be in a bad way");
      return -1;
    }
  }

  if (got_register == 1) {
    kr_webrtc_register(client, name);
  } else if (got_unregister == 1) {
    kr_webrtc_unregister(client);
  } else if (got_call == 1) {
    for (i = 0; i < KR_IWS_MAX_CLIENTS; i++) {
      printk("got call");
      if (strncmp(client->server->clients[i].webrtc_user.name, name,
       KR_WEBRTC_NAME_MAX) == 0) {
        kr_webrtc_call(&(client->server->clients[i]), name, client->webrtc_user.name, message);
      }
    }
  } else if (got_answer == 1) {
    for (i = 0; i < KR_IWS_MAX_CLIENTS; i++) {
      printk("got answer");
      if (strncmp(client->server->clients[i].webrtc_user.name, name,
       KR_WEBRTC_NAME_MAX) == 0) {
        kr_webrtc_answer(&(client->server->clients[i]), name, client->webrtc_user.name, message);
      }
    }
  } else if (got_candidate == 1) {
    for (i = 0; i < KR_IWS_MAX_CLIENTS; i++) {
      printk("got candidate");
      if (strncmp(client->server->clients[i].webrtc_user.name, name,
       KR_WEBRTC_NAME_MAX) == 0) {
        kr_webrtc_candidate(&(client->server->clients[i]), name, client->webrtc_user.name, message);
      }
    }
  }
  return 0;
}

void krad_websocket_remove_portgroup (kr_iws_client_t *client,
 kr_address_t *address) {

  char json[192];

  snprintf(json, sizeof(json), "[{\"com\":\"kradmixer\","
   "\"ctrl\":\"remove_portgroup\",\"portgroup_name\":\"%s\"}]",
   address->id.name);

  interweb_ws_pack(client, (uint8_t *)json, strlen(json));
}

void krad_websocket_set_portgroup_eff(kr_iws_client_t *client,
 kr_address_t *address, float value) {

  char json[256];

  snprintf(json, sizeof(json), "[{\"com\":\"kradmixer\","
   "\"ctrl\":\"effect_control\",\"portgroup_name\":\"%s\","
   "\"effect_name\":\"%s\",\"effect_num\":%d,"
   "\"control_name\":\"%s\","
   "\"value\":%g}]", address->id.name,
   kr_strsfxeft(address->sub_id + 1), address->sub_id2,
   kr_strsfxeftctl(address->control.effect_control), value);

  interweb_ws_pack(client, (uint8_t *)json, strlen(json));
}

void krad_websocket_remove_subunit(kr_iws_client_t *client,
 kr_address_t *address) {

  char json[192];

  snprintf(json, sizeof(json), "[{\"com\":\"kradcompositor\","
   "\"ctrl\":\"remove_subunit\",\"subunit_type\":\"%s\","
   "\"subunit_id\":\"%d\"}]",
   kr_comp_strfsubtype(address->path.subunit.compositor_subunit),
   address->id.number);

  interweb_ws_pack(client, (uint8_t *)json, strlen(json));
}

void krad_websocket_set_portgroup_control ( kr_iws_client_t *client,
 kr_address_t *address, float value) {

  char json[192];

  snprintf(json, sizeof(json), "[{\"com\":\"kradmixer\","
   "\"ctrl\":\"control_portgroup\",\"portgroup_name\":\"%s\",\"control_name\":"
   "\"%s\", \"value\":%g}]",
   address->id.name,
   kr_mixer_ctltostr(address->control.portgroup_control),
   value);

  interweb_ws_pack(client, (uint8_t *)json, strlen(json));
}

void krad_websocket_set_portgroup_peak ( kr_iws_client_t *client,
 kr_address_t *address, float value) {

  char json[192];

  snprintf(json, sizeof(json), "[{\"com\":\"kradmixer\","
   "\"ctrl\":\"peak_portgroup\",\"portgroup_name\":\"%s\", \"value\":%g}]",
   address->id.name, value);

  interweb_ws_pack(client, (uint8_t *)json, strlen(json));
}

void krad_websocket_update_portgroup ( kr_iws_client_t *client,
 kr_address_t *address, char *value ) {

  char json[192];

  snprintf(json, sizeof(json), "[{\"com\":\"kradmixer\","
   "\"ctrl\":\"update_portgroup\",\"portgroup_name\":\"%s\",\"control_name\":"
   "\"%s\", \"value\":\"%s\"}]",
   address->id.name,
   kr_mixer_ctltostr(address->control.portgroup_control),
   value);

  interweb_ws_pack(client, (uint8_t *)json, strlen(json));
}

void krad_websocket_set_tag (kr_iws_client_t *client, char *tag_item,
 char *tag_name, char *tag_value) {

  char json[1024];

  snprintf(json, sizeof(json), "[{\"com\":\"kradradio\","
   "\"ctrl\":\"tag\",\"tag_item\":\"%s\",\"tag_name\":\"%s\","
   "\"tag_value\":\"%s\"}]",
   tag_item, tag_name, tag_value);

  interweb_ws_pack(client, (uint8_t *)json, strlen(json));
}

void krad_websocket_set_cpu_usage (kr_iws_client_t *client, int usage) {

  char json[96];

  snprintf(json, sizeof(json), "[{\"com\":\"kradmixer\","
   "\"ctrl\":\"cpu\",\"system_cpu_usage\":\"%d\"}]",
   usage);

  interweb_ws_pack(client, (uint8_t *)json, strlen(json));
}

void krad_websocket_update_subunit(kr_iws_client_t *client,
 kr_crate_t *crate) {

  int pos;
  char json[2048];
  kr_address_t *address;

  address = crate->addr;
  pos = 0;

  if (crate->addr->path.unit == KR_COMPOSITOR) {

    pos += snprintf(json + pos, sizeof(json) - pos,
     "[{\"com\":\"kradcompositor\","
     "\"ctrl\":\"update_subunit\",\"subunit_type\":\"%s\",\"subunit_id\":%d,"
     "\"control_name\":\"%s\",",
     kr_comp_strfsubtype(address->path.subunit.compositor_subunit),
     address->id.number,
     kr_compositor_control_to_string(address->control.compositor_control));

    if (kr_crate_contains_float (crate)) {
      pos += snprintf(json + pos, sizeof(json) - pos, "\"value\":%g",
       crate->real);
    } else {
      if (kr_crate_contains_int (crate)) {
        pos += snprintf(json + pos, sizeof(json) - pos, "\"value\":%d",
         crate->integer);
      }
    }

    pos += snprintf(json + pos, sizeof(json) - pos, "}]");
    interweb_ws_pack(client, (uint8_t *)json, strlen(json));
  }
}

static int crate_to_json(kr_iws_client_t *client, kr_crate_t *crate) {

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

static int krad_delivery_handler(kr_iws_client_t *client) {

  kr_crate_t *crate;
  char *string;

  string = NULL;
  crate = NULL;

  kr_delivery_recv(client->ws.krclient);

  while ((kr_delivery_get(client->ws.krclient, &crate) > 0) &&
         (crate != NULL)) {

    /* Subunit updated */
    if (kr_crate_notice (crate) == EBML_ID_KRAD_SUBUNIT_CONTROL) {
      if (crate->addr->path.unit == KR_MIXER) {
        if (crate->addr->path.subunit.mixer_subunit == KR_PORTGROUP) {
          if (kr_crate_contains_float (crate)) {
            if (crate->addr->control.portgroup_control == KR_PEAK) {
              krad_websocket_set_portgroup_peak(client, crate->addr, crate->real);
            } else {
              krad_websocket_set_portgroup_control(client, crate->addr, crate->real);
            }
          } else {
            if (crate->addr->control.portgroup_control == KR_CROSSFADE_GROUP) {
              if (kr_uncrate_string (crate, &string)) {
                krad_websocket_update_portgroup(client, crate->addr, string);
                kr_string_recycle (&string);
              } else {
                krad_websocket_update_portgroup(client, crate->addr, "");
              }
            }
          }
        }
        if (crate->addr->path.subunit.mixer_subunit == KR_EFFECT) {
          if (kr_crate_contains_float (crate)) {
            krad_websocket_set_portgroup_eff(client, crate->addr, crate->real);
          }
        }
      }
      if (crate->addr->path.unit == KR_COMPOSITOR) {
        krad_websocket_update_subunit(client, crate);
      }
      kr_crate_recycle (&crate);
      continue;
    }
    /* Subunit Destroyed */
    if (kr_crate_notice (crate) == EBML_ID_KRAD_RADIO_UNIT_DESTROYED) {
      if ((crate->addr->path.unit == KR_MIXER) &&
          (crate->addr->path.subunit.mixer_subunit == KR_PORTGROUP)) {
        krad_websocket_remove_portgroup(client, crate->addr);
      }
      if (crate->addr->path.unit == KR_COMPOSITOR) {
        krad_websocket_remove_subunit(client, crate->addr);
      }
      kr_crate_recycle (&crate);
      continue;
    }
    /* Initial list of subunits or subunit created */
    if (kr_crate_loaded(crate)) {
      crate_to_json(client, crate);
      kr_crate_recycle (&crate);
      continue;
    }
  }

  return 0;
}

