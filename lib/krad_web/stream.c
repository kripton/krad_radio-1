int find_stream(kr_web_client *client) {
  if (strncmp(client->address, "/fakestream.bs", 14) == 0) {
    client->type = KR_WS_GET_STREAM;
    return 1;
  } else {
    printk("Web Server: Checking for stream %s ... nope", client->address);
  }
  return 0;
}

int handle_get_stream(kr_web_client *client) {
  size_t fakebytes;
  fakebytes = rand() % 87;
  printk("Web Server: Fake streaming %zu bytes", fakebytes);
  return 0;
}

int handle_put_stream(kr_web_client *client) {
  printk("Web Server: Fake reading stream in %zu bytes", client->in->len);
  kr_io2_pulled(client->in, client->in->len);
  return 0;
}
