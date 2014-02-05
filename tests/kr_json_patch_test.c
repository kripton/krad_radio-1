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
  printf("%d step path :: ", kr_path_steps(path));
  for (;;) {
    len = kr_path_cur_name(path, &name);
    /*printf("/ (%d) %.*s ", len, len, name);*/
    printf("/ %.*s ", len, name);
    ret = kr_path_step(path);
    if (ret != 0) break;
  }
  printf("\n");
  /*len = kr_path_last(path, &name);
  printf("Last is (%d chars) %s\n", len, name);*/
}

int json_to_patchset(kr_path *path, char *json, int json_len, kr_patchset *patchset) {
  int ret;
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmntok_t *tok;
  jsmnerr_t err;
  int ntokens;
  int k;
  union {
    int integer;
    float real;
  } value;
  if ((path == NULL) || (json == NULL) || (json_len < 1) || (patchset == NULL)) {
    return -1;
  }
  printf("Going to parse:\n%s\n", json);
  if (1) {
    path_print(path);
    kr_path_rewind(path);
  }
  k = 0;
  ret = 0;
  patchset->used = 0;
  jsmn_init(&parser);
  err = jsmn_parse(&parser,json,tokens,512);
  ntokens = parser.toknext;
  if (err != JSMN_SUCCESS || ntokens < 3) {
    return -1;
  }
  printf("We got %d tokens\n", ntokens);
  tok = &tokens[k];
  if (tok->type == JSMN_OBJECT) {
    printf("First we got an object!!\n");
  } else {
    if (tok->type == JSMN_ARRAY) {
      printf("First we got an array!!\n");
    } else {
      printf("First we went wrong.. bailing..\n");
      return -1;
    }
  }
  for (k = 1; k < ntokens; k++) {
    if (patchset->used == KR_PATCHSET_PATCHES_MAX) {
      /* damn this would be attacker to fail */
      printf("Too many patches\n");
      return -1;
    }
    tok = &tokens[k];
    switch (tok->type) {
      case JSMN_OBJECT:
        printf("Token %d an object\n", k + 1);
        break;
      case JSMN_ARRAY:
        printf("Token %d an array\n", k + 1);
        break;
      case JSMN_STRING:
        printf("Token %d a string\n", k + 1);
        if (kr_path_steps_free(path) == 0) {
          /* no sploiting, disconnect this jerk */
          printf("Too many paths\n");
          return -1;
        }
        kr_path_push(path, &json[tok->start], tok->end - tok->start);
        if (1) {
          path_print(path);
          kr_path_rewind(path);
        }
        break;
      case JSMN_PRIMITIVE:
        printf("Token %d we got us a primitive!\n", k + 1);
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
        //} else {
          /* rage force disconnect the fool who sends a non number */
//          return -1;
  //      }
        break;
      default:
        printf("O dear major jsmn messup!\n");
        return -1;
        break;
    }
  }
  printf("Done parsing json patchset, we got %d patches\n\n", patchset->used);
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
  json_to_patchset(path, json, json_len, &patchset);
  json = "{\"drive\":5}";
  json_len = strlen(json);
  kr_path_parse(path, url, url_len);
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
