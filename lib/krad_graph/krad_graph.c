#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "krad_graph.h"

enum deps_indeps {
  DEPS = 1,
  INDEPS
};

static int vertex_index(kr_graph *graph, kr_vertex *v) {
  int8_t i;
  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type && (&graph->vertices[i] == v)) {
      return i;
    }
  }
  return -1;
}

static int visit(kr_graph *graph, uint8_t v, uint8_t *marked) {
  uint16_t i;
  uint16_t *adj;
  kr_vertex *ve;
  marked[v] = 1;
  ve = &graph->vertices[v];
  adj = ve->adj;
  for (i = 0; i < MAX_VERTICES; i++) {
    if (adj[i]) {
      if (marked[i] == 1) {
        return 1;
      }
      else if (marked[i] == 0) {
        if (visit(graph,i,marked)) return 1;
      }
    }
  }
  marked[v] = 2;
  return 0;
}

static int kr_graph_is_cyclic(kr_graph *graph) {
  uint8_t marked[MAX_VERTICES];
  uint8_t i;
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

static int get_in_edges(kr_graph *graph, kr_vertex *vertex, void **user, int max) {
    uint16_t i;
    int count;
    for (i = count = 0; i < MAX_EDGES; i++) {
      if (graph->edges[i].to == vertex) {
        if (graph->edges[i].user) {
          user[count++] = graph->edges[i].user;
        }
      }
    }
    return count;
}

static int get_out_edges(kr_graph *graph, kr_vertex *vertex, void **user, int max) {
    uint16_t i;
    int count;
    for (i = count = 0; i < MAX_EDGES; i++) {
      if (graph->edges[i].from == vertex) {
        if (graph->edges[i].user) {
          user[count++] = graph->edges[i].user;
        }
      }
    }
    return count;
}

static int vertex_deps_indeps(kr_graph *graph, kr_vertex *vertex, 
  kr_vertex **out, int max, int type) {
  kr_vertex *vertices[MAX_VERTICES];
  kr_vertex *done[MAX_VERTICES];
  uint16_t *vrts;
  uint16_t k;
  int16_t i;
  int16_t j;
  int16_t count;

  k = 0;
  count = 0;
  vertices[k++] = vertex;

  while (k) {
    vertex = vertices[--k];
    if (type == DEPS)
      vrts = vertex->deps;
    else
      vrts = vertex->adj;
    for (i = 0; i < MAX_VERTICES; i++) {
      if (vrts[i]) {
        for (j = 0; j < vrts[i]; j++) {
          done[count++] = &graph->vertices[i];
        }
        vertices[k++] = &graph->vertices[i];
      }
    }
  }

  k = 0;

  for (i = count-1; i >= 0; i--) {
    if (k >= max) return k;
    out[k++] = done[i];
  }

  return k;
}

int kr_graph_in_out_edges(kr_graph *graph, kr_vertex *vertex, 
  int dir, void **user, int max) {
  int count;

  if (max == 0) return 0;

  count = 0;

  if (dir == IN) {
    count = get_in_edges(graph,vertex,user,max);
  } else if (dir == OUT) {
    count = get_out_edges(graph,vertex,user,max);
  } else {
    count += get_in_edges(graph,vertex,user,max);
    count += get_out_edges(graph,vertex,user,max);
  }

  return count;
}

int kr_vertex_indeps(kr_graph *graph, kr_vertex *vertex, 
  kr_vertex **indeps, int max) {
  if (max == 0) return 0;
  return vertex_deps_indeps(graph, vertex, indeps, max, INDEPS);
}

int kr_vertex_deps(kr_graph *graph, kr_vertex *vertex, 
  kr_vertex **deps, int max) {
  if (max == 0) return 0;
  return vertex_deps_indeps(graph, vertex, deps, max, DEPS);
}

static int kr_graph_edge_destroy_internal(kr_graph *graph, kr_edge *edge) {
  if(!edge->to || !edge->from) {
    memset(edge,0,sizeof(kr_edge));
    return 1;
  } else {
    if (graph->edge_destroy_cb) {
      graph->edge_destroy_cb(edge->user);
    }
    edge->from->adj[vertex_index(graph,edge->to)]--;
    edge->to->deps[vertex_index(graph,edge->from)]--;
    memset(edge,0,sizeof(kr_edge));
    return 0;
  }
}

int kr_graph_edge_destroy(kr_graph *graph, kr_edge *edge) {
  if (!graph || !edge) return 1;
  return kr_graph_edge_destroy_internal(graph,edge);
}

kr_edge *kr_graph_edge_create(kr_graph *graph, kr_vertex *to, kr_vertex *from, void *user) {
  uint16_t i;
  if (graph == NULL) return NULL;
  if (to == from) return NULL;
  if (to == NULL || from == NULL) return NULL;
  if (to->type == 0 || from->type == 0) return NULL;
  for (i = 0; i < MAX_EDGES; i++) {
    if (!graph->edges[i].from && !graph->edges[i].to) {
      graph->edges[i].from = from;
      graph->edges[i].to = to;
      graph->edges[i].user = user;
      from->adj[vertex_index(graph,to)]++;
      to->deps[vertex_index(graph,from)]++;
      if (kr_graph_is_cyclic(graph)) {
        printf("Cycle detected!\n");
        kr_graph_edge_destroy(graph,&graph->edges[i]);
        return NULL;
      } else {
        return &graph->edges[i];
      }
    }
  }
  return NULL;
}

kr_vertex *kr_graph_vertex_create(kr_graph *graph, kr_vertex_type type, void *user) {
  uint8_t i;
  if (graph == NULL || type == 0) return NULL;
  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type == 0) {
      graph->vertices[i].type = type;
      graph->vertices[i].user = user;
      return &graph->vertices[i];
    }
  }
  return NULL;
}

static int kr_graph_vertex_destroy_internal(kr_graph *graph, kr_vertex *vertex) {
  uint16_t i;
  for (i = 0; i < MAX_EDGES; i++) {
    if (graph->edges[i].from == vertex || graph->edges[i].to == vertex) {
      if (kr_graph_edge_destroy_internal(graph,&graph->edges[i])) {
        return 1;
      }
    }
  }
  memset(vertex,0,sizeof(kr_vertex));
  return 0;
}

int kr_graph_vertex_destroy(kr_graph *graph, kr_vertex *vertex) {
  if (graph == NULL || vertex == NULL) return 1;
  return kr_graph_vertex_destroy_internal(graph,vertex);
}

kr_graph *kr_graph_create(kr_graph_setup *setup) {
  kr_graph *graph;
  if (setup == NULL) return NULL;
  graph = calloc(1,sizeof(kr_graph));
  graph->edge_destroy_cb = setup->edge_destroy_cb;
  graph->vertex_process_cb = setup->vertex_process_cb;
  return graph;
}

int kr_graph_destroy(kr_graph *graph) {
  if (graph == NULL) return 1;
  free(graph);
  return 0;
}
