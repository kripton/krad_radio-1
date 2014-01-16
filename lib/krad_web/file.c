int handle_post_file(kr_web_client *client) {

  return 0;
}

int handle_put_file(kr_web_client *client) {

  return 0;
}

int handle_get_file(kr_web_client *client) {
  int len;
  char *address;
  kr_web_server *s;
  s = client->server;
  address = client->address;
  if (address[0] == '/') {
    address = client->address + 1;
  }
  len = strlen(address);
  for (;;) {
    if ((len > -1) && (len < 32)) {
      if (strmatch(address, "krad.js")) {
        pack_http_header(client, "text/javascript");
        kr_io2_pack(client->out, s->api_js, s->api_js_len);
        kr_io2_pack(client->out, s->iface_js, s->iface_js_len);
        break;
      }
      if (strmatch(address, "dev/krad.js")) {
        pack_http_header(client, "text/javascript");
        kr_io2_pack(client->out, s->api_js, s->api_js_len);
        kr_io2_pack(client->out, s->deviface_js, s->deviface_js_len);
        break;
      }
      if ((len == 0) || (strmatch(address, "dev/"))) {
        pack_http_header(client, "text/html");
        kr_io2_pack(client->out, s->html, s->html_len);
        break;
      }
    }
    pack_http_404_response(client);
    break;
  }
  client->drop_after_sync = 1;
  return 0;
}
