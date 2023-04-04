#include <time.h>
#include <stdio.h>
#include <stdlib.h>

double timespec_diff(struct timespec t1, struct timespec t2) {
  return difftime(t1.tv_sec, t2.tv_sec)*1E9 + t1.tv_nsec - t2.tv_nsec;
}

int main(void) {
  struct tm tp1 = {
    .tm_sec = 7,
    .tm_mday = 1,
  };
  time_t t1 = mktime(&tp1);
  struct timespec ts1 = {
    .tv_sec = t1,
    .tv_nsec = 123,
  };

  struct tm tp2 = {
    .tm_sec = 5,
    .tm_mday = 1,
  };
  time_t t2 = mktime(&tp2);
  struct timespec ts2 = {
    .tv_sec = t2,
    .tv_nsec = 200,
  };

  double diff = timespec_diff(ts1, ts2);
  printf("%g\n", diff);
  return EXIT_SUCCESS;
}
