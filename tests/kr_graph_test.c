#include "krad_graph.h"

char *kr_graph_vertex_type_to_str(kr_vertex_type type) {
  switch (type) {
    case KR_INPUT: return "INPUT";
    case KR_BUS: return "BUS";
    case KR_OUTPUT: return "OUTPUT";
    default: break;
  }
  return NULL;
}

void print_graph_info(kr_graph *graph) {
  int i;
  int j;

  for (i = j = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type) {
      j++;
      printf("  vertex %d of type %s\n",
        j-1,kr_graph_vertex_type_to_str(graph->vertices[i].type));
    }
  }

  if (j == 0) {
    printf("  your graph is empty...\n");
    return;
  }

  for (i = 0; i < MAX_EDGES; i++) {
    if (graph->edges[i].from && graph->edges[i].to) {
      printf("  we have an edge from a %s %p to %s %p\n",
        kr_graph_vertex_type_to_str(graph->edges[i].from->type), graph->edges[i].from,
        kr_graph_vertex_type_to_str(graph->edges[i].to->type), graph->edges[i].to);
    }
  }

}

int main(int argc, char const *argv[]) {
  kr_graph *graph;
  kr_graph_setup graph_setup;
  kr_vertex *vertices[5];

  memset(&graph_setup,0,sizeof(kr_graph_setup));

  graph = kr_graph_create(&graph_setup);

  if (graph == NULL) {
    fprintf(stderr, "graph creation failed!\n");
    return 1;
  }

  printf("graph created successfully!\n");
  printf("graph info: \n");

  print_graph_info(graph);

  printf("creating some vertices in our graph...\n");

  vertices[0] = kr_graph_vertex_create(graph,KR_INPUT);
  vertices[1] = kr_graph_vertex_create(graph,KR_INPUT);
  vertices[2] = kr_graph_vertex_create(graph,KR_BUS);
  vertices[3] = kr_graph_vertex_create(graph,KR_BUS);
  vertices[4] = kr_graph_vertex_create(graph,KR_OUTPUT);

  printf("graph info: \n");

  print_graph_info(graph);

 // printf("destroying a %s vertex\n",kr_graph_vertex_type_to_str(vertices[2]->type));

//  kr_graph_vertex_destroy(graph,vertices[2]);

  printf("graph info: \n");
  print_graph_info(graph);

  printf("connecting some vertices together\n");
  int monkey = 1;
  if (monkey) {
    kr_graph_edge_create(graph,vertices[2],vertices[3]);
  }
  kr_graph_edge_create(graph,vertices[4],vertices[0]);
  kr_graph_edge_create(graph,vertices[4],vertices[3]);
  kr_graph_edge_create(graph,vertices[3],vertices[1]);
  kr_graph_edge_create(graph,vertices[3],vertices[2]);
  kr_graph_edge_create(graph,vertices[3],vertices[2]);
  kr_graph_edge_create(graph,vertices[2],vertices[3]); /* this gets denied if monkey = 0 */

  printf("graph info: \n");

  print_graph_info(graph);

  printf("destroying graph now.\n");
  kr_graph_destroy(graph);

  return 0;
}
