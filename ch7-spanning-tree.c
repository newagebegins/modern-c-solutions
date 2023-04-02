#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#define NUM_NODES 6

bool visited[NUM_NODES] = {0};
bool G[NUM_NODES][NUM_NODES] = {
  {0,1,0,1,1,0},
  {1,0,1,1,0,0},
  {0,1,0,1,0,1},
  {1,1,1,0,1,0},
  {1,0,0,1,0,1},
  {0,0,1,0,1,0},
};
bool G_spanning_tree[NUM_NODES][NUM_NODES] = {0};

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

void printMatrix(bool m[NUM_NODES][NUM_NODES]) {
  for (unsigned r = 0; r < NUM_NODES; ++r) {
    for (unsigned c = 0; c < NUM_NODES; ++c) {
      printf("%u ", m[r][c]);
    }
    printf("\n");
  }
}

int main(void) {
  printf("G:\n");
  printMatrix(G);
  enqueue(0);
  visited[0] = true;
  while (!is_empty()) {
    unsigned x = dequeue();
    for (unsigned n = 0; n < NUM_NODES; ++n) {
      if (G[x][n] && !visited[n]) {
        G_spanning_tree[x][n] = true;
        G_spanning_tree[n][x] = true;
        enqueue(n);
        visited[n] = true;
      }
    }
  }
  printf("\nSpanning tree of G:\n");
  printMatrix(G_spanning_tree);
  return 0;
}
