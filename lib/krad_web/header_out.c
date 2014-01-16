static int pack_http_header(kr_web_client *client, char *mimetype);
static int pack_http_404_response(kr_web_client *client);

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
