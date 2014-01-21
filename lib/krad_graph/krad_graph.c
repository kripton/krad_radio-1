#include "krad_graph.h"

static int vertex_index(kr_graph *graph, kr_vertex *v) {
  int i;

  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type && (&graph->vertices[i] == v)) {
      return i;
    }
  }

  return -1;
}

static int kr_vertex_adj(kr_graph *graph, kr_vertex *v, int *adj) {
  int n;
  int i;
  n = 0;

  for (i = 0; i < MAX_EDGES; i++) {
    if (graph->edges[i].to && graph->edges[i].from) {
      if (graph->edges[i].from == v) {
        adj[n++] = vertex_index(graph,graph->edges[i].to);
        if (adj[n-1] < 0) n--;
      }
    }
  }

  return n;
}

static int visit(kr_graph *graph, int v, uint8_t *marked) {
  int i;
  int n;
  int adj[MAX_EDGES];

  marked[v] = 1;
  n = kr_vertex_adj(graph,&graph->vertices[v],adj);
  
  for (i = 0; i < n; i++) {
    if (marked[adj[i]] == 1) {
      return 1;
    }
    else if (marked[adj[i]] == 0) {
      if (visit(graph,adj[i],marked)) return 1;
    }
  }

  marked[v] = 2;
  return 0;
}
 
static int kr_graph_is_cyclic(kr_graph *graph) {
  uint8_t marked[MAX_VERTICES];
  int i;

  memset(marked,0,sizeof(marked));

  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type) {
      if (marked[i] == 0) {
        if (visit(graph,i,marked)) return 1;
      }
    }
  }
  return 0;
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

int kr_graph_edge_create(kr_graph *graph, kr_vertex *to, kr_vertex *from) {
  int i;
  if (graph == NULL) return 1;
  if (to == from) return 1;
  if (to == NULL || from == NULL) return 1;
  if (to->type == 0 || from->type == 0) return 1;

  for (i = 0; i < MAX_EDGES; i++) {
    if (!graph->edges[i].from && !graph->edges[i].to) {
      graph->edges[i].from = from;
      graph->edges[i].to = to;
      if (kr_graph_is_cyclic(graph)) {
        printf("Cycle detected!\n");
        kr_graph_edge_destroy(graph,to,from);
        return 1;
      } else {
        return 0;
      }
    }
  }
  
  return 1;
}

kr_vertex *kr_graph_vertex_create(kr_graph *graph, kr_vertex_type type) {
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