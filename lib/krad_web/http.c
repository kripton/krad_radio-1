static int pack_http_header(kr_web_client *client, char *mimetype);
static int pack_http_404_response(kr_web_client *client);

void print_headers(kr_web_client *client) {
  char *headers;
  headers = (char *)client->in->rd_buf;
  printk("Headers length: %d", client->http.header_pos);
  printk("%s", headers);
}

static int pack_http_header(kr_web_client *client, char *mimetype) {
  int pos;
  char *buffer;
  pos = 0;
  buffer = (char *)client->out->buf;
  pos += sprintf(buffer + pos, "HTTP/1.1 200 OK\r\n");
  pos += sprintf(buffer + pos, "Status: 200 OK\r\n");
  pos += sprintf(buffer + pos, "Connection: close\r\n");
  pos += sprintf(buffer + pos, "Server: Krad Radio %d\r\n", KR_VERSION);
  pos += sprintf(buffer + pos, "Content-Type: %s; charset=utf-8\r\n", mimetype);
  //pos += sprintf(buffer + pos, "Content-Length: 1822\r\n");
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
  pos += sprintf(buffer + pos, "Server: Krad Radio %d\r\n", KR_VERSION);
  pos += sprintf(buffer + pos, "Content-Type: text/html; charset=utf-8\r\n");
  pos += sprintf(buffer + pos, "\r\n");
  pos += sprintf(buffer + pos, "404 Not Found");
  kr_io2_advance(client->out, pos);
  return pos;
}

