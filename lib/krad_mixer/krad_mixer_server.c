#include "krad_mixer_server.h"

static kr_mixer_path *path_find(kr_mixer *mixer, char *name);
static kr_mixer_path *path_iter(kr_mixer *mixer, int *count);
static int get_info(kr_mixer *mixer, kr_mixer_info *info);

/*
kr_mixer_path *path_iter(kr_mixer *mixer, int *count) {
  kr_mixer_path *path;
  if (mixer == NULL) return NULL;
  while ((path = kr_pool_iterate_active(mixer->path_pool, count))) {
    if ((path->state == KR_MXP_READY) || (path->state == KR_MXP_ACTIVE)) {
      return path;
    }
  }
  return NULL;
}

kr_mixer_path *path_find(kr_mixer *mixer, char *name) {
  int i;
  int len;
  kr_mixer_path *path;
  if ((mixer == NULL) || (name == NULL)) return NULL;
  i = 0;
  len = strlen(name);
  if (len < 1) return NULL;
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_READY) || (path->state == KR_MXP_ACTIVE)) {
      if ((strlen(path->name) == len)
       && (strncmp(name, path->name, len) == 0)) {
        return path;
      }
    }
  }
  return NULL;
}
*/
/*
  path = path_find(mixer, setup->info.name);
  if (path != NULL) {
    printke("mixer mkpath path with that name already exists");
    return NULL;
  }
*/
/*
int get_info(kr_mixer *mixer, kr_mixer_info *info) {
  kr_mixer_path *path;
  int i;
  if ((mixer == NULL) || (info == NULL)) return -1;
  i = 0;
  memset(info, 0, sizeof(kr_mixer_info));
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_READY) || (path->state == KR_MXP_ACTIVE)) {
      if (path->type == KR_MXR_INPUT) {
        info->inputs++;
      }
      if (path->type == KR_MXR_OUTPUT) {
        info->outputs++;
      }
      if (path->type == KR_MXR_BUS) {
        info->buses++;
      }
    }
  }
  info->period_size = mixer->period_size;
  info->sample_rate = mixer->sample_rate;
  strncpy(info->clock, "Something", sizeof(info->clock));
  return 0;
}

*/

static kr_mixer_path *path_find(kr_mixer *mixer, char *name) {
  return NULL;
}

static kr_mixer_path *path_iter(kr_mixer *mixer, int *count) {
  return NULL;
}

int get_info(kr_mixer *mixer, kr_mixer_info *info) {
  if ((mixer == NULL) || (info == NULL)) return -1;
  memset(info, 0, sizeof(kr_mixer_info));
  return 0;
}

