#include "krad_transponder_graph.h"

static char *transponder_subunit_type_to_string (kr_xpdr_subunit_type_t type);
static void kr_xpdr_port_read (kr_xpdr_input_t *inport, void *msg);
static void kr_xpdr_port_write (kr_xpdr_input_t *input, void *msg);
static void kr_xpdr_port_disconnect (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                     kr_xpdr_output_t *output,
                                     kr_xpdr_input_t *input);
static void kr_xpdr_port_connect (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                  kr_xpdr_subunit_t *from_kr_xpdr_subunit,
                                  kr_xpdr_output_t *output,
                                  kr_xpdr_input_t *input);
static void kr_xpdr_input_disconnect (kr_xpdr_input_t *kr_xpdr_input);
static void kr_xpdr_input_destroy (kr_xpdr_input_t *kr_xpdr_input);
static void kr_xpdr_output_free (kr_xpdr_output_t *kr_xpdr_output);
static void kr_xpdr_output_destroy (kr_xpdr_output_t *kr_xpdr_output);
static kr_xpdr_output_t *kr_xpdr_output_create ();
static void kr_xpdr_subunit_start (kr_xpdr_subunit_t *kr_xpdr_subunit);
static int kr_xpdr_output_set_header (kr_xpdr_output_t *outport,
                                      krad_codec_header_t *krad_codec_header);
static int kr_xpdr_subunit_add (kr_xpdr_t *kr_xpdr,
                                kr_xpdr_subunit_type_t type,
                                kr_xpdr_watch_t *watch);
static kr_xpdr_subunit_t *xpdr_subunit_create (kr_xpdr_t *xpdr,
                                               kr_xpdr_subunit_type_t type,
                                               kr_xpdr_watch_t *watch);
static void kr_xpdr_subunit_destroy (kr_xpdr_subunit_t **kr_xpdr_subunit);
static void kr_xpdr_subunit_stop (kr_xpdr_subunit_t *kr_xpdr_subunit);
static void kr_xpdr_subunit_send_destroy_msg (kr_xpdr_subunit_t *kr_xpdr_subunit);
static void *kr_xpdr_subunit_thread (void *arg);
static int kr_xpdr_subunit_poll (kr_xpdr_subunit_t *kr_xpdr_subunit);
static void kr_xpdr_subunit_handle_control_msg (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                                kr_xpdr_control_msg_t *msg);
static void kr_xpdr_subunit_disconnect_ports_actual (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                                     kr_xpdr_output_t *output,
                                                     kr_xpdr_input_t *input);
static void kr_xpdr_subunit_connect_ports_actual (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                                  kr_xpdr_output_t *output,
                                                  kr_xpdr_input_t *input);

static char *transponder_subunit_type_to_string (kr_xpdr_subunit_type_t type) {
  switch (type) {
    case DEMUXER:
      return "Demuxer";
    case MUXER:
      return "Muxer";
    case DECODER:
      return "Decoder";
    case ENCODER:
      return "Encoder";
    case RAW:
      return "RAW";
  }
  return "Unknown Subunit";
}

static void kr_xpdr_port_read (kr_xpdr_input_t *inport, void *msg) {

  int ret;
  char buffer[1];

  ret = read (inport->socketpair[1], buffer, 1);
  if (ret != 1) {
    if (ret == 0) {
      printk ("Krad XPDR: port read got EOF");
      return;
    }
    printk ("Krad XPDR: port read unexpected read return value %d", ret);
  }

  if (msg != NULL) {
    //printk ("Krad Transponder: msg read space %zu",
    // krad_ringbuffer_read_space (inport->msg_ring));  
    ret = krad_ringbuffer_read (inport->msg_ring, (char *)msg, sizeof(void *));
    if (ret != sizeof(void *)) {
      printk ("Krad XPDR: invalid msg read len %d", ret);
    }    
  } else {
    printk ("Krad XPDR: uh oh nullzor!");
  }
  //printk ("Krad Transponder: input port read");

}

static void kr_xpdr_port_write (kr_xpdr_input_t *input, void *msg) {

  int wrote;

  if (input != NULL) {
    krad_ringbuffer_write (input->msg_ring, (char *)msg, sizeof(void *));
    wrote = write (input->socketpair[0], "0", 1);
    if (wrote != 1) {
      printk ("Krad XPDR: port write unexpected write return value %d", wrote);
    }
  }

  //printk ("Krad Transponder: port write");

}

