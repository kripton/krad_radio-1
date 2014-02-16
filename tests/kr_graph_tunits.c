#include "kr_graph_tunits.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

uint randr(uint min, uint max) {
  struct timeval tv;
  double scale;
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);
  scale = (double)abs(min - max) / RAND_MAX;
  return min + floor(rand() * scale);;
}

tunit *tunit_create(tunit_setup *setup) {
  tunit *unit;
  unit = calloc(1, sizeof(tunit));
  if (unit == NULL) return unit;
  unit->type = setup->type;
  if (setup->type != TU_LINK) {
    unit->elem = kr_graph_vertex(setup->graph, setup->type, setup->user);
  } else {
    unit->elem = kr_graph_link(setup->graph, &setup->to->elem,
     &setup->from->elem, setup->user);
  }
  if (unit->elem.type == 0) {
    free(unit);
    return NULL;
  }
  strncpy(unit->name, setup->name, sizeof(unit->name));
  unit->integer = setup->integer;
  return unit;
}

int tunit_process(tunit *tunit) {
  return 0;
}

int tunit_ctl(tunit *tunit) {
  return 0;
}

int tunit_destroy(tunit *unit) {
  free(unit);
  return 0;
}

tunit *tunit_random_gen(kr_graph *graph, int type, int k, tunit **units, int nv) {
  tunit_setup tunit_setup;
  memset(&tunit_setup, 0, sizeof(tunit_setup));
  tunit_setup.graph = graph;
  tunit_setup.name = names[k];
  tunit_setup.integer = randr(0, 10);
  tunit_setup.type = type;
  if (type == TU_LINK) {
    tunit_setup.from = units[randr(0, nv)];
    tunit_setup.to = units[randr(0, nv)];
  }
  return tunit_create(&tunit_setup);
}

void tunits_destroy(tunit **units, int n) {
  int i;
  for (i = 0; i < n; i++) {
    tunit_destroy(units[i]);
  }
}

int tunits_random_populate(kr_graph *graph, tunit **units, int max) {
  int n;
  int v;
  int e;
  int i;
  tunit *tunit;
  n = randr(4,max);
  for (i = v = 0; i < n; i++) {
    tunit = tunit_random_gen(graph, randr(TU_SOURCE, TU_OUTPUT+1), v, NULL, 0);
    if (tunit) {
      units[v] = tunit;
      v++;
    }
  }
  n = randr(0,max-v);
  for (i = e = 0; i < n; i++) {
    tunit = tunit_random_gen(graph, TU_LINK, v+e, units, v);
    if (tunit) {
      units[v+e] = tunit;
      e++;
    }
  }
  printf("%d random tunits generated! %d vertices, %d edges\n",v+e,v,e);
  return v+e;
}

int run_test() {
  int n;
  tunit *units[32];
  kr_graph *graph;
  kr_graph_setup graph_setup;
  memset(&graph_setup, 0, sizeof(graph_setup));
  graph = kr_graph_create(&graph_setup);
  if (graph == NULL) return 1;
  n = tunits_random_populate(graph, units, 32);
  tunits_destroy(units,n);
  kr_graph_destroy(graph);
  return 0;
}

int main(int argc, char const *argv[]) {
  int res;
  res = run_test();
  return res;
}