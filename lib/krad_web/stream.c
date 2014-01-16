int web_client_get_stream(kr_web_client *client) {
  if (strncmp(client->get, "/fakestream.bs", 14) == 0) {
    client->type = KR_WS_STREAM_OUT;
    return 1;
  } else {
    printk("Web Server: Checking for stream %s ... nope", client->get);
  }
  return 0;
}

int web_stream_client_handle(kr_web_client *client) {
  size_t fakebytes;
  fakebytes = rand() % 87;
  printk("Web Server: Fake streaming %zu bytes", fakebytes);
  return 0;
}

int web_stream_in_client_handle(kr_web_client *client) {
  printk("Web Server: Fake reading stream in %zu bytes", client->in->len);
  kr_io2_pulled(client->in, client->in->len);
  return 0;
}
