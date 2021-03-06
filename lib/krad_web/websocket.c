#define WS_MASK_BIT 0x80  // 10000000
#define WS_FIN_FRM 0x80   // 10000000
#define WS_CONT_FRM 0x00  // 00000000
#define WS_TEXT_FRM 0x01  // 00000001
#define WS_BIN_FRM 0x02   // 00000010
#define WS_CLOSE_FRM 0x08 // 00001000
#define WS_PING_FRM 0x09  // 00001001
#define WS_PONG_FRM 0x0a  // 00001010

ssize_t unpack_frame_header(kr_websocket_tracker *ws, void *out, size_t max, void *in, size_t len) {
  uint8_t *size_bytes;
  uint8_t payload_sz_8;
  uint64_t payload_sz_64;
  uint16_t payload_sz_16;
  int bytes_read;
  uint8_t frame_type;
  bytes_read = 0;
  ws->input = in;
  ws->input_len = len;
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
  printk("payload size is %"PRIu64"", ws->len);
  //kr_io2_pulled(client->in, bytes_read);
  return bytes_read;
}

ssize_t unpack_frame_data(kr_websocket_tracker *ws, void *out, size_t maxout, void *in, size_t len) {
  int pos;
  int max;
  ws->input = in;
  ws->input_len = len;
  if (ws->input_len < ws->len) {
    printk("Incomplete WS frame: %u / %"PRIu64"", ws->input_len, ws->len);
    return 0;
  }
  ws->output = out;
  ws->output_len = maxout;
  pos = 0;
  if ((ws->len == 0) || (ws->pos == ws->len) || (ws->input_len == 0) ||
      (ws->output_len == 0)) {
    return 0;
  }
  max = MIN(MIN((ws->len - ws->pos), ws->input_len), ws->output_len);
  for (pos = 0; pos < max; pos++) {
    ws->output[pos] = ws->input[ws->pos] ^ ws->mask[ws->pos % 4];
    ws->pos++;
  }
  ws->output[pos] = '\0'; /* FIXME this could be overflow pos */
  //printk("unmasked %d bytes %s", pos, (char *)output);
  if (ws->pos == ws->len) {
    ws->len = 0;
    ws->pos = 0;
  }
  return pos;
}

ssize_t websocket_unpack(void *ctx, void *out, size_t max, void *in, size_t len) {
  kr_websocket_tracker *ws;
  int ret;
  int bytes_read;
  if (len < 1) return -1;
  if (max < len) return -2;
  if (ctx == NULL) return -3;
  if (out == NULL) return -4;
  if (in == NULL) return -5;
  bytes_read = 0;
  ws = (kr_websocket_tracker *)ctx;
  if (ws->len == 0) {
    ret = unpack_frame_header(ws, out, max, in, len);
    if (ret < 1) {
      return ret;
    } else {
      printk("frame header %d", ret);
      bytes_read += ret;
      in += ret;
      len -= ret;
    }
  }
  if (ws->len > 0) {
    ret = unpack_frame_data(ws, out, max, in, len);
    if (ret > 0) {
      printk("frame data %d", ret);
      bytes_read += ret;
    }
  }
  return bytes_read;
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

ssize_t websocket_pack(void *ctx, void *out, size_t max, void *in, size_t len) {
  uint32_t header_len;
  uint32_t total_len;
  uint8_t header[10];
  if (len < 1) return -1;
  if (max < len) return -2;
  if (ctx == NULL) return -3;
  if (out == NULL) return -4;
  if (in == NULL) return -5;
  header_len = pack_frame_header(header, len);
  total_len = header_len + len;
  if (max < total_len) {
    printke("Websocket client: No space to pack buffer");
    return -6;
  }
  memcpy(out, header, header_len);
  memcpy(out + header_len, in, len);
  return total_len;
}

static int build_accept_key(char *resp, char *key) {
  static char *ws_guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  int ret;
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

int websocket_app_client(kr_web_client *client) {
  kr_web_server *server;
  kr_web_event event;
  server = client->server;
  event.type = KR_WEB_CLIENT_CREATE;
  event.fd = client->sd;
  event.in = client->in;
  event.out = client->out;
  event.state_tracker = &client->ws;
  event.state_tracker_sz = sizeof(kr_websocket_tracker);
  event.output_cb = websocket_pack;
  event.input_cb = websocket_unpack;
  event.user = server->user;
  server->event_cb(&event);
  return 0;
}

int handle_websocket(kr_web_client *client) {
  int pos;
  char *buffer;
  char clientkey[64];
  char acceptkey[64];
  char *headers;
  int ret;
  pos = 0;
  headers = (char *)client->in->rd_buf;
  buffer = (char *)client->out->buf;
  memset(acceptkey, 0, sizeof(acceptkey));
  memset(clientkey, 0, sizeof(clientkey));
  ret = copy_header(clientkey, headers, "Sec-WebSocket-Key: ", sizeof(clientkey));
  if (ret != 0) {
    printke("Web Server: Error getting websocket key header");
    return -1;
  }
  build_accept_key(acceptkey, clientkey);
  kr_io2_pulled(client->in, client->http.header_pos + 1);
  pos += sprintf(buffer + pos, "HTTP/1.1 101 Switching Protocols\r\n");
  pos += sprintf(buffer + pos, "Upgrade: websocket\r\n");
  pos += sprintf(buffer + pos, "Connection: Upgrade\r\n");
  pos += sprintf(buffer + pos, "Sec-WebSocket-Protocol: krad-ws-api\r\n");
  pos += sprintf(buffer + pos, "Sec-WebSocket-Accept: %s\r\n", acceptkey);
  pos += sprintf(buffer + pos, "\r\n");
  kr_io2_advance(client->out, pos);
  set_socket_nodelay(client->sd);
  websocket_app_client(client);
  client->sd = -1;
  return -1;
}

int websocket_headers_detected(char *headers) {
  if (strstr(headers, "Upgrade: websocket") != NULL) {
    return 1;
  }
  return 0;
}
