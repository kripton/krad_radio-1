#include "krad_path.h"
#include <string.h>

struct kr_path {
  char *component[KR_PATH_COMPONENT_MAX];
  int len[KR_PATH_COMPONENT_MAX];
  int count;
  int cur;
};

size_t kr_path_sizeof() {
  return sizeof(kr_path);
}

int kr_path_parse(kr_path *path, char *str, int len) {
  if (path == NULL) return -1;
  if (str == NULL) return -1;
  return 0;
}

int kr_path_clear(kr_path *path) {
  if (path == NULL) return -1;
  memset(path, 0, sizeof(*path));
  return 0;
}

int kr_path_step(kr_path *path) {
  if (path == NULL) return -1;
  return 0;
}

int kr_path_steps(kr_path *path) {
  if (path == NULL) return -1;
  return 0;
}

int kr_path_rewind(kr_path *path) {
  if (path == NULL) return -1;
  return 0;
}

int kr_path_last(kr_path *path, char **name) {
  if (path == NULL) return -1;
  if (name == NULL) return -1;
  return 0;
}

int kr_path_steps_ahead(kr_path *path) {
  if (path == NULL) return -1;
  return 0;
}

int kr_path_cur_name(kr_path *path, char **name) {
  if (path == NULL) return -1;
  if (name == NULL) return -1;
  return 0;
}

int kr_path_push(kr_path *path, char *str, int len) {
  if (path == NULL) return -1;
  if (str == NULL) return -1;
  if (len < 0) return -1;
  return 0;
}

int kr_path_clear_last(kr_path *path) {
  if (path == NULL) return -1;
  return 0;
}
