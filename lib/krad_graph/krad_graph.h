#include <stdint.h>
#ifndef KRAD_GRAPH_H
#define KRAD_GRAPH_H
#define MAX_VERTICES 64
#define MAX_EDGES 4096

typedef struct kr_graph kr_graph;
typedef struct kr_vertex kr_vertex;

typedef void (kr_graph_edge_destroy_cb)(void *user);
typedef void (kr_graph_vertex_process_cb)(void *user_to, void **user_from, int from_len);

typedef enum {
  IN,
  OUT
} kr_edge_dir;

typedef enum {
  KR_INPUT = 1,
  KR_BUS,
  KR_OUTPUT
} kr_vertex_type;

typedef struct {
  kr_vertex *from;
  kr_vertex *to;
  void *user;
} kr_edge;

struct kr_vertex {
  uint16_t adj[MAX_VERTICES];
  uint16_t deps[MAX_VERTICES];
  kr_vertex_type type;
  void *user;
};

struct kr_graph {
  kr_vertex vertices[MAX_VERTICES];
  kr_edge edges[MAX_EDGES];
  kr_graph_edge_destroy_cb *edge_destroy_cb;
  kr_graph_vertex_process_cb *vertex_process_cb;
};

typedef struct {
  int inputs;
  int outputs;
  int buses;
  kr_graph_edge_destroy_cb *edge_destroy_cb;
  kr_graph_vertex_process_cb *vertex_process_cb;
} kr_graph_setup;

int kr_graph_edge_destroy(kr_graph *graph, kr_edge *edge);
kr_edge *kr_graph_edge_create(kr_graph *graph, kr_vertex *to, kr_vertex *from, void *user);
int kr_graph_vertex_destroy(kr_graph *graph, kr_vertex *vertex);
kr_vertex *kr_graph_vertex_create(kr_graph *graph, kr_vertex_type type, void *user);
kr_graph *kr_graph_create(kr_graph_setup *setup);
int kr_graph_destroy(kr_graph *graph);
int kr_vertex_deps(kr_graph *graph, kr_vertex *vertex, kr_vertex **deps, int max);
int kr_graph_in_out_edges(kr_graph *graph, kr_vertex *vertex, int dir, void **user, int max);

#endif
