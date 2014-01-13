#define WS_MASK_BIT 0x80  // 10000000
#define WS_FIN_FRM 0x80   // 10000000
#define WS_CONT_FRM 0x00  // 00000000
#define WS_TEXT_FRM 0x01  // 00000001
#define WS_BIN_FRM 0x02   // 00000010
#define WS_CLOSE_FRM 0x08 // 00001000
#define WS_PING_FRM 0x09  // 00001001
#define WS_PONG_FRM 0x0a  // 00001010

static int32_t unpack_frame_header(kr_web_client *client) {
  kr_websocket_client *ws;
  uint8_t *size_bytes;
  uint8_t payload_sz_8;
  uint64_t payload_sz_64;
  uint16_t payload_sz_16;
  int32_t bytes_read;
  uint8_t frame_type;
  bytes_read = 0;
  ws = &client->ws;
  ws->input_len = client->in->len;
  ws->input = client->in->rd_buf;
  if (ws->input_len < 6) {
    return 0;
  }
  frame_type = ws->input[0];
  //printk("pframe type = %2X", frame_type);
  if (frame_type & WS_FIN_FRM) {
    //printk ("We have a fin frame!");
    frame_type ^= WS_FIN_FRM;
  }
  //printk("poframe type = %2X", frame_type);
  if (frame_type == WS_PING_FRM) {
    printke("We have a ping frame but we are not dealing with it right!");
  } else {
    if (frame_type == WS_CLOSE_FRM) {
      printk("Websocket close!");
      return -1;
    } else {
      if (frame_type == WS_BIN_FRM) {
        //printk ("We have a bin frame!");
      } else {
        if (frame_type == WS_TEXT_FRM) {
          //printk ("We have a text frame!");
        } else {
          if (frame_type == WS_CONT_FRM) {
            //printk ("We have a CONT frame!");
          } else {
            printke ("Unknown frame type!");
            return -9;
          }
        }
      }
    }
  }
  payload_sz_8 = ws->input[1];
  if (payload_sz_8 & WS_MASK_BIT) {
    payload_sz_8 ^= WS_MASK_BIT;
  } else {
    printke("Mask Bit is NOT set");
    return -4;
  }
  if (payload_sz_8 < 126) {
    //printk("payload size is %u", payload_sz_8);
    ws->mask[0] = ws->input[2];
    ws->mask[1] = ws->input[3];
    ws->mask[2] = ws->input[4];
    ws->mask[3] = ws->input[5];
    ws->len = payload_sz_8;
    bytes_read = 6;
  } else {
    if (ws->input_len < 8) {
      return 0;
    }
    if (payload_sz_8 == 126) {
      size_bytes = (uint8_t *)&payload_sz_16;
      size_bytes[1] = ws->input[2];
      size_bytes[0] = ws->input[3];
      ws->mask[0] = ws->input[4];
      ws->mask[1] = ws->input[5];
      ws->mask[2] = ws->input[6];
      ws->mask[3] = ws->input[7];
      ws->len = payload_sz_16;
      bytes_read = 8;
    } else {
      if (ws->input_len < 14) {
        return 0;
      }
      size_bytes = (uint8_t *)&payload_sz_64;
      size_bytes[7] = ws->input[2];
      size_bytes[6] = ws->input[3];
      size_bytes[5] = ws->input[4];
      size_bytes[4] = ws->input[5];
      size_bytes[3] = ws->input[6];
      size_bytes[2] = ws->input[7];
      size_bytes[1] = ws->input[8];
      size_bytes[0] = ws->input[9];
      ws->mask[0] = ws->input[10];
      ws->mask[1] = ws->input[11];
      ws->mask[2] = ws->input[12];
      ws->mask[3] = ws->input[13];
      ws->len = payload_sz_64;
      bytes_read = 14;
    }
  }
  if (ws->len > 8192 * 6) {
    printke("input ws frame size too big");
    ws->len = 0;
    ws->pos = 0;
    return -10;
  }
  ws->pos = 0;
  ws->frames++;
  //printk("payload size is %"PRIu64"", ws->len);
  kr_io2_pulled(client->in, bytes_read);
  return bytes_read;
}

