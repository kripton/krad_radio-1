#include "krad_path.h"
#include <string.h>

struct kr_path {
  char full[KR_PATH_LENGTH_MAX + 1];
  char *component[KR_PATH_COMPONENT_MAX];
  int len[KR_PATH_COMPONENT_MAX];
  int count;
  int cur;
};

size_t kr_path_sizeof() {
  return sizeof(kr_path);
}

int kr_path_parse(kr_path *path, char *str, int len) {
  char *cur;
  int i;
  if (path == NULL) return -1;
  if (str == NULL) return -1;
  if (len < 1) return -1;
  if (len > KR_PATH_LENGTH_MAX) return -1;
  kr_path_clear(path);
  path->full[KR_PATH_LENGTH_MAX] = '\0';
  path->cur = 0;
  path->count = 0;
  memcpy(path->full, str, len);
  path->full[len] = '\0';
  cur = path->full;
  while ((cur = strchr(cur, '/'))) {
    path->component[path->count] = cur + 1;
    path->count++;
    cur[0] = '\0';
    cur++;
    if (!strlen(cur)) {
      path->count = 0;
      return -1;
    }
  }
  for (i = 0; i < path->count; i++) {
    path->len[i] = strlen(path->component[i]);
  }
  return 0;
}

int kr_path_clear(kr_path *path) {
  if (path == NULL) return -1;
  memset(path, 0, sizeof(*path));
  return 0;
}

int kr_path_step(kr_path *path) {
  if (path == NULL) return -1;
  if (path->count < 1) return -1;
  if ((path->count - 1) == path->cur) return -1;
  if ((path->count - 1) < path->cur) return -2;
  if ((path->count - 1) > path->cur) path->cur++;
  return 0;
}

int kr_path_steps(kr_path *path) {
  if (path == NULL) return -1;
  return path->count;
}

int kr_path_rewind(kr_path *path) {
  if (path == NULL) return -1;
  path->cur = 0;
  return 0;
}

int kr_path_last(kr_path *path, char **name) {
  if (path == NULL) return -1;
  if (name == NULL) return -1;
  if (path->count < 1) return -1;
  *name = path->component[path->count - 1];
  return path->len[path->count - 1];
}

int kr_path_steps_ahead(kr_path *path) {
  if (path == NULL) return -1;
  if (path->count < 1) return -1;
  return (path->count - 1) - path->cur;
}

int kr_path_steps_free(kr_path *path) {
  if (path == NULL) return -1;
  return KR_PATH_COMPONENT_MAX - path->count;
}

int kr_path_cur_name(kr_path *path, char **name) {
  if (path == NULL) return -1;
  if (name == NULL) return -1;
  if (path->count < 1) return -1;
  *name = path->component[path->cur];
  return path->len[path->cur];
}

int kr_path_push(kr_path *path, char *str, int len) {
  if (path == NULL) return -1;
  if (str == NULL) return -1;
  if (len < 0) return -1;
  if (path->count == KR_PATH_COMPONENT_MAX) return -1;
  path->component[path->count] = str;
  path->len[path->count] = len;
  path->count++;
  return 0;
}

int kr_path_clear_last(kr_path *path) {
  if (path == NULL) return -1;
  if (path->count < 1) return -1;
  path->component[path->count - 1] = NULL;
  path->len[path->count - 1] = 0;
  path->count--;
  return 0;
}