static void kr_xpdr_port_disconnect (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                     kr_xpdr_output_t *output,
                                     kr_xpdr_input_t *input) {

  printk ("Krad Transponder: sending disconnect ports msg");

  kr_xpdr_control_msg_t *msg;
  msg = calloc (1, sizeof (kr_xpdr_control_msg_t));
  msg->type = DISCONNECTPORTS;
  msg->input = input;
  msg->output = output;
  kr_xpdr_port_write (kr_xpdr_subunit->control, &msg);

  printk ("Krad Transponder: sent disconnect ports msg");
}

static void kr_xpdr_port_connect (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                  kr_xpdr_subunit_t *from_kr_xpdr_subunit,
                                  kr_xpdr_output_t *output,
                                  kr_xpdr_input_t *input) {


  printk ("Krad Transponder: sending connecting ports msg");

  kr_xpdr_control_msg_t *msg;
  msg = calloc (1, sizeof (kr_xpdr_control_msg_t));
  msg->type = CONNECTPORTS;
  msg->input = input;
  msg->output = output;
  
  input->connected_to_subunit = kr_xpdr_subunit;
  input->subunit = from_kr_xpdr_subunit;
  kr_xpdr_port_write (kr_xpdr_subunit->control, &msg);

  printk ("Krad Transponder: sent connecting ports msg");
}

static void kr_xpdr_input_disconnect (kr_xpdr_input_t *kr_xpdr_input) {
  printk ("Krad Transponder: disconnecting ports");
  //close (kr_xpdr_input->socketpair[0]);
  close (kr_xpdr_input->socketpair[1]);
}

static void kr_xpdr_input_free (kr_xpdr_input_t *kr_xpdr_input) {
  krad_ringbuffer_free ( kr_xpdr_input->msg_ring );
  free (kr_xpdr_input);
}

static void kr_xpdr_input_destroy (kr_xpdr_input_t *kr_xpdr_input) {
  kr_xpdr_input_disconnect (kr_xpdr_input);
  kr_xpdr_input_free (kr_xpdr_input);
  printk ("Krad Transponder: input port destroyed");
}

static void kr_xpdr_output_free (kr_xpdr_output_t *kr_xpdr_output) {
  int h;
  
  for (h = 0; h < kr_xpdr_output->headers; h++) {
    kr_slice_unref (kr_xpdr_output->kr_slice[h]);
  }
  free (kr_xpdr_output->connections);
  free (kr_xpdr_output);
}

static void kr_xpdr_output_destroy (kr_xpdr_output_t *kr_xpdr_output) {

  //kr_xpdr_port_disconnect (kr_xpdr_port);
  kr_xpdr_output_free (kr_xpdr_output);
  printk ("Krad XPDR: output port destroyed");
}

kr_xpdr_input_t *kr_xpdr_input_create () {

  kr_xpdr_input_t *kr_xpdr_input = calloc (1, sizeof(kr_xpdr_input_t));

  if (socketpair(AF_UNIX, SOCK_STREAM, 0, kr_xpdr_input->socketpair)) {
    printk ("Krad XPDR: subunit could not create socketpair errno: %d", errno);
    free (kr_xpdr_input);
    return NULL;
  }

  kr_xpdr_input->msg_ring =
  krad_ringbuffer_create ( 200 * sizeof(kr_slice_t *) );

  printk ("Krad XPDR: input port created");

  return kr_xpdr_input;
}

static kr_xpdr_output_t *kr_xpdr_output_create () {
  kr_xpdr_output_t *kr_xpdr_output = calloc (1, sizeof(kr_xpdr_output_t));
  kr_xpdr_output->connections = calloc (KRAD_TRANSPONDER_PORT_CONNECTIONS,
                                        sizeof(kr_xpdr_input_t *));
  printk ("Krad Transponder: output port created");
  return kr_xpdr_output;
}

static void kr_xpdr_subunit_connect_ports_actual (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                           kr_xpdr_output_t *output,
                                           kr_xpdr_input_t *input) {

  int p;

  for (p = 0; p < KRAD_TRANSPONDER_PORT_CONNECTIONS; p++) {
    if (output->connections[p] == NULL) {
      output->connections[p] = input;
      printk ("Krad Transponder: output port actually connected!");
      break;
    }
  }
}

