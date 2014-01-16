void debug_print_headers(kr_web_client *client) {
  char *headers;
  headers = (char *)client->in->rd_buf;
  printk("Headers length: %d", client->hdr_pos);
  printk("%s", headers);
}

int find_end_of_client_headers(kr_web_client *client) {
  int i;
  uint8_t *buf;
  buf = client->in->rd_buf;
  for (i = 0; i < client->in->len; i++) {
    if ((buf[i] == '\n') || (buf[i] == '\r')) {
      if (client->hdr_pos != (i - 1)) {
        client->hdr_le = 0;
      }
      client->hdr_pos = i;
      if (buf[i] == '\n') {
        client->hdr_le += 1;
      }
      if (client->hdr_le == 2) {
        buf[client->hdr_pos] = '\0';
        client->hdrs_recvd = 1;
        return 1;
      }
    }
  }
  return 0;
}

int identify_method(kr_web_client *client) {
  uint8_t *buf;
  buf = client->in->rd_buf;
  if (client->hdr_pos < 8) return -1;
  if (memcmp(buf, "GET ", 4) == 0) {
    client->verb = KR_WS_GET;
    return 0;
  }
  if (memcmp(buf, "PUT ", 4) == 0) {
    client->verb = KR_WS_PUT;
    return 0;
  }
  if (memcmp(buf, "HEAD ", 5) == 0) {
    client->verb = KR_WS_HEAD;
    return 0;
  }
  if (memcmp(buf, "SOURCE ", 7) == 0) {
    client->verb = KR_WS_SOURCE;
    return 0;
  }
  if (memcmp(buf, "POST ", 5) == 0) {
    client->verb = KR_WS_POST;
    return 0;
  }
  if (memcmp(buf, "PATCH ", 6) == 0) {
    client->verb = KR_WS_PATCH;
    return 0;
  }
  if (memcmp(buf, "OPTIONS ", 8) == 0) {
    client->verb = KR_WS_OPTIONS;
    return 0;
  }
  return -1;
}

int copy_header(char *header, char *headers, char *header_name, int max) {
  char *pos;
  int32_t len;
  int32_t name_len;
  name_len = strlen(header_name);
  pos = strstr(headers, header_name) + name_len;
  if (pos == NULL) {
    return -1;
  }
  len = strcspn(pos, " \n\r?");
  len = MIN(len, max - 1);
  memcpy(header, pos, len);
  header[len] = '\0';
  return 0;
}

int handle_get(kr_web_client *client) {
  char *headers;
  int32_t ret;
  headers = (char *)client->in->rd_buf;
  if (strstr(headers, "Upgrade: websocket") != NULL) {
    ret = copy_header(client->get, headers, "GET ", sizeof(client->get));
    if (ret < 0) return -1;
    ret = copy_header(client->ws.key, headers, "Sec-WebSocket-Key: ", sizeof(client->ws.key));
    if (ret < 0) return -1;
    ret = copy_header(client->ws.proto, headers, "Sec-WebSocket-Protocol: ", sizeof(client->ws.proto));
    if (ret < 0) return -1;
    client->type = KR_WS_WS;
    kr_io2_pulled(client->in, client->hdr_pos + 1);
    return 0;
  } else {
    if ((strstr(headers, "GET ") != NULL) && (strstr(headers, " HTTP/1") != NULL)) {
      ret = copy_header(client->get, headers, "GET ", sizeof(client->get));
      if (ret < 0) return -1;
      printk("GET IS %s", client->get);
      if (strncmp("/api", client->get, 4) == 0) {
         client->type = KR_WS_API;
         printk("Web Server: REST API Client");
      } else {
        if (!web_client_get_stream(client)) {
          client->type = KR_WS_FILE;
        }
      }
      kr_io2_pulled(client->in, client->hdr_pos);
      return 0;
    }
  }
  return -1;
}

int handle_put(kr_web_client *client) {
  char *buf;
  int mount_len;
  char *mount_start;
  mount_start = NULL;
  mount_len = 0;
  buf = (char *)client->in->rd_buf;
  switch (client->verb) {
    case KR_WS_PUT:
      mount_start = buf + 4;
      mount_len = strcspn(mount_start, " &?\n\r");
      break;
    case KR_WS_SOURCE:
      mount_start = buf + 7;
      mount_len = strcspn(mount_start, " &?\n\r");
      break;
    default:
      return -1;
  }
  if ((mount_len < 5) || (mount_len > 127)) return -1;
  client->mount[mount_len] = '\0';
  memcpy(client->mount, mount_start, mount_len);
  client->type = KR_WS_STREAM_IN;
  printk("Source/Put client mount is: %s", client->mount);
  return 0;
}

int handle_post(kr_web_client *client) {
  return 0;
}

int handle_unknown_client(kr_web_client *client) {
  int ret;
  if (!client->hdrs_recvd) {
    if (find_end_of_client_headers(client)) {
      ret = identify_method(client);
      if (ret < 0) return -1;
      debug_print_headers(client);
      switch (client->verb) {
        case KR_WS_GET:
          ret = handle_get(client);
          return ret;
        case KR_WS_POST:
          ret = handle_post(client);
          return ret;
        case KR_WS_SOURCE: /* Fallin thru */
        case KR_WS_PUT:
          ret = handle_put(client);
          return ret;
        default:
          return -1;
      }
    }
  }
  if ((!client->hdrs_recvd) && (client->in->len >= 4096)) {
    printk("Web Server: No end to headers in sight after %d bytes",
     client->in->len);
    return -1;
  }
  return 0;
}
