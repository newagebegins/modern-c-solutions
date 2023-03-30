#include <stdlib.h>
#include <stdio.h>

size_t Find(size_t parent[], size_t i) {
  if (parent[i] == SIZE_MAX) {
    return i;
  }
  return Find(parent, parent[i]);
}

void FindReplace(size_t parent[], size_t i, size_t x) {
  if (i == x) {
    return;
  }
  if (parent[i] != SIZE_MAX) {
    FindReplace(parent, parent[i], x);
  }
  parent[i] = x;
}

size_t FindCompress(size_t parent[], size_t i) {
  size_t root = Find(parent, i);
  FindReplace(parent, i, root);
  return root;
}

void Union(size_t parent[], size_t i, size_t j) {
  size_t root = FindCompress(parent, i);
  FindReplace(parent, j, root);
}

void initialize(size_t parent[]) {
  parent[0] = SIZE_MAX;
  parent[1] = 0;
  parent[2] = 0;
  parent[3] = 1;
  parent[4] = 1;
  parent[5] = SIZE_MAX;
  parent[6] = 5;
  parent[7] = 6;
  parent[8] = 6;
  parent[9] = SIZE_MAX;
  parent[10] = SIZE_MAX;
  parent[11] = 10;
  parent[12] = 3;
}

void printArray(size_t A[], size_t n) {
  for (size_t i = 0; i < n; ++i) {
    if (A[i] == SIZE_MAX) {
      printf("R ");
    } else {
      printf("%zu ", A[i]);
    }
  }
  printf("\n");
}

int main(void) {
  size_t A[13];

  initialize(A);
  puts("Initial state:");
  printArray(A, 13);

  Union(A, 9, 11);
  puts("After Union(9,11):");
  printArray(A, 13);

  initialize(A);
  Union(A, 3, 6);
  puts("After Union(3,6):");
  printArray(A, 13);

  initialize(A);
  Union(A, 3, 2);
  puts("After Union(3,2):");
  printArray(A, 13);

  return 0;
}
