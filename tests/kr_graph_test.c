#include "krad_graph.h"
#include "krad_timer.h"

void print_usage(const char *cmd) {
  printf("Usage:\n  %s num_of_vertices num_of_edges\n",cmd);
}

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
      printf("  vertex %p of type %s\n",
        &graph->vertices[i],kr_graph_vertex_type_to_str(graph->vertices[i].type));
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

unsigned int randr(unsigned int min, unsigned int max) {

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  scale = (double)abs(min - max) / RAND_MAX;

  return min + floor(rand() * scale);;
}

int random_vertices_gen(kr_graph *graph, int n, kr_vertex **vertices) {
  int i;
  int k;
  for (i = k = 0; i < n; i++) {
    vertices[k] = kr_graph_vertex_create(graph,randr(1,3));
    if (vertices[k]) {
      k++;
    }
  }
  return k;
}

int random_edges_gen(kr_graph *graph, int n, int vcount, kr_vertex **vertices) {
  int i;
  int j;
  int k;
  int l;

  for (i = l = 0; i < n; i++) {
    j = randr(0,vcount-1);
    k = randr(0,vcount-1);
    printf("Generating random edge from %p to %p\n",vertices[k],vertices[j]);
    if (!kr_graph_edge_create(graph,vertices[j],vertices[k])) {
      l++;
    }
  }

  return l;
}

int main(int argc, char const *argv[]) {
  kr_graph *graph;
  kr_timer *timer;
  kr_graph_setup graph_setup;
  kr_vertex *vertices[MAX_VERTICES];
  int vertex_count;
  int edge_count;

  if (argc != 3) {
    print_usage(argv[0]);
    return 0;
  }

  if (atoi(argv[1]) > MAX_VERTICES) {
    fprintf(stderr, "MAX_VERTICES (%d) exceeded\n",MAX_VERTICES);
    return 1;
  }

  if (atoi(argv[2]) > MAX_EDGES) {
    fprintf(stderr, "MAX_EDGES (%d) exceeded\n",MAX_EDGES);
    return 1;
  }

  memset(&graph_setup,0,sizeof(kr_graph_setup));

  timer = kr_timer_create();
  kr_timer_start(timer);

  graph = kr_graph_create(&graph_setup);

  if (graph == NULL) {
    fprintf(stderr, "graph creation failed!\n");
    return 1;
  }

  printf("graph created successfully!\n\n");

  vertex_count = random_vertices_gen(graph,atoi(argv[1]),vertices);
  printf("Generated %d random vertices\n\n",vertex_count);

  kr_timer_status(timer);

  edge_count = random_edges_gen(graph,atoi(argv[2]),vertex_count,vertices);

  printf("\nGenerated %d random edges\n\n",edge_count);

  kr_timer_status(timer);

  //printf("graph info: \n\n");
  //print_graph_info(graph);

  printf("destroying graph now.\n");
  kr_graph_destroy(graph);

  kr_timer_status(timer);

  kr_timer_finish(timer);
  kr_timer_destroy(timer);

  return 0;
}
