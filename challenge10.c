#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// ============== Merge Sort =====================

void CopyArray(double A[], int iBegin, int iEnd, double B[]) {
  for (int k = iBegin; k < iEnd; ++k) {
    B[k] = A[k];
  }
}

void TopDownMerge(double A[], int iBegin, int iMiddle, int iEnd, double B[]) {
  int i = iBegin;
  int j = iMiddle;

  for (int k = iBegin; k < iEnd; ++k) {
    if (i < iMiddle && (j >= iEnd || A[i] <= A[j])) {
      B[k] = A[i];
      ++i;
    } else {
      B[k] = A[j];
      ++j;
    }
  }
}

void TopDownSplitMerge(double B[], int iBegin, int iEnd, double A[]) {
  if (iEnd - iBegin <= 1) {
    return;
  }
  int iMiddle = (iEnd + iBegin) / 2;
  TopDownSplitMerge(A, iBegin, iMiddle, B);
  TopDownSplitMerge(A, iMiddle, iEnd, B);
  TopDownMerge(B, iBegin, iMiddle, iEnd, A);
}

void TopDownMergeSort(double A[], double B[], int n) {
  CopyArray(A, 0, n, B);
  TopDownSplitMerge(B, 0, n, A);
}

// ============== Quick Sort ===========================

int partition(double A[], int lo, int hi) {
  double pivot = A[hi];

  int i = lo - 1;

  for (int j = lo; j < hi; ++j) {
    if (A[j] <= pivot) {
      ++i;
      double tmp = A[i];
      A[i] = A[j];
      A[j] = tmp;
    }
  }

  ++i;
  double tmp = A[i];
  A[i] = A[hi];
  A[hi] = tmp;
  return i;
}

void quicksort(double A[], int lo, int hi) {
  if (lo >= hi || lo < 0) {
    return;
  }

  int p = partition(A, lo, hi);

  quicksort(A, lo, p - 1);
  quicksort(A, p + 1, hi);
}

// =====================================================

void fill_with_random_numbers(double A[], size_t size) {
  double f = 1000.0 / RAND_MAX;
  for (size_t i = 0; i < size; ++i) {
    A[i] = rand() * f;
  }
}

enum { S1 = 5000000, S2 = S1*3 };

int main(void) {
  clock_t c1, c2;
  size_t t;

  printf("Merge sort:\n");

  static double A1[S1];
  static double B1[S1];
  fill_with_random_numbers(A1, S1);
  c1 = clock();
  TopDownMergeSort(A1, B1, S1);
  c2 = clock();
  t = (c2 - c1) * 1000 / CLOCKS_PER_SEC;
  printf("%d items -> %zu ms\n", S1, t);

  static double A2[S2];
  static double B2[S2];
  fill_with_random_numbers(A2, S2);
  c1 = clock();
  TopDownMergeSort(A2, B2, S2);
  c2 = clock();
  t = (c2 - c1) * 1000 / CLOCKS_PER_SEC;
  printf("%d items -> %zu ms\n", S2, t);

  printf("\nQuick sort:\n");

  static double C1[S1];
  fill_with_random_numbers(C1, S1);
  c1 = clock();
  quicksort(C1, 0, S1-1);
  c2 = clock();
  t = (c2 - c1) * 1000 / CLOCKS_PER_SEC;
  printf("%d items -> %zu ms\n", S1, t);

  static double C2[S2];
  fill_with_random_numbers(C2, S2);
  c1 = clock();
  quicksort(C2, 0, S2-1);
  c2 = clock();
  t = (c2 - c1) * 1000 / CLOCKS_PER_SEC;
  printf("%d items -> %zu ms\n", S2, t);
}
