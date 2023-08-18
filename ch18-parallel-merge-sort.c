#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define Pthread_create(threadp, attrp, start_routinep, argp) assert(pthread_create(threadp, attrp, start_routinep, argp) == 0)
#define Pthread_join(thread, value_ptr) assert(pthread_join(thread, value_ptr) == 0)

#define Gettimeofday(a,b) assert(gettimeofday(a,b) == 0)

void TopDownMerge(int A[], int iBegin, int iMiddle, int iEnd, int B[]) {
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

void TopDownSplitMerge(int B[], int iBegin, int iEnd, int A[]) {
  if (iEnd - iBegin <= 1) {
    return;
  }
  int iMiddle = (iEnd + iBegin) / 2;
  TopDownSplitMerge(A, iBegin, iMiddle, B);
  TopDownSplitMerge(A, iMiddle, iEnd, B);
  TopDownMerge(B, iBegin, iMiddle, iEnd, A);
}

int arrayEqual(size_t N, int A[N], int B[N]) {
  for (size_t i = 0; i < N; i++) {
    if (A[i] != B[i]) {
      return 0;
    }
  }
  return 1;
}

static int cmpint(const void *p1, const void *p2) {
  int x1 = *((int*)p1);
  int x2 = *((int*)p2);
  if (x1 < x2) return -1;
  if (x1 > x2) return 1;
  return 0;
}

void fillArray(int A[], size_t n) {
  for (size_t i = 0; i < n; ++i) {
    A[i] = rand() % 10000;
  }
}

typedef struct Task Task;
struct Task {
  int* A;
  int* B;
  int iBegin;
  int iEnd;
};

void* sortTask(void* arg) {
  Task* task = arg;
  TopDownSplitMerge(task->A, task->iBegin, task->iEnd, task->B);
  return 0;
}

void parallelMergeSort(size_t N, int A[N], size_t k) {
  // Working copy of A
  int* B = malloc(N * sizeof(int));
  memcpy(B, A, N * sizeof(int));

  if (k == 0) {
    TopDownSplitMerge(B, 0, N, A);
  } else {
    int num_threads = 1;
    for (int i = 0; i < k; ++i) {
      num_threads *= 2;
    }
    int chunk_size = N/num_threads;

    for (int j = k; j > 0; j--, num_threads /= 2, chunk_size *= 2) {
      pthread_t threads[num_threads];
      Task tasks[num_threads];

      for (size_t i = 0; i < num_threads; ++i) {
        tasks[i] = (Task){
          .A = j % 2 ? A : B,
          .B = j % 2 ? B : A,
          .iBegin = i*chunk_size,
          .iEnd = (i+1)*chunk_size,
        };
      }
      for (size_t i = 0; i < num_threads; ++i) {
        Pthread_create(&threads[i], 0, sortTask, (void *) &tasks[i]);
      }
      for (size_t i = 0; i < num_threads; ++i) {
        Pthread_join(threads[i], 0);
      }
    }
    TopDownMerge(B, 0, N/2, N, A);
  }

  free(B);
}

int main(int argc, char* argv[argc+1]) {
  assert(argc == 2);

  // We create 2^k threads for sorting.
  int k = atoi(argv[1]);
  assert(k >= 0 && k <= 4);

  // Number of elements to sort.
  // For correct work N should be a power of 2.
  int const N = 1048576;

  int* A = malloc(N * sizeof(int));
  fillArray(A, N);

  int* Acopy = malloc(N * sizeof(int));
  memcpy(Acopy, A, N * sizeof(int));

  struct timeval start;
  Gettimeofday(&start, 0);

  parallelMergeSort(N, A, k);

  struct timeval end;
  Gettimeofday(&end, 0);

  double secs = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1E-6;
  printf("time = %g secs\n", secs);

  // Check that parallelMergeSort works as expected.
  qsort(Acopy, N, sizeof(int), cmpint);
  assert(arrayEqual(N, A, Acopy));
}
