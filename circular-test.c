#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "circular.h"

void circular_print(circular* c) {
  size_t len = circular_getlength(c);
  for (size_t i = 0; i < len; ++i) {
    double* el = circular_element(c, i);
    printf("%.1f ", *el);
  }
  printf("\n");
}

int main(void) {
  circular* c = circular_new(7);
  circular_append(c, 1.1);
  circular_append(c, 2.2);
  circular_append(c, 3.3);
  circular_append(c, 4.4);
  circular_append(c, 5.5);
  circular_append(c, 6.6);
  circular_append(c, 7.7);
  circular_print(c);
  assert(circular_pop(c) == 1.1);
  assert(circular_pop(c) == 2.2);
  assert(circular_pop(c) == 3.3);
  assert(circular_pop(c) == 4.4);
  assert(circular_pop(c) == 5.5);
  circular_append(c, 8.8);
  circular_append(c, 9.9);
  circular_print(c);
  circular_resize(c, 10);
  circular_print(c);
  circular_append(c, 3.5);
  circular_append(c, 7.2);
  circular_append(c, 5.7);
  circular_append(c, 6.8);
  circular_print(c);
  circular_resize(c, 12);
  circular_print(c);
  circular_resize(c, 9);
  circular_print(c);
  assert(circular_pop(c) == 6.6);
  assert(circular_pop(c) == 7.7);
  assert(circular_pop(c) == 8.8);
  assert(circular_pop(c) == 9.9);
  assert(circular_pop(c) == 3.5);
  assert(circular_pop(c) == 7.2);
  circular_print(c);
  circular_resize(c, 3);
  circular_print(c);
  circular_resize(c, 2);
  circular_print(c);
  circular_resize(c, 5);
  circular_print(c);
  return EXIT_SUCCESS;
}
