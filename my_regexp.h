#ifndef MY_REGEXP
#define MY_REGEXP

#include <stdlib.h>
#include <stdbool.h>

typedef struct saved_match saved_match;
struct saved_match {
  size_t index;
  char const* start;
  char const* end;
};

bool match(char const* r, char const* s);
char* regexp_replace(char const* r, char const* s, char const* repl);
saved_match regexp_search(char const* r, char const* s);

#endif
