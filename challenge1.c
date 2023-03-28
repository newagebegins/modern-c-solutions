#include <stdlib.h>
#include <stdio.h>

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

void printArray(double A[], size_t n) {
  printf("[");
  if (n == 0) {
    return;
  }
  printf("%.2f", A[0]);
  for (size_t i = 1; i < n; ++i) {
    printf(", %.2f", A[i]);
  }
  printf("]\n");
}

int main(void) {
  double A[8] = {8.0, 3.3, 5.2, 3.3, 9.0, 22.1, 7.11, 0.3};
  double B[8];

  printf("Merge sort:\n");
  printArray(A, 8);
  TopDownMergeSort(A, B, 8);
  printArray(A, 8);

  double C[8] = {8.0, 3.3, 5.2, 3.3, 9.0, 22.1, 7.11, 0.3};
  printf("\nQuick sort:\n");
  printArray(C, 8);
  quicksort(C, 0, 7);
  printArray(C, 8);
}
