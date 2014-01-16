static int pack_http_header(kr_web_client *client, char *mimetype);
static int pack_http_404_response(kr_web_client *client);

void print_headers(kr_web_client *client) {
  char *headers;
  headers = (char *)client->in->rd_buf;
  printk("Headers length: %d", client->hdr_pos);
  printk("%s", headers);
}

static int pack_http_header(kr_web_client *client, char *mimetype) {
  int pos;
  char *buffer;
  pos = 0;
  buffer = (char *)client->out->buf;
  pos += sprintf(buffer + pos, "HTTP/1.0 200 OK\r\n");
  pos += sprintf(buffer + pos, "Status: 200 OK\r\n");
  pos += sprintf(buffer + pos, "Connection: close\r\n");
  pos += sprintf(buffer + pos, "Server: Krad-Radio\r\n");
  pos += sprintf(buffer + pos, "Content-Type: %s; charset=utf-8\r\n", mimetype);
  pos += sprintf(buffer + pos, "\r\n");
  kr_io2_advance(client->out, pos);
  return pos;
}

static int pack_http_404_response(kr_web_client *client) {
  int32_t pos;
  char *buffer;
  pos = 0;
  buffer = (char *)client->out->buf;
  pos += sprintf(buffer + pos, "HTTP/1.1 404 Not Found\r\n");
  pos += sprintf(buffer + pos, "Status: 404 Not Found\r\n");
  pos += sprintf(buffer + pos, "Connection: close\r\n");
  pos += sprintf(buffer + pos, "Server: Krad-Radio\r\n");
  pos += sprintf(buffer + pos, "Content-Type: text/html; charset=utf-8\r\n");
  pos += sprintf(buffer + pos, "\r\n");
  pos += sprintf(buffer + pos, "404 Not Found");
  kr_io2_advance(client->out, pos);
  return pos;
}

int find_end_of_request_headers(kr_web_client *client) {
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
        client->got_headers = 1;
        return 1;
      }
    }
  }
  return 0;
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

int get_address_method(kr_web_client *client) {
  int ret;
  char *headers;
  headers = (char *)client->in->rd_buf;
  if (client->hdr_pos < 8) return -1;
  if (memcmp(headers, "GET ", 4) == 0) {
    ret = copy_header(client->address, headers, "GET ", sizeof(client->address));
    if (ret < 0) return -1;
    client->method = KR_WS_GET;
    return 0;
  }
  if (memcmp(headers, "PUT ", 4) == 0) {
    ret = copy_header(client->address, headers, "PUT ", sizeof(client->address));
    if (ret < 0) return -1;
    client->method = KR_WS_PUT;
    return 0;
  }
  if (memcmp(headers, "HEAD ", 5) == 0) {
    client->method = KR_WS_HEAD;
    return 0;
  }
  if (memcmp(headers, "SOURCE ", 7) == 0) {
    ret = copy_header(client->address, headers, "SOURCE ", sizeof(client->address));
    if (ret < 0) return -1;
    client->method = KR_WS_SOURCE;
    return 0;
  }
  if (memcmp(headers, "POST ", 5) == 0) {
    client->method = KR_WS_POST;
    return 0;
  }
  if (memcmp(headers, "PATCH ", 6) == 0) {
    client->method = KR_WS_PATCH;
    return 0;
  }
  if (memcmp(headers, "OPTIONS ", 8) == 0) {
    client->method = KR_WS_OPTIONS;
    return 0;
  }
  return -1;
}

int handle_get(kr_web_client *client) {
  char *headers;
  int32_t ret;
  headers = (char *)client->in->rd_buf;
  if (strstr(headers, "Upgrade: websocket") != NULL) {
    ret = copy_header(client->ws.key, headers, "Sec-WebSocket-Key: ", sizeof(client->ws.key));
    if (ret < 0) return -1;
    ret = copy_header(client->ws.proto, headers, "Sec-WebSocket-Protocol: ", sizeof(client->ws.proto));
    if (ret < 0) return -1;
    client->type = KR_WS_WEBSOCKET;
    kr_io2_pulled(client->in, client->hdr_pos + 1);
    return 0;
  } else {
    if ((strstr(headers, "GET ") != NULL) && (strstr(headers, " HTTP/1") != NULL)) {
      printk("GET %s", client->address);
      if (strncmp("/api", client->address, 4) == 0) {
         client->type = KR_WS_REST_API;
         printk("Web Server: REST API Client");
      } else {
        if (!find_stream(client)) {
          client->type = KR_WS_GET_FILE;
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
  switch (client->method) {
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
  client->address[mount_len] = '\0';
  memcpy(client->address, mount_start, mount_len);
  client->type = KR_WS_GET_STREAM;
  printk("Source/Put client mount is: %s", client->address);
  return 0;
}

int handle_post(kr_web_client *client) {
  return 0;
}

int handle_http_request(kr_web_client *client) {
  int ret;
  if (!client->got_headers) {
    if (find_end_of_request_headers(client)) {
      ret = get_address_method(client);
      if (ret < 0) return -1;
      switch (client->method) {
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
          print_headers(client);
          return -1;
      }
    }
  }
  if ((!client->got_headers) && (client->in->len >= 4096)) {
    printke("Web Server: Request headers too long %d bytes", client->in->len);
    return -1;
  }
  return 0;
}
