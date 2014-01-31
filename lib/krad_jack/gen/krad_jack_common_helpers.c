#include "krad_jack_common_helpers.h"

int kr_jack_direction_to_index(int val) {
  switch (val) {
    case KR_JACK_INPUT:
      return 0;
    case KR_JACK_OUTPUT:
      return 1;
  }
  return -1;
}

char *kr_strfr_kr_jack_direction(int val) {
  switch (val) {
    case KR_JACK_INPUT:
      return "kr_jack_input";
    case KR_JACK_OUTPUT:
      return "kr_jack_output";
  }
  return NULL;
}

int kr_strto_kr_jack_direction(char *string) {
  if (!strcmp(string,"kr_jack_input")) {
    return KR_JACK_INPUT;
  }
  if (!strcmp(string,"kr_jack_output")) {
    return KR_JACK_OUTPUT;
  }

  return -1;
}

int kr_jack_state_to_index(int val) {
  switch (val) {
    case KR_JACK_OFFLINE:
      return 0;
    case KR_JACK_ONLINE:
      return 1;
  }
  return -1;
}

char *kr_strfr_kr_jack_state(int val) {
  switch (val) {
    case KR_JACK_OFFLINE:
      return "kr_jack_offline";
    case KR_JACK_ONLINE:
      return "kr_jack_online";
  }
  return NULL;
}

int kr_strto_kr_jack_state(char *string) {
  if (!strcmp(string,"kr_jack_offline")) {
    return KR_JACK_OFFLINE;
  }
  if (!strcmp(string,"kr_jack_online")) {
    return KR_JACK_ONLINE;
  }

  return -1;
}

int kr_jack_setup_info_patch_apply(kr_jack_setup_info *info, kr_jack_setup_info_patch *patch) {
  const ptrdiff_t off[2] = { offsetof(kr_jack_setup_info, client_name), 
    offsetof(kr_jack_setup_info, server_name)
  };
  const size_t sz[2] = { sizeof(info->client_name), 
    sizeof(info->server_name)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_jack_info_patch_apply(kr_jack_info *info, kr_jack_info_patch *patch) {
  const ptrdiff_t off[9] = { offsetof(kr_jack_info, client_name), 
    offsetof(kr_jack_info, server_name), offsetof(kr_jack_info, state), 
    offsetof(kr_jack_info, inputs), offsetof(kr_jack_info, outputs), 
    offsetof(kr_jack_info, sample_rate), offsetof(kr_jack_info, period_size), 
    offsetof(kr_jack_info, xruns), offsetof(kr_jack_info, frames)
  };
  const size_t sz[9] = { sizeof(info->client_name), 
    sizeof(info->server_name), sizeof(info->state), 
    sizeof(info->inputs), sizeof(info->outputs), 
    sizeof(info->sample_rate), sizeof(info->period_size), 
    sizeof(info->xruns), sizeof(info->frames)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_jack_path_info_patch_apply(kr_jack_path_info *info, kr_jack_path_info_patch *patch) {
  const ptrdiff_t off[3] = { offsetof(kr_jack_path_info, name), 
    offsetof(kr_jack_path_info, channels), offsetof(kr_jack_path_info, direction)
  };
  const size_t sz[3] = { sizeof(info->name), 
    sizeof(info->channels), sizeof(info->direction)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

int kr_jack_setup_info_init(kr_jack_setup_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_jack_setup_info));

  return 0;
}

int kr_jack_setup_info_valid(kr_jack_setup_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 64; i++) {
    if (!st->client_name[i]) {
      break;
    }
    if (i == 63 && st->client_name[i]) {
      return -2;
    }
  }
  for (i = 0; i < 64; i++) {
    if (!st->server_name[i]) {
      break;
    }
    if (i == 63 && st->server_name[i]) {
      return -3;
    }
  }

  return 0;
}

int kr_jack_setup_info_random(kr_jack_setup_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_jack_setup_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->client_name[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->client_name[63] = '\0';
    }
  }
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->server_name[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->server_name[63] = '\0';
    }
  }

  return 0;
}

int kr_jack_info_init(kr_jack_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_jack_info));

  return 0;
}

int kr_jack_info_valid(kr_jack_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 64; i++) {
    if (!st->client_name[i]) {
      break;
    }
    if (i == 63 && st->client_name[i]) {
      return -2;
    }
  }
  for (i = 0; i < 64; i++) {
    if (!st->server_name[i]) {
      break;
    }
    if (i == 63 && st->server_name[i]) {
      return -3;
    }
  }

  return 0;
}

int kr_jack_info_random(kr_jack_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_jack_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->client_name[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->client_name[63] = '\0';
    }
  }
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->server_name[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->server_name[63] = '\0';
    }
  }

  return 0;
}

int kr_jack_path_info_init(kr_jack_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_jack_path_info));

  return 0;
}

int kr_jack_path_info_valid(kr_jack_path_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 64; i++) {
    if (!st->name[i]) {
      break;
    }
    if (i == 63 && st->name[i]) {
      return -2;
    }
  }

  return 0;
}

int kr_jack_path_info_random(kr_jack_path_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(kr_jack_path_info));
  for (i = 0; i < 64; i++) {
    scale = (double)25 / RAND_MAX;
    st->name[i] = 97 + floor(rand() * scale);
    if (i == 63) {
      st->name[63] = '\0';
    }
  }

  return 0;
}