static void kr_xpdr_subunit_disconnect_ports_actual (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                              kr_xpdr_output_t *output,
                                              kr_xpdr_input_t *input) {

  int p;

  for (p = 0; p < KRAD_TRANSPONDER_PORT_CONNECTIONS; p++) {
    if (output->connections[p] == input) {
      output->connections[p] = NULL;
      
      kr_slice_t *kr_slice;
      kr_slice = kr_slice_create ();
      kr_slice->final = 1;
      //kr_slice->origin = kr_xpdr_subunit;
      kr_xpdr_port_write (input, &kr_slice);
      
      close (input->socketpair[0]);
      printk ("Krad Transponder: output port actually disconnected!");
      break;
    }
  }
}

static void kr_xpdr_subunit_handle_control_msg (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                             kr_xpdr_control_msg_t *msg) {
  if (msg->type == CONNECTPORTS) {
    printk("Krad Transponder Subunit: got CONNECTPORTS msg!");
    kr_xpdr_subunit_connect_ports_actual (kr_xpdr_subunit, msg->output, msg->input);
    free (msg);
    return;
  }
  if (msg->type == DISCONNECTPORTS) {
    printk("Krad Transponder Subunit: got DISCONNECTPORTS msg!");
    kr_xpdr_subunit_disconnect_ports_actual (kr_xpdr_subunit, msg->output, msg->input);    
    free (msg);
    return;
  }
  if (msg->type == UPDATE) {
    printk("Krad Transponder Subunit: got UPDATE msg!");
    free (msg);
    return;
  }

  printk("Krad Transponder Subunit: got some other message!");
  
}

