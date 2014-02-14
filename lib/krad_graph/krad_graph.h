#include <stdint.h>
#ifndef KRAD_GRAPH_H
#define KRAD_GRAPH_H
#define MAX_VERTICES 64
#define MAX_EDGES 1024

typedef struct kr_graph kr_graph;
typedef struct kr_vertex kr_vertex;
typedef struct kr_edge kr_edge;
typedef enum kr_vertex_type kr_vertex_type;
typedef enum kr_edge_dir kr_edge_dir;

typedef void (kr_graph_edge_destroy_cb)(void *user);
typedef void (kr_graph_vertex_process_cb)(void *user_to, void **user_from, int from_len);

enum kr_vertex_type {
  KR_VERTEX_SOURCE = 1,
  KR_VERTEX_BUS,
  KR_VERTEX_OUTPUT
};

typedef struct {
  int inputs;
  int outputs;
  int buses;
  kr_graph_edge_destroy_cb *edge_destroy_cb;
  kr_graph_vertex_process_cb *vertex_process_cb;
} kr_graph_setup;

int kr_graph_in_out_edges(kr_graph *graph, kr_vertex *vertex, int dir, void **user, int max);
int kr_graph_output_users_from(kr_graph *graph, kr_vertex *vertex, void **user, int max);
int kr_graph_output_users_from_edge(kr_graph *graph, kr_edge *edge, void **user, int max);
int kr_graph_source_users_from(kr_graph *graph, kr_vertex *vertex, void **user, int max);
int kr_graph_chains(kr_graph *graph, kr_vertex *to, kr_vertex *from, void ***user, int max, int max_len);
int kr_graph_edge_destroy(kr_graph *graph, kr_edge *edge);
kr_edge *kr_graph_edge_create(kr_graph *graph, kr_vertex *to, kr_vertex *from, void *user);
int kr_graph_vertex_destroy(kr_graph *graph, kr_vertex *vertex);
kr_vertex *kr_graph_vertex_create(kr_graph *graph, kr_vertex_type type, void *user);
int kr_graph_destroy(kr_graph *graph);
kr_graph *kr_graph_create(kr_graph_setup *setup);
#endif