int find_end_of_request_headers(kr_web_client *client) {
  int i;
  uint8_t *headers;
  headers = client->in->rd_buf;
  for (i = 0; i < client->in->len; i++) {
    if ((headers[i] == '\n') || (headers[i] == '\r')) {
      if (client->http.header_pos != (i - 1)) {
        client->http.header_endstate = 0;
      }
      client->http.header_pos = i;
      if (headers[i] == '\n') {
        client->http.header_endstate += 1;
      }
      if (client->http.header_endstate == 2) {
        headers[client->http.header_pos] = '\0';
        client->http.got_headers = 1;
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
  if (client->http.header_pos < 8) return -1;
  if (memcmp(headers, "GET ", 4) == 0) {
    ret = copy_header(client->http.address, headers, "GET ", sizeof(client->http.address));
    if (ret < 0) return -1;
    client->http.method = KR_HTTP_GET;
    return 0;
  }
  if (memcmp(headers, "PUT ", 4) == 0) {
    ret = copy_header(client->http.address, headers, "PUT ", sizeof(client->http.address));
    if (ret < 0) return -1;
    client->http.method = KR_HTTP_PUT;
    return 0;
  }
  if (memcmp(headers, "HEAD ", 5) == 0) {
    client->http.method = KR_HTTP_HEAD;
    return 0;
  }
  if (memcmp(headers, "SOURCE ", 7) == 0) {
    ret = copy_header(client->http.address, headers, "SOURCE ", sizeof(client->http.address));
    if (ret < 0) return -1;
    client->http.method = KR_HTTP_SOURCE;
    return 0;
  }
  if (memcmp(headers, "POST ", 5) == 0) {
    ret = copy_header(client->http.address, headers, "POST ", sizeof(client->http.address));
    if (ret < 0) return -1;
    client->http.method = KR_HTTP_POST;
    return 0;
  }
  if (memcmp(headers, "DELETE ", 7) == 0) {
    ret = copy_header(client->http.address, headers, "DELETE ", sizeof(client->http.address));
    if (ret < 0) return -1;
    client->http.method = KR_HTTP_DELETE;
    return 0;
  }
  if (memcmp(headers, "PATCH ", 6) == 0) {
    ret = copy_header(client->http.address, headers, "PATCH ", sizeof(client->http.address));
    if (ret < 0) return -1;
    client->http.method = KR_HTTP_PATCH;
    return 0;
  }
  if (memcmp(headers, "OPTIONS ", 8) == 0) {
    client->http.method = KR_HTTP_OPTIONS;
    return 0;
  }
  return -1;
}

int handle_post(kr_web_client *client) {
  return 0;
}

int handle_put(kr_web_client *client) {
  char *buf;
  int mount_len;
  char *mount_start;
  mount_start = NULL;
  mount_len = 0;
  buf = (char *)client->in->rd_buf;
  /* FIXME hardcoded raaaaa */
  if ((strncmp("/mix", client->http.address, 4) == 0)
   || (strncmp("/com", client->http.address, 4) == 0)
   || (strncmp("/tra", client->http.address, 4) == 0)) {
    client->type = KR_WS_REST_API;
    printk("Web Server: REST PUT API Client");
    kr_io2_pulled(client->in, client->http.header_pos);
    return 0;
  }
  switch (client->http.method) {
    case KR_HTTP_PUT:
      mount_start = buf + 4;
      mount_len = strcspn(mount_start, " &?\n\r");
      break;
    case KR_HTTP_SOURCE:
      mount_start = buf + 7;
      mount_len = strcspn(mount_start, " &?\n\r");
      break;
    default:
      return -1;
  }
  if ((mount_len < 5) || (mount_len > 127)) return -1;
  client->http.address[mount_len] = '\0';
  memcpy(client->http.address, mount_start, mount_len);
  client->type = KR_WS_PUT_STREAM;
  printk("Source/Put client mount is: %s", client->http.address);
  return 0;
}

int handle_get(kr_web_client *client) {
  char *headers;
  headers = (char *)client->in->rd_buf;
  if (websocket_headers_detected(headers)) {
    client->type = KR_WS_WEBSOCKET;
    return 0;
  } else {
    if ((strstr(headers, "GET ") != NULL) && (strstr(headers, " HTTP/1") != NULL)) {
      printk("GET %s", client->http.address);
      /* FIXME hardcoded raaaaa */
      if ((strncmp("/mix", client->http.address, 4) == 0)
       || (strncmp("/com", client->http.address, 4) == 0)
       || (strncmp("/tra", client->http.address, 4) == 0)) {
         client->type = KR_WS_REST_API;
         printk("Web Server: REST GET API Client");
      } else {
        if (!find_stream(client)) {
          client->type = KR_WS_GET_FILE;
        }
      }
      kr_io2_pulled(client->in, client->http.header_pos);
      return 0;
    }
  }
  return -1;
}

int handle_patch(kr_web_client *client) {
  int ret;
  ret = -1;
  printk("PATCH %s", client->http.address);
  /* FIXME hardcoded raaaaa */
  if ((strncmp("/mix", client->http.address, 4) == 0)
   || (strncmp("/com", client->http.address, 4) == 0)
   || (strncmp("/tra", client->http.address, 4) == 0)) {
    client->type = KR_WS_REST_API;
    printk("Web Server: REST PATCH API Client");
    ret = 0;
  }
  kr_io2_pulled(client->in, client->http.header_pos);
  return ret;
}

int handle_delete(kr_web_client *client) {
  int ret;
  ret = -1;
  printk("DELETE %s", client->http.address);
  /* FIXME hardcoded raaaaa */
  if ((strncmp("/mix", client->http.address, 4) == 0)
   || (strncmp("/com", client->http.address, 4) == 0)
   || (strncmp("/tra", client->http.address, 4) == 0)) {
    client->type = KR_WS_REST_API;
    printk("Web Server: REST DELETE API Client");
    ret = 0;
  }
  kr_io2_pulled(client->in, client->http.header_pos);
  return ret;
}

int handle_http_request(kr_web_client *client) {
  int ret;
  if (!client->http.got_headers) {
    if (find_end_of_request_headers(client)) {
      ret = get_address_method(client);
      if (ret < 0) return -1;
      switch (client->http.method) {
        case KR_HTTP_GET:
          ret = handle_get(client);
          return ret;
        case KR_HTTP_PATCH:
          ret = handle_patch(client);
          return ret;
        case KR_HTTP_DELETE:
          ret = handle_delete(client);
          return ret;
        case KR_HTTP_POST:
          ret = handle_post(client);
          return ret;
        case KR_HTTP_SOURCE: /* Fallin thru */
        case KR_HTTP_PUT:
          ret = handle_put(client);
          return ret;
        default:
          print_headers(client);
          return -1;
      }
    }
  }
  if ((!client->http.got_headers) && (client->in->len >= 4096)) {
    printke("Web Server: Request headers too long %d bytes", client->in->len);
    return -1;
  }
  return 0;
}