static int kr_xpdr_subunit_poll (kr_xpdr_subunit_t *kr_xpdr_subunit) {

  int n;
  int nfds;
  int ret;
  int timeout;
  struct pollfd pollfds[4];
  
  timeout = -1;
  n = 0;

  pollfds[n].fd = kr_xpdr_subunit->control->socketpair[1];
  pollfds[n].events = POLLIN;
  n++;
  
  if (kr_xpdr_subunit->type == MUXER) {
    //if (kr_xpdr_subunit->inputs[0]->connected_to_subunit != NULL) {
      pollfds[n].fd = kr_xpdr_subunit->inputs[0]->socketpair[1];
      pollfds[n].events = POLLIN;
      n++;
    //}
    //if (kr_xpdr_subunit->inputs[1]->connected_to_subunit != NULL) {
      pollfds[n].fd = kr_xpdr_subunit->inputs[1]->socketpair[1];
      pollfds[n].events = POLLIN;
      n++;
    //}
  }
  
  if (kr_xpdr_subunit->type == DECODER) {
    if (kr_xpdr_subunit->inputs[0]->connected_to_subunit != NULL) {
      pollfds[n].fd = kr_xpdr_subunit->inputs[0]->socketpair[1];
      pollfds[n].events = POLLIN;
      n++;
    }
  }
  
  if (kr_xpdr_subunit->watch != NULL) {
    if (kr_xpdr_subunit->watch->fd > 0) {
      pollfds[n].fd = kr_xpdr_subunit->watch->fd;
      pollfds[n].events = POLLIN;
      n++;
    } else {
      if (kr_xpdr_subunit->watch->idle_callback_interval > 0) {
        timeout = kr_xpdr_subunit->watch->idle_callback_interval;
      }
    }
  }
  

  nfds = n;

  ret = poll (pollfds, nfds, timeout);
  if (ret < 0) {
    // error
    return 0;
  }
  
  if (ret == 0) {
    if (kr_xpdr_subunit->watch->idle_callback_interval > 0) {
      kr_xpdr_subunit->watch->readable_callback (kr_xpdr_subunit->watch->callback_pointer);
    }
    return 1;
  }  

  if (ret > 0) {
    for (n = 0; n < nfds; n++) {
      if (pollfds[n].revents) {
        if (pollfds[n].revents & POLLERR) {
          printk("Krad Transponder Subunit: Err we got Err in Hrr!");
          if (pollfds[n].fd == kr_xpdr_subunit->control->socketpair[1]) {
            printk("Krad Transponder Subunit: Err on control socket!");
          }
          return 0;
        }

        if (pollfds[n].revents & POLLIN) {
        
          if ((kr_xpdr_subunit->watch != NULL) && 
              (kr_xpdr_subunit->watch->fd > 0) &&
              (pollfds[n].fd == kr_xpdr_subunit->watch->fd)) {
            kr_xpdr_subunit->watch->readable_callback (kr_xpdr_subunit->watch->callback_pointer);          
          }

          if (pollfds[n].fd == kr_xpdr_subunit->control->socketpair[1]) {
            kr_xpdr_control_msg_t *msg;
            msg = NULL;
            kr_xpdr_port_read (kr_xpdr_subunit->control, (void **)&msg);
            if (msg->type == DESTROY) {
              printk("Krad Transponder: Subunit Got Destroy MSG!");
              free (msg);
              kr_xpdr_subunit->destroy = 1;
              if ((kr_xpdr_subunit->type == MUXER) || (kr_xpdr_subunit->type == DECODER)) {
                if (kr_xpdr_subunit->inputs[0]->connected_to_subunit != NULL) {
                  kr_xpdr_subunit->destroy++;
                  kr_xpdr_port_disconnect (kr_xpdr_subunit->inputs[0]->connected_to_subunit,
                                                    kr_xpdr_subunit->inputs[0]->connected_to_subunit->outputs[0],
                                                    kr_xpdr_subunit->inputs[0]);
                }
                if (kr_xpdr_subunit->type == MUXER) {
                  if (kr_xpdr_subunit->inputs[1]->connected_to_subunit != NULL) {
                    kr_xpdr_subunit->destroy++;
                    kr_xpdr_port_disconnect (kr_xpdr_subunit->inputs[1]->connected_to_subunit,
                                             kr_xpdr_subunit->inputs[1]->connected_to_subunit->outputs[1],
                                             kr_xpdr_subunit->inputs[1]);
                  }
                }
              }
              if ((kr_xpdr_subunit->type == ENCODER) ||
                  (kr_xpdr_subunit->type == DEMUXER) ||
                  (kr_xpdr_subunit->type == RAW)) {
                return 0;
              }

            } else {
              kr_xpdr_subunit_handle_control_msg (kr_xpdr_subunit, msg);
            }
          }

          if (kr_xpdr_subunit->type == MUXER) {
            kr_slice_t *kr_slice;
            kr_slice = NULL;
            if (pollfds[n].fd == kr_xpdr_subunit->inputs[0]->socketpair[1]) {
              kr_xpdr_port_read (kr_xpdr_subunit->inputs[0], &kr_slice);
            }
            if (pollfds[n].fd == kr_xpdr_subunit->inputs[1]->socketpair[1]) {
              kr_xpdr_port_read (kr_xpdr_subunit->inputs[1], (void **)&kr_slice);
            }
            if (kr_slice != NULL) {
              //printk ("Krad Transponder Subunit: Got a packet!");
              
              if (kr_slice->final == 1) {
                printk ("Krad XPDR Subunit: Got FINAL packet!");
              } else {
                //printk ("Krad XPDR Subunit: packet size %u", kr_slice->size);
              }
              
              kr_xpdr_subunit->kr_slice = kr_slice;
              
              kr_xpdr_subunit->watch->readable_callback (kr_xpdr_subunit->watch->callback_pointer);
              
              kr_xpdr_subunit->kr_slice = NULL;
              //kr_slice_unref (kr_slice);
            }
          }
        }

        if (pollfds[n].revents & POLLOUT) {
          // out on something
        }
        
        if (pollfds[n].revents & POLLHUP) {
          printk ("Krad Transponder Subunit: Got Hangup");
          if (pollfds[n].fd == kr_xpdr_subunit->control->socketpair[1]) {
            printk("Err! Hangup on control socket!");
            return 0;
          }
          if (kr_xpdr_subunit->type == MUXER) {
            if (pollfds[n].fd == kr_xpdr_subunit->inputs[0]->socketpair[1]) {
              printk ("Krad XPDR Subunit: Encoded Video Input Disconnected");
              kr_xpdr_subunit->inputs[0]->connected_to_subunit = NULL;
              if (kr_xpdr_subunit->destroy > 1) {
                kr_xpdr_subunit->destroy--;
              }
            }
            if (pollfds[n].fd == kr_xpdr_subunit->inputs[1]->socketpair[1]) {
              printk ("Krad XPDR Subunit: Encoded Audio Input Disconnected");
              kr_xpdr_subunit->inputs[1]->connected_to_subunit = NULL;
              if (kr_xpdr_subunit->destroy > 1) {
                kr_xpdr_subunit->destroy--;
              }
            }
          }
        }
      }
    }
  }
  return 1;
}

