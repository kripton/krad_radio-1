static int handle_json(kr_web_client *client, char *json, size_t len) {
  return 0;
}

static int crate_to_json(kr_web_client *client, kr_crate *crate) {
  return 0;
}

static int krad_delivery_handler(kr_web_client *client) {
  kr_crate *crate;
  crate = NULL;
  kr_delivery_recv(client->ws.krclient);
  while ((kr_delivery_get(client->ws.krclient, &crate) > 0)
   && (crate != NULL)) {
      crate_to_json(client, crate);
      kr_crate_recycle(&crate);
      continue;
  }
  return 0;
}
