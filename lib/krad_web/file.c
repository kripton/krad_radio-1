int web_file_client_handle(kr_web_client *client) {
  int len;
  char *get;
  kr_web_server *s;
  s = client->server;
  get = client->get;
  if (get[0] == '/') {
    get = client->get + 1;
  }
  len = strlen(get);
  for (;;) {
    if ((len > -1) && (len < 32)) {
      if (strmatch(get, "krad.js")) {
        pack_http_header(client, "text/javascript");
        kr_io2_pack(client->out, s->api_js, s->api_js_len);
        kr_io2_pack(client->out, s->iface_js, s->iface_js_len);
        break;
      }
      if (strmatch(get, "dev/krad.js")) {
        pack_http_header(client, "text/javascript");
        kr_io2_pack(client->out, s->api_js, s->api_js_len);
        kr_io2_pack(client->out, s->deviface_js, s->deviface_js_len);
        break;
      }
      if ((len == 0) || (strmatch(get, "dev/"))) {
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
