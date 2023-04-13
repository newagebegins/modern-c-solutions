#include <stdio.h>

int main(void) {
  char const A[] = { 'e', 'n', 'd', '\0', };
  char const B[] = { 'e', 'n', 'd', '\0', };
  char const* c = "end";
  char const* d = "end";
  char const* e = "friend";
  char const* f = (char const[]){ 'e', 'n', 'd', '\0', };
  char const* g = (char const[]){ 'e', 'n', 'd', '\0', };

  printf("A = %p\n", A);
  printf("B = %p\n", B);
  printf("c = %p\n", c);
  printf("d = %p\n", d);
  printf("e = %p\n", e);
  printf("f = %p\n", f);
  printf("g = %p\n", g);
}