static void *kr_xpdr_subunit_thread (void *arg) {

  kr_xpdr_subunit_t *kr_xpdr_subunit = (kr_xpdr_subunit_t *)arg;

  krad_system_set_thread_name ("kr_txp_su");

  while (kr_xpdr_subunit_poll (kr_xpdr_subunit)) {
    if (kr_xpdr_subunit->destroy == 1) {
      break;
    }
  }
  return NULL;
}

static void kr_xpdr_subunit_send_destroy_msg (kr_xpdr_subunit_t *kr_xpdr_subunit) {
  kr_xpdr_control_msg_t *msg;
  msg = calloc (1, sizeof (kr_xpdr_control_msg_t));
  msg->type = DESTROY;
  kr_xpdr_port_write (kr_xpdr_subunit->control, &msg);
}

static void kr_xpdr_subunit_start (kr_xpdr_subunit_t *kr_xpdr_subunit) {
  kr_xpdr_subunit->control = kr_xpdr_input_create ();
  pthread_create (&kr_xpdr_subunit->thread, NULL, kr_xpdr_subunit_thread, (void *)kr_xpdr_subunit);
}

static void kr_xpdr_subunit_stop (kr_xpdr_subunit_t *kr_xpdr_subunit) {

  int p;
  int m;

  // Recursive Dep Destroy

  if ((kr_xpdr_subunit->type == ENCODER) || (kr_xpdr_subunit->type == DEMUXER)) {
    for (p = 0; p < KRAD_TRANSPONDER_PORT_CONNECTIONS; p++) {
      if (kr_xpdr_subunit->outputs[0] != NULL) {
        if (kr_xpdr_subunit->outputs[0]->connections[p] != NULL) {
          for (m = 0; m < KRAD_TRANSPONDER_SUBUNITS; m++) {
            if (kr_xpdr_subunit->xpdr->subunits[m] == kr_xpdr_subunit->outputs[0]->connections[p]->subunit) {
              kr_xpdr_subunit_destroy (&kr_xpdr_subunit->xpdr->subunits[m]);
              break;
            }
          }
        }
      }
      if (kr_xpdr_subunit->outputs[1] != NULL) {
        if (kr_xpdr_subunit->outputs[1]->connections[p] != NULL) {
          for (m = 0; m < KRAD_TRANSPONDER_SUBUNITS; m++) {
            if (kr_xpdr_subunit->xpdr->subunits[m] == kr_xpdr_subunit->outputs[1]->connections[p]->subunit) {
              kr_xpdr_subunit_destroy (&kr_xpdr_subunit->xpdr->subunits[m]);
              break;
            }
          }
        }
      }
    }
  }

  kr_xpdr_subunit_send_destroy_msg (kr_xpdr_subunit);
  pthread_join (kr_xpdr_subunit->thread, NULL);
  kr_xpdr_input_destroy (kr_xpdr_subunit->control);  
}

static void kr_xpdr_subunit_destroy (kr_xpdr_subunit_t **kr_xpdr_subunit) {

  int p;
  
  if (*kr_xpdr_subunit != NULL) {
  
    kr_xpdr_subunit_stop (*kr_xpdr_subunit);
    
    // maybe this happens elsewere
    for (p = 0; p < 2; p++) {
      if ((*kr_xpdr_subunit)->inputs[p] != NULL) {
        kr_xpdr_input_destroy ((*kr_xpdr_subunit)->inputs[p]);
      }
      if ((*kr_xpdr_subunit)->outputs[p] != NULL) {
        kr_xpdr_output_destroy ((*kr_xpdr_subunit)->outputs[p]);
      }
    }
    
    if ((*kr_xpdr_subunit)->watch->destroy_callback != NULL) {
      (*kr_xpdr_subunit)->watch->destroy_callback ((*kr_xpdr_subunit)->watch->callback_pointer);
    }
    
    printk ("Krad Transponder: %s subunit destroyed",
            transponder_subunit_type_to_string((*kr_xpdr_subunit)->type));

    free ((*kr_xpdr_subunit)->inputs);
    free ((*kr_xpdr_subunit)->outputs);
    if ((*kr_xpdr_subunit)->watch != NULL) {
      free ((*kr_xpdr_subunit)->watch);
    }
    
    free (*kr_xpdr_subunit);
    *kr_xpdr_subunit = NULL;
  }
}