int kr_mixer_command(kr_io2_t *in, kr_io2_t *out, kr_radio_client *client) {
  kr_mixer_info mixer_info;
  kr_sfx *sfx;
  kr_sfx_cmd cmd;
  kr_mixer_path *path;
  kr_mixer_path_info info;
  char name[64];
  char control[16];
  void *ptr;
  float floatval;
  int i;
  kr_radio *radio;
  kr_mixer *mixer;
  kr_address_t address;
  unsigned char *response;
  unsigned char *payload;
  kr_ebml ebml_in;
  kr_ebml ebml_out;
  uint32_t command;
  uint32_t element;
  uint64_t size;
  int ret;
  char string[512];
  uint32_t numbers[10];
  kr_app_server *as;
  int duration;
  i = 0;
  duration = 0;
  ptr = NULL;
  radio = client->radio;
  mixer = radio->mixer;
  as = radio->app;
  path = NULL;
  name[0] = '\0';
  control[0] = '\0';
  string[0] = '\0';
  if (!(kr_io2_has_in(in))) {
    return 0;
  }
  kr_ebml2_set_buffer(&ebml_in, in->rd_buf, in->len);
  ret = kr_ebml2_unpack_id(&ebml_in, &command, &size);
  if ((ret < 0) || (command != EBML_ID_KRAD_MIXER_CMD)) {
    printke("krad_mixer_command invalid EBML ID Not found");
    return 0;
  }
  ret = kr_ebml2_unpack_id(&ebml_in, &command, &size);
  if (ret < 0) {
    printke("krad_mixer_command EBML ID Not found");
    return 0;
  }
  kr_ebml2_set_buffer(&ebml_out, out->buf, out->space);
  switch (command) {
    case EBML_ID_KRAD_MIXER_CMD_SET_CONTROL:
      kr_ebml2_unpack_element_string(&ebml_in, &element, name, sizeof(name));
      kr_ebml2_unpack_element_string(&ebml_in, &element, control, sizeof(control));
      kr_ebml2_unpack_element_float(&ebml_in, &element, &floatval);
      kr_ebml2_unpack_element_uint32(&ebml_in, &element, &numbers[0]);
      duration = numbers[0];
      if ((duration == 0) && (kr_app_server_current_client_is_subscriber(as))) {
        ptr = as->current_client;
      }
      path = path_find(mixer, name);
      if (path) {
        kr_mixer_path_ctl(path, control, floatval, duration, ptr);
      }
      break;
    case EBML_ID_KRAD_MIXER_CMD_SET_EFFECT_CONTROL:
      kr_ebml2_unpack_element_string(&ebml_in, &element, name, sizeof(name));
      kr_ebml2_unpack_element_uint32(&ebml_in, &element, &numbers[0]);
      kr_ebml2_unpack_element_uint32(&ebml_in, &element, &numbers[5]);
      kr_ebml2_unpack_element_string(&ebml_in, &element, control, sizeof(control));
      kr_ebml2_unpack_element_float(&ebml_in, &element, &floatval);
      kr_ebml2_unpack_element_uint32(&ebml_in, &element, &numbers[6]);
      kr_ebml2_unpack_element_uint32(&ebml_in, &element, &numbers[7]);
      path = path_find(mixer, name);
      if (path) {
        duration = numbers[6];
        if ((duration == 0) && (kr_app_server_current_client_is_subscriber(as))) {
          ptr = as->current_client;
        }
        sfx = kr_mixer_path_sfx_kludge(path);
        if (sfx) {
          cmd.control = KR_SFX_EFFECT_CONTROL;
          cmd.effect = numbers[0];
          cmd.control_id = numbers[5];
          cmd.control_str = control;
          cmd.value = floatval;
          cmd.duration = duration;
          cmd.easing = numbers[7];
          kr_sfx_ctl(sfx, &cmd);
        }
      }
      break;
    case EBML_ID_KRAD_MIXER_CMD_LIST_PORTGROUPS:
      address.path.unit = KR_MIXER;
      address.path.subunit.mixer_subunit = KR_PORTGROUP;
      while ((path = path_iter(mixer, &i))) {
        kr_mixer_get_path_info(path, &info);
        strcpy(address.id.name, info.name);
        krad_radio_address_to_ebml2(&ebml_out, &response, &address);
        kr_ebml_pack_uint32(&ebml_out,
                             EBML_ID_KRAD_RADIO_MESSAGE_TYPE,
                             EBML_ID_KRAD_SUBUNIT_INFO);
        kr_ebml2_start_element(&ebml_out, EBML_ID_KRAD_RADIO_MESSAGE_PAYLOAD, &payload);
        kr_mixer_path_info_to_ebml(&ebml_out, (void *)&info);
        kr_ebml2_finish_element(&ebml_out, payload);
        kr_ebml2_finish_element(&ebml_out, response);
      }
      break;
    case EBML_ID_KRAD_MIXER_CMD_PORTGROUP_INFO:
      kr_ebml2_unpack_element_string(&ebml_in, &element, name, sizeof(name));
      path = path_find(mixer, name);
      if (path != NULL) {
        address.path.unit = KR_MIXER;
        address.path.subunit.mixer_subunit = KR_PORTGROUP;
        kr_mixer_get_path_info(path, &info);
        strcpy(address.id.name, info.name);
        krad_radio_address_to_ebml2(&ebml_out, &response, &address);
        kr_ebml_pack_uint32(&ebml_out,
                             EBML_ID_KRAD_RADIO_MESSAGE_TYPE,
                             EBML_ID_KRAD_SUBUNIT_INFO);
        kr_ebml2_start_element(&ebml_out, EBML_ID_KRAD_RADIO_MESSAGE_PAYLOAD, &payload);
        kr_mixer_path_info_to_ebml(&ebml_out, (void *)&info); /* new generated call */
        kr_ebml2_finish_element(&ebml_out, payload);
        kr_ebml2_finish_element(&ebml_out, response);
      }
      break;
    case EBML_ID_KRAD_MIXER_CMD_UPDATE_PORTGROUP:
      /*
      kr_ebml2_unpack_element_string(&ebml_in, &element, name, sizeof(name));
      kr_ebml2_unpack_id(&ebml_in, &element, &size);
      if (element == EBML_ID_KRAD_MIXER_PORTGROUP_CROSSFADE_NAME) {
        kr_ebml2_unpack_string(&ebml_in, string, size);
        unit = path_find(mixer, name);
        if (unit != NULL) {
          if (unit->crossfader != NULL) {
            kr_mixer_xf_decouple(mixer, unit->crossfader);
            if (strlen(string) == 0) {
              return 0;
            }
          }
          if (strlen(string) > 0) {
            unit2 = path_find(mixer, string);
            if (unit2 != NULL) {
              if (unit2->crossfader != NULL) {
                kr_mixer_xf_decouple(mixer, unit2->crossfader);
              }
              if (unit != unit2) {
                kr_mixer_xf_couple(mixer, unit, unit2);
              }
            }
          }
        }
      }
      if (element == EBML_ID_KRAD_MIXER_MAP_CHANNEL) {
        kr_ebml2_unpack_element_uint32(&ebml_in, &element, &numbers[0]);
        kr_ebml2_unpack_element_uint32(&ebml_in, &element, &numbers[1]);
        unit = path_find(mixer, name);
        if (unit != NULL) {
          kr_mixer_channel_move(unit, numbers[0], numbers[1]);
        }
      }
      if (element == EBML_ID_KRAD_MIXER_MIXMAP_CHANNEL) {
        kr_ebml2_unpack_element_uint32(&ebml_in, &element, &numbers[0]);
        kr_ebml2_unpack_element_uint32(&ebml_in, &element, &numbers[1]);
        unit = path_find(mixer, name);
        if (unit != NULL) {
          kr_mixer_channel_copy(unit, numbers[0], numbers[1]);
        }
      }
      break;
      */
    case EBML_ID_KRAD_MIXER_CMD_GET_INFO:
      address.path.unit = KR_MIXER;
      address.path.subunit.mixer_subunit = KR_UNIT;
      krad_radio_address_to_ebml2(&ebml_out, &response, &address);
      kr_ebml_pack_uint32(&ebml_out,
                           EBML_ID_KRAD_RADIO_MESSAGE_TYPE,
                           EBML_ID_KRAD_UNIT_INFO);
      kr_ebml2_start_element(&ebml_out, EBML_ID_KRAD_RADIO_MESSAGE_PAYLOAD, &payload);
      get_info(mixer, &mixer_info);
      kr_mixer_info_to_ebml(&ebml_out, (void *)&mixer_info);
      kr_ebml2_finish_element(&ebml_out, payload);
      kr_ebml2_finish_element(&ebml_out, response);
      break;
    default:
      return -1;
  }
  if (((ebml_out.pos > 0) || (command == EBML_ID_KRAD_MIXER_CMD_LIST_PORTGROUPS)) &&
       (!kr_app_server_current_client_is_subscriber(as))) {
    krad_radio_pack_shipment_terminator(&ebml_out);
  }
  kr_io2_pulled(in, ebml_in.pos);
  kr_io2_advance(out, ebml_out.pos);
  return 0;
}
