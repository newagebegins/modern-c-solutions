#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define V 5

size_t D[V][V] = {
  {0,6,SIZE_MAX,1,SIZE_MAX},
  {6,0,5,2,2},
  {SIZE_MAX,5,0,SIZE_MAX,5},
  {1,2,SIZE_MAX,0,1},
  {SIZE_MAX,2,5,1,0},
};

size_t S[V][2] = {0};
bool visited[V] = {0};
char names[V] = {'A', 'B', 'C', 'D', 'E'};

void initialize_S(size_t start) {
  for (size_t v = 0; v < V; ++v) {
    S[v][0] = v == start ? 0 : SIZE_MAX;
    S[v][1] = v;
  }
}

void initialize_visited() {
  for (size_t v = 0; v < V; ++v) {
    visited[v] = false;
  }
}

void print_S(void) {
  for (size_t v = 0; v < V; ++v) {
    printf("%c\t%zu\t%c\n", names[v], S[v][0], names[S[v][1]]);
  }
}

bool all_visited(void) {
  for (size_t v = 0; v < V; ++v) {
    if (!visited[v]) {
      return false;
    }
  }
  return true;
}

size_t next_vertex(void) {
  size_t min_v = SIZE_MAX;
  size_t min_dist = SIZE_MAX;
  for (size_t v = 0; v < V; ++v) {
    if (!visited[v] && S[v][0] < min_dist) {
      min_v = v;
      min_dist = S[v][0];
    }
  }
  return min_v;
}

bool are_neighbors(size_t x, size_t y) {
  return D[x][y] != SIZE_MAX;
}

// Based on https://youtu.be/pVfj6mxhdMw
void dijkstras_algorithm(size_t start) {  
  initialize_S(start);
  initialize_visited();
  while (!all_visited()) {
    size_t v = next_vertex();
    visited[v] = true;
    for (size_t n = 0; n < V; ++n) {
      if (are_neighbors(v, n) && !visited[n]) {
        size_t dist_from_start = S[v][0] + D[v][n];
        if (dist_from_start < S[n][0]) {
          S[n][0] = dist_from_start;
          S[n][1] = v;
        }
      }
    }
  }
}

int main(void) {
  printf("Starting at A:\n");
  dijkstras_algorithm(0);
  print_S();

  printf("\nStarting at C:\n");
  dijkstras_algorithm(2);
  print_S();

  return EXIT_SUCCESS;
}