static kr_xpdr_subunit_t *xpdr_subunit_create (kr_xpdr_t *xpdr,
                                               kr_xpdr_subunit_type_t type,
                                               kr_xpdr_watch_t *watch) {

  int p;
  kr_xpdr_subunit_t *kr_xpdr_subunit = calloc (1, sizeof (kr_xpdr_subunit_t));
  kr_xpdr_subunit->inputs = calloc (2, sizeof (kr_xpdr_input_t *));
  kr_xpdr_subunit->outputs = calloc (2, sizeof (kr_xpdr_output_t *));
  kr_xpdr_subunit->xpdr = xpdr;
  kr_xpdr_subunit->type = type;

  printk ("Krad Transponder: creating %s subunit",
          transponder_subunit_type_to_string(kr_xpdr_subunit->type));

  switch (type) {
    case DEMUXER:
      for (p = 0; p < 2; p++) {
        kr_xpdr_subunit->outputs[p] = kr_xpdr_output_create ();
      }
      break;
    case MUXER:
      for (p = 0; p < 2; p++) {
        kr_xpdr_subunit->inputs[p] = kr_xpdr_input_create ();
        //kr_xpdr_subunit->outputs[p] = kr_xpdr_port_create ();    
      }
      break;
    case DECODER:
      kr_xpdr_subunit->inputs[0] = kr_xpdr_input_create ();
      break;      
    case ENCODER:
      kr_xpdr_subunit->outputs[0] = kr_xpdr_output_create ();
      break;
    case RAW:
      break;
  }
  
  if (watch != NULL) {
    kr_xpdr_subunit->watch = calloc(1, sizeof(kr_xpdr_watch_t));
    memcpy (kr_xpdr_subunit->watch, watch, sizeof(kr_xpdr_watch_t));        
  }
  
  if ((kr_xpdr_subunit->type == ENCODER) &&
      (kr_xpdr_subunit->watch->encoder_header_callback != NULL)) {
    kr_xpdr_set_header (kr_xpdr_subunit,
          kr_xpdr_subunit->watch->encoder_header_callback (kr_xpdr_subunit->watch->callback_pointer));
  }

  kr_xpdr_subunit_start (kr_xpdr_subunit);
  printk ("Krad Transponder: %s subunit created",
          transponder_subunit_type_to_string(kr_xpdr_subunit->type));

  return kr_xpdr_subunit;
}

static int kr_xpdr_subunit_add (kr_xpdr_t *kr_xpdr,
                                kr_xpdr_subunit_type_t type,
                                kr_xpdr_watch_t *watch) {

  int m;

  for (m = 0; m < KRAD_TRANSPONDER_SUBUNITS; m++) {
    if (kr_xpdr->subunits[m] == NULL) {
      kr_xpdr->subunits[m] = xpdr_subunit_create (kr_xpdr, type, watch);
      return m;
    }
  }
  return -1;
}

static int
kr_xpdr_output_set_header (kr_xpdr_output_t *outport,
                           krad_codec_header_t *krad_codec_header) {

  int h;
  
  h = 0;
  if ((krad_codec_header != NULL) && (outport->headers == 0)) { 
    for (h = 0; h < MIN(4, krad_codec_header->header_count); h++) {
      outport->kr_slice[h] =
        kr_slice_create_with_data (krad_codec_header->header[h],
                                   krad_codec_header->header_size[h]);
      outport->kr_slice[h]->header = h + 1;
      outport->headers++;
    }
    outport->krad_codec_header = krad_codec_header;
    return outport->headers;
  }
  return -1;
}

/* Public API */

kr_slice_t *
kr_xpdr_get_slice (kr_xpdr_subunit_t *kr_xpdr_subunit) {
  return kr_xpdr_subunit->kr_slice;
}

