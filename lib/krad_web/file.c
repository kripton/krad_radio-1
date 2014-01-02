int32_t web_file_client_handle(kr_web_client *client) {
  int32_t len;
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
        web_server_pack_headers(client, "text/javascript");
        web_server_pack_buffer(client, s->api_js, s->api_js_len);
        web_server_pack_buffer(client, s->iface_js, s->iface_js_len);
        break;
      }
      if (strmatch(get, "dev/krad.js")) {
        web_server_pack_headers(client, "text/javascript");
        web_server_pack_buffer(client, s->api_js, s->api_js_len);
        web_server_pack_buffer(client, s->deviface_js, s->deviface_js_len);
        break;
      }
      if ((len == 0) || (strmatch(get, "dev/"))) {
        web_server_pack_headers(client, "text/html");
        web_server_pack_buffer(client, s->html, s->html_len);
        break;
      }
    }
    web_server_pack_404(client);
    break;
  }
  client->drop_after_sync = 1;
  return 0;
}
