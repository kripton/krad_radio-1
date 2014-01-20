#include "krad_graph.h"

int kr_graph_edge_create(kr_graph *graph, kr_vertex *to, kr_vertex *from) {
  int i;

  if (graph == NULL) return 1;
  if (to == from) return 1;
  if (to == NULL || from == NULL) return 1;

  /* TO DO, check for possible cycles */

  for (i = 0; i < MAX_EDGES; i++) {
    if (!graph->edges[i].from && !graph->edges[i].to) {
      graph->edges[i].from = from;
      graph->edges[i].to = to;
      break;
    }
  }

  return 1;
}

int kr_graph_edge_destroy(kr_graph *graph, kr_vertex *to, kr_vertex *from) {
  int i;
  if (!graph || !to || !from) return 1;

  for (i = 0; i < MAX_EDGES; i++) {
    if (graph->edges[i].from == from && graph->edges[i].to == to) {
      memset(&graph->edges[i],0,sizeof(kr_edge));
      return 0;
    }
  }

  return 1;
}

kr_vertex *kr_graph_vertex_create(kr_graph *graph, kr_vertex_type type) {
  kr_vertex *vertex;
  int i;

  if (graph == NULL || type == 0) return NULL;

  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type == 0) {
      graph->vertices[i].type = type;
      return &graph->vertices[i];
    }
  }

  return NULL;
}

int kr_graph_vertex_destroy(kr_graph *graph, kr_vertex *vertex) {
  int i;

  if (graph == NULL || vertex == NULL) return 1;

  for (i = 0; i < MAX_EDGES; i++) {
    if (graph->edges[i].from == vertex || graph->edges[i].to == vertex) {
      memset(&graph->edges[i],0,sizeof(kr_edge));
    }
  }

  memset(vertex,0,sizeof(kr_vertex));

  return 0;
}

kr_graph *kr_graph_create(kr_graph_setup *setup) {
  kr_graph *graph;

  if (setup == NULL) return NULL;

  graph = calloc(1,sizeof(kr_graph));
  
  return graph;
}

int kr_graph_destroy(kr_graph *graph) {
  if (graph == NULL) return 1;
  free(graph);
  return 0;
}
