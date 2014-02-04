#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "../lib/gen/jsmn/jsmn.h"
#include "../lib/gen/jsmn/jsmn.c"

#include "../lib/krad_app/krad_path.h"
#include "../lib/krad_app/krad_path.c"

#define KR_PATCH_DEPTH_MAX 8
#define KR_PATCHSET_PATCHES_MAX 128

typedef struct kr_patch kr_patch;

struct kr_patch {
  int coconut;
};

typedef struct kr_patchset kr_patchset;

struct kr_patchset {
  kr_patch patches[KR_PATCHSET_PATCHES_MAX];
  int used;
};

static void path_print(kr_path *path) {
  char *name;
  int len;
  int ret;
  name = NULL;
  if (path == NULL) {
    fprintf(stderr, "path_print called with null kr_path\n");
    return;
  }
  printf("printing %d steps\n", kr_path_steps(path));
  for (;;) {
    len = kr_path_cur_name(path, &name);
    printf("Component: (%d chars) %s\n", len, name);
    ret = kr_path_step(path);
    if (ret != 0) break;
  }
  len = kr_path_last(path, &name);
  printf("Last is (%d chars) %s\n", len, name);
}

int json_to_patchset(kr_path *path, char *json, int len, kr_patchset *patchset) {
  int ret;
  union {
    int integer;
    float real;
  } value;
  //while (stepping thru json) {
  while (1) {
    if (patchset->used == KR_PATCHSET_PATCHES_MAX) {
      /* damn this would be attacker to fail */
      printf("Too many patches\n");
      return -1;
    }
    if (1) {
    //if (element_name) {
      if (kr_path_steps_ahead(path) == 0) {
        /* no sploiting, disconnect this jerk */
        printf("Too many paths\n");
        return -1;
      }
      //path[path_len++] = element_name;
    } else {
      if (1) {
     /* if ((element_value) && (element_value_is_number)) {
        if (element_is_float) value.real = element_value;
        if (element_is_int) value.integer = element_value;
        ret = kr_strarr_to_patch(&patchset->patches[patchset->used], value, path, path_len);
        */
        if (ret != 0) {
          /* damn this patch is invalid as hell */
          return -1;
        }
        patchset->used++;
        kr_path_clear_last(path);
      } else {
        /* rage force disconnect the fool who sends a non number */
        return -1;
      }
    }
  }
  return patchset->used;
}

static int test() {
  kr_path *path;
  char *url;
  int url_len;
  char *json;
  int json_len;
  kr_patchset patchset;
  memset(&patchset, 0, sizeof(patchset));
  kr_path_alloca(path);
  url = "/mixer";
  url_len = strlen(url);
  kr_path_parse(path, url, url_len);
  json = "[{\"bongo\":\"coconut\"}]";
  json_len = strlen(json);
  path_print(path);
  kr_path_rewind(path);
  json_to_patchset(path, json, json_len, &patchset);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  ret = test();
  if (ret) {
    fprintf(stderr, "Fail.\n");
  } else {
    printf("It worked!\n");
  }
  return ret;
}
