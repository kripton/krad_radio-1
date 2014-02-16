#include "data.h"
#include "krad_graph.h"

typedef enum {
  TU_SOURCE = 1,
  TU_BUS,
  TU_OUTPUT,
  TU_LINK
} tunit_type;

typedef struct {
  char name[64];
  int integer;
  kr_graph_elem elem;
  tunit_type type;
} tunit;

typedef struct {
  char *name;
  tunit_type type;
  int integer;
  kr_graph *graph;
  void *user;
  tunit *to;
  tunit *from;
} tunit_setup;