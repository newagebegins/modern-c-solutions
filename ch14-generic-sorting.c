#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef int compare_function(void const*, void const*);

int compare_double(void const* A, void const* B) {
  double const* a = A;
  double const* b = B;
  if (*a < *b) return -1;
  if (*a > *b) return 1;
  return 0;
}

// ============== Merge Sort =====================

void TopDownMerge(void* A, int iBegin, int iMiddle, int iEnd, void* B, size_t size, compare_function compar) {
  int i = iBegin;
  int j = iMiddle;

  for (int k = iBegin; k < iEnd; ++k) {
    if (i < iMiddle && (j >= iEnd || compar(A+i*size, A+j*size) <= 0)) {
      memcpy(B+k*size, A+i*size, size);
      ++i;
    } else {
      memcpy(B+k*size, A+j*size, size);
      ++j;
    }
  }
}

void TopDownSplitMerge(void* B, int iBegin, int iEnd, void* A, size_t size, compare_function compar) {
  if (iEnd - iBegin <= 1) {
    return;
  }
  int iMiddle = (iEnd + iBegin) / 2;
  TopDownSplitMerge(A, iBegin, iMiddle, B, size, compar);
  TopDownSplitMerge(A, iMiddle, iEnd, B, size, compar);
  TopDownMerge(B, iBegin, iMiddle, iEnd, A, size, compar);
}

void TopDownMergeSort(void* A, void* B, size_t n, size_t size, compare_function compar) {
  memcpy(B, A, n*size);
  TopDownSplitMerge(B, 0, n, A, size, compar);
}

// ============== Quick Sort ===========================

int partition(void* A, int lo, int hi, size_t size, compare_function compar) {
  void* pivot = A + hi*size;

  int i = lo - 1;

  for (int j = lo; j < hi; ++j) {
    if (compar(A+j*size, pivot) <= 0) {
      ++i;
      unsigned char tmp[size];
      memcpy(tmp, A+i*size, size);
      memcpy(A+i*size, A+j*size, size);
      memcpy(A+j*size, tmp, size);
    }
  }

  ++i;
  unsigned char tmp[size];
  memcpy(tmp, A+i*size, size);
  memcpy(A+i*size, A+hi*size, size);
  memcpy(A+hi*size, tmp, size);
  return i;
}

void quicksort(void* A, int lo, int hi, size_t size, compare_function compar) {
  if (lo >= hi || lo < 0) {
    return;
  }

  int p = partition(A, lo, hi, size, compar);

  quicksort(A, lo, p - 1, size, compar);
  quicksort(A, p + 1, hi, size, compar);
}

// =====================================================

bool isSorted(double A[], size_t n) {
  for (size_t i = 0; i < n-1; ++i) {
    if (A[i] > A[i+1]) {
      return false;
    }
  }
  return true;
}

void printArray(double A[], size_t n) {
  printf("[");
  if (n == 0) {
    return;
  }
  printf("%.2f", A[0]);
  for (size_t i = 1; i < n; ++i) {
    printf(", %.2f", A[i]);
  }
  printf("] - ");
  if (isSorted(A, n)) {
    printf("sorted\n");
  } else {
    printf("not sorted\n");
  }
}

int main(void) {
  double A[8] = {8.0, 3.3, 5.2, 3.3, 9.0, 22.1, 7.11, 0.3};
  double B[8];

  printf("Merge sort:\n");
  printArray(A, 8);
  TopDownMergeSort(A, B, 8, sizeof A[0], compare_double);
  printArray(A, 8);

  double C[8] = {8.0, 3.3, 5.2, 3.3, 9.0, 22.1, 7.11, 0.3};
  printf("\nQuick sort:\n");
  printArray(C, 8);
  quicksort(C, 0, 7, sizeof C[0], compare_double);
  printArray(C, 8);
}