krad_codec_header_t *
kr_xpdr_get_header (kr_xpdr_subunit_t *kr_xpdr_subunit) {
  return kr_xpdr_get_subunit_output_header (kr_xpdr_subunit, 0);
}

krad_codec_header_t *
kr_xpdr_get_audio_header (kr_xpdr_subunit_t *kr_xpdr_subunit) {
  return kr_xpdr_get_subunit_output_header (kr_xpdr_subunit, 1);
}

krad_codec_header_t *
kr_xpdr_get_subunit_output_header (kr_xpdr_subunit_t *kr_xpdr_subunit,
                                   int port) {
  if (kr_xpdr_subunit != NULL) {
    if (kr_xpdr_subunit->type == ENCODER) {
      return kr_xpdr_subunit->outputs[0]->krad_codec_header;
    }
    if (kr_xpdr_subunit->type == DEMUXER) {
      if (kr_xpdr_subunit->outputs[port] != NULL) {
        return kr_xpdr_subunit->outputs[port]->krad_codec_header;
      }
    }
  }
  return NULL;
}

int kr_xpdr_set_header (kr_xpdr_subunit_t *kr_xpdr_subunit,
                        krad_codec_header_t *krad_codec_header) {

  if (kr_xpdr_subunit->type == ENCODER) {
    return kr_xpdr_output_set_header (kr_xpdr_subunit->outputs[0],
                                                     krad_codec_header);
  }

  if (kr_xpdr_subunit->type == DEMUXER) {
    if (krad_codec_header->codec == THEORA) {
      return kr_xpdr_output_set_header (kr_xpdr_subunit->outputs[0],
                                                       krad_codec_header);
    }
    if ((krad_codec_header->codec == VORBIS) ||
        (krad_codec_header->codec == FLAC) ||
        (krad_codec_header->codec == OPUS)) {
      return kr_xpdr_output_set_header (kr_xpdr_subunit->outputs[1],
                                                       krad_codec_header);
    }
  }
  return -1;
}

int kr_xpdr_slice_broadcast (kr_xpdr_subunit_t *kr_xpdr_subunit,
                             kr_slice_t **kr_slice) {

  int p;
  int b;
  int port;
  
  b = 0;
  
  port = 0;
  
  //(*kr_slice)->origin = kr_xpdr_subunit;
  
  
  if ((kr_xpdr_subunit->type == DEMUXER) &&
      (((*kr_slice)->codec == FLAC) || ((*kr_slice)->codec == OPUS) || ((*kr_slice)->codec == VORBIS))) {
    port = 1;
  }
  
  //printk ("Krad Transponder: output port broadcasting");  
  
  for (p = 0; p < KRAD_TRANSPONDER_PORT_CONNECTIONS; p++) {
    if (kr_xpdr_subunit->outputs[port]->connections[p] != NULL) {
      kr_slice_ref (*kr_slice);
      kr_xpdr_port_write (kr_xpdr_subunit->outputs[port]->connections[p], kr_slice);
      b++;
    }
  }

  //printk ("Krad Transponder: output port broadcasted to %d", b);

  return p;
}

void kr_xpdr_subunit_remove (kr_xpdr_t *kr_xpdr, int sid) {
  if ((sid > -1) && (sid < KRAD_TRANSPONDER_SUBUNITS)) {
    if (kr_xpdr->subunits[sid] != NULL) {
      printk ("Krad Transponder: removing subunit %d", sid);
      kr_xpdr_subunit_destroy (&kr_xpdr->subunits[sid]);    
    } else {
      printke ("Krad Transponder: can't remove subunit %d, not found", sid);
    }
  }
}

kr_xpdr_subunit_t *kr_xpdr_get_subunit (kr_xpdr_t *kr_xpdr, int sid) {
  if ((sid > -1) && (sid < KRAD_TRANSPONDER_SUBUNITS)) {
    if (kr_xpdr->subunits[sid] != NULL) {
      printk ("Krad Transponder: found subunit %d", sid);
      return kr_xpdr->subunits[sid];
    } else {
      printke ("Krad Transponder: can't find subunit %d", sid);
    }
  }
  return NULL;
}