static int32_t unpack_frame_data(kr_web_client *client) {
  kr_websocket_client *ws;
  int32_t ret;
  int32_t pos;
  int32_t max;
  uint8_t output[4096];
  ws = &client->ws;
  ws->input_len = client->in->len;
  ws->input = client->in->rd_buf;
  if (ws->input_len < ws->len) {
    printk("Incomplete WS frame: %u / %"PRIu64"", ws->input_len,
     ws->len);
    return 0;
  }
  ws->output = output;
  ws->output_len = sizeof(output);
  pos = 0;
  if ((ws->len == 0) || (ws->pos == ws->len) || (ws->input_len == 0) ||
      (ws->output_len == 0)) {
    return 0;
  }
  max = MIN(MIN((ws->len - ws->pos), ws->input_len), ws->output_len);
  //printk ("max is %d", max);
  for (pos = 0; pos < max; pos++) {
    ws->output[pos] = ws->input[ws->pos] ^ ws->mask[ws->pos % 4];
    ws->pos++;
  }
  output[pos] = '\0';
  //printk("unmasked %d bytes %s", pos, (char *)output);
  /* FIXME important bit */
  //ret = handle_json(client, (char *)output, pos);
  ret = -1;


  if (ret != 0) return -1;
  kr_io2_pulled(client->in, pos);
  if (ws->pos == ws->len) {
    ws->len = 0;
    ws->pos = 0;
  }
  return pos;
}

static int32_t pack_accept_response(char *resp, char *key) {
  static char *ws_guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  int32_t ret;
  char string[128];
  uint8_t hash[20];
  if ((resp == NULL) || (key == NULL)) {
    return -1;
  }
  snprintf(string, sizeof(string), "%s%s", key, ws_guid);
  string[127] = '\0';
  kr_sha1((uint8_t *)string, strlen(string), hash);
  ret = kr_base64((uint8_t *)resp, hash, 20, 64);
  return ret;
}

static uint32_t pack_frame_header(uint8_t *out, uint32_t size) {
  uint16_t size_16;
  uint64_t size_64;
  uint8_t *size_bytes;
  out[0] = WS_FIN_FRM | WS_TEXT_FRM;
  if (size < 126) {
    out[1] = size;
    return 2;
  } else {
    if (size < 65536) {
      out[1] = 126;
      size_16 = size;
      size_bytes = (uint8_t *)&size_16;
      out[2] = size_bytes[1];
      out[3] = size_bytes[0];
      return 4;
    } else {
      out[1] = 127;
      size_64 = size;
      size_bytes = (uint8_t *)&size_64;
      out[2] = size_bytes[7];
      out[3] = size_bytes[6];
      out[4] = size_bytes[5];
      out[5] = size_bytes[4];
      out[6] = size_bytes[3];
      out[7] = size_bytes[2];
      out[8] = size_bytes[1];
      out[9] = size_bytes[0];
      return 10;
    }
  }
}

int websocket_pack(kr_io2_t *out, uint8_t *buffer, size_t len) {
  uint32_t header_len;
  uint8_t header[10];
  if (len > 0) {
    header_len = pack_frame_header(header, len);
    if (out->space >= (header_len + len)) {
      kr_io2_pack(out, header, header_len);
      kr_io2_pack(out, buffer, len);
      return 0;
    } else {
      printke("Websocket client: No space to pack buffer");
    }
  }
  return -1;
}

int32_t websocket_unpack(kr_web_client *client) {
  int ret;
    for (;;) {
      if (client->ws.len == 0) {
        ret = unpack_frame_header(client);
        if (ret < 0) {
          break;
        }
      }
      if (client->ws.len > 0) {
        ret = unpack_frame_data(client);
        if (ret <= 0) {
          break;
        }
      } else {
        ret = 0;
        break;
      }
    }
  return ret;
}

int32_t app_client_event(kr_web_client *client) {
  kr_web_server *server;
  kr_web_event event;
  server = client->server;
  event.type = KR_WEB_CLIENT_CREATE;
  event.fd = client->sd;
  event.output_cb = websocket_pack;
  //event.input_cb = websocket_unpack;
  event.user = server->user;
  server->event_cb(&event);
  return 0;
}

int32_t websocket_client_handle(kr_web_client *client) {
  int32_t pos;
  char *buffer;
  char acceptkey[64];
  pos = 0;
  buffer = (char *)client->out->buf;
  memset(acceptkey, 0, sizeof(acceptkey));
  pack_accept_response(acceptkey, client->ws.key);
  pos += sprintf(buffer + pos, "HTTP/1.1 101 Switching Protocols\r\n");
  pos += sprintf(buffer + pos, "Upgrade: websocket\r\n");
  pos += sprintf(buffer + pos, "Connection: Upgrade\r\n");
  pos += sprintf(buffer + pos, "Sec-WebSocket-Protocol: krad-ws-api\r\n");
  pos += sprintf(buffer + pos, "Sec-WebSocket-Accept: %s\r\n", acceptkey);
  pos += sprintf(buffer + pos, "\r\n");
  kr_io2_advance(client->out, pos);
  client->ws.shaked = 1;
  set_socket_nodelay(client->sd);
  app_client_event(client);
  //printk("interweb_ws_shake happens");
  return 0;
}
