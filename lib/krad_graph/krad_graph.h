#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifndef KRAD_GRAPH_H
#define KRAD_GRAPH_H
#define MAX_VERTICES 64
#define MAX_EDGES 4096

typedef struct kr_graph kr_graph;
typedef struct kr_vertex kr_vertex;

typedef enum {
  KR_INPUT = 1,
  KR_BUS,
  KR_OUTPUT
} kr_vertex_type;

typedef struct {
  kr_vertex *from;
  kr_vertex *to;
} kr_edge;

struct kr_vertex {
  uint16_t adj[MAX_VERTICES];
  kr_vertex_type type;
};

struct kr_graph {
  kr_vertex vertices[MAX_VERTICES];
  kr_edge edges[MAX_EDGES];
};

typedef struct {
  int inputs;
  int outputs;
  int buses;
} kr_graph_setup;

int kr_graph_edge_destroy(kr_graph *graph, kr_vertex *to, kr_vertex *from);
int kr_graph_edge_create(kr_graph *graph, kr_vertex *to, kr_vertex *from);
int kr_graph_vertex_destroy(kr_graph *graph, kr_vertex *vertex);
kr_vertex *kr_graph_vertex_create(kr_graph *graph, kr_vertex_type type);
kr_graph *kr_graph_create(kr_graph_setup *setup);
int kr_graph_destroy(kr_graph *graph);

#endif