int kr_xpdr_add_raw (kr_xpdr_t *kr_xpdr, kr_xpdr_watch_t *watch) {
  return kr_xpdr_subunit_add (kr_xpdr, RAW, watch);
}

int kr_xpdr_add_decoder (kr_xpdr_t *kr_xpdr, kr_xpdr_watch_t *watch) {
  return kr_xpdr_subunit_add (kr_xpdr, DECODER, watch);
}

int kr_xpdr_add_encoder (kr_xpdr_t *kr_xpdr, kr_xpdr_watch_t *watch) {
  return kr_xpdr_subunit_add (kr_xpdr, ENCODER, watch);
}

int kr_xpdr_add_demuxer (kr_xpdr_t *kr_xpdr, kr_xpdr_watch_t *watch) {
  return kr_xpdr_subunit_add (kr_xpdr, DEMUXER, watch);
}

int kr_xpdr_add_muxer (kr_xpdr_t *kr_xpdr, kr_xpdr_watch_t *watch) {
  return kr_xpdr_subunit_add (kr_xpdr, MUXER, watch);
}

void kr_xpdr_subunit_connect (kr_xpdr_subunit_t *kr_xpdr_subunit,
                              kr_xpdr_subunit_t *from_kr_xpdr_subunit) {
 
  kr_xpdr_port_connect (from_kr_xpdr_subunit,
                        kr_xpdr_subunit,
                        from_kr_xpdr_subunit->outputs[0],
                        kr_xpdr_subunit->inputs[0]);
}

void kr_xpdr_subunit_connect2 (kr_xpdr_subunit_t *kr_xpdr_subunit,
                               kr_xpdr_subunit_t *from_kr_xpdr_subunit) {
 
  kr_xpdr_port_connect (from_kr_xpdr_subunit,
                        kr_xpdr_subunit,
                        from_kr_xpdr_subunit->outputs[0],
                        kr_xpdr_subunit->inputs[1]);
}

void kr_xpdr_subunit_connect3 (kr_xpdr_subunit_t *kr_xpdr_subunit,
                               kr_xpdr_subunit_t *from_kr_xpdr_subunit) {
 
  kr_xpdr_port_connect (from_kr_xpdr_subunit,
                        kr_xpdr_subunit,
                        from_kr_xpdr_subunit->outputs[1],
                        kr_xpdr_subunit->inputs[0]);
}

int kr_xpdr_count (kr_xpdr_t *kr_xpdr) {
  
  int m;
  int c;
  
  c = 0;

  for (m = 0; m < KRAD_TRANSPONDER_SUBUNITS; m++) {
    if (kr_xpdr->subunits[m] != NULL) {
      c++;
    }
  }
  return c;
}

int kr_xpdr_get_info (kr_xpdr_t *kr_xpdr, int num, char *string) {
  
  if (kr_xpdr->subunits[num] != NULL) {
    sprintf (string, "Krad Transponder: Subunit %d - %s",
             num,
             transponder_subunit_type_to_string(kr_xpdr->subunits[num]->type));      
    return 1;
  }

  return -1;
}

void *kr_xpdr_get_link (kr_xpdr_t *kr_xpdr, int num) {
  
  if (kr_xpdr->subunits[num] != NULL) {
    return kr_xpdr->subunits[num]->watch->callback_pointer;
  }

  return NULL;
}

void krad_xpdr_destroy (kr_xpdr_t **xpdr) {

  int m;

  if (*xpdr != NULL) {

    printk ("Krad XPDR: Destroying");

    for (m = 0; m < KRAD_TRANSPONDER_SUBUNITS; m++) {
      if ((*xpdr)->subunits[m] != NULL) {
        kr_xpdr_subunit_destroy (&(*xpdr)->subunits[m]);
      }
    }

    free ((*xpdr)->subunits);
    free (*xpdr);
    *xpdr = NULL;
    
    printk ("Krad XPDR: Destroyed");
  }
}

kr_xpdr_t *krad_xpdr_create () {

  kr_xpdr_t *xpdr;

  printk ("Krad XPDR: Creating");

  xpdr = calloc (1, sizeof(kr_xpdr_t));
  xpdr->subunits = calloc (KRAD_TRANSPONDER_SUBUNITS,
                           sizeof(kr_xpdr_subunit_t *));

  printk ("Krad XPDR: Created");

  return xpdr;
}
