#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#define NUM_NODES 9

char nodes[NUM_NODES] = {'A','B','C','D','E','F','G','H','I'};
bool visited[NUM_NODES] = {0};
bool adjacency_matrix[NUM_NODES][NUM_NODES] = {
  {0,0,1,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0,0},
  {1,1,0,0,1,0,0,0,0},
  {0,0,0,0,1,0,0,0,0},
  {0,0,1,1,0,1,0,0,0},
  {0,0,0,0,1,0,1,1,0},
  {0,0,0,0,0,1,0,0,0},
  {0,0,0,0,0,1,0,0,1},
  {0,0,0,0,0,0,0,1,0},
};

unsigned queue[NUM_NODES] = {0};
int front = -1;
int rear = -1;

bool is_empty() {
  return front == -1;
}

bool is_full() {
  return (rear + 1) % NUM_NODES == front;
}

void enqueue(unsigned x) {
  assert(!is_full());
  if (is_empty()) {
    front = 0;
    rear = 0;
  } else {
    rear = (rear + 1) % NUM_NODES;
  }
  queue[rear] = x;
}

unsigned dequeue() {
  assert(!is_empty());
  unsigned x = queue[front];
  if (front == rear) {
    front = -1;
    rear = -1;
  } else {
    front = (front + 1) % NUM_NODES;
  }
  return x;
}

void breadth_first_search() {
  while (!is_empty()) {
    unsigned x = dequeue();
    printf("%c ", nodes[x]);
    visited[x] = true;
    for (unsigned n = 0; n < NUM_NODES; ++n) {
      if (adjacency_matrix[x][n] && !visited[n]) {
        enqueue(n);
      }
    }
  }
  printf("\n");
}

int main(void) {
  enqueue(4);
  breadth_first_search();
  return 0;
}
