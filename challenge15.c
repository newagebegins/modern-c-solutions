// Challenge 15: Text processing in strings

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "my_regexp.h"

// Find a word w in the string s.
// Return a pointer to the start of w in s or 0 if not found.
char const* find_word(char const* s, char const* w) {
  if (!s || !w) return 0;
  if (!w[0]) return s;
  for (size_t i = 0; s[i]; ++i) {
    size_t j = i;
    size_t k = 0;
    while (s[j] == w[k]) {
      ++j;
      ++k;
      if (!w[k]) return s + j - k;
      if (!s[j]) return 0;
    }
  }
  return 0;
}

void test_find_word(void) {
  char const* s1 = "Hello,foo,12,foobar,7";
  char const* s2 = "Hello,foofoobar";
  char const* s3 = "3311122";

  char const* w1 = "foobar";
  char const* w2 = "112";

  char const* f1 = find_word(s1, w1);
  char const* f2 = find_word(s2, w1);
  char const* f3 = find_word(s3, w2);

  printf("f1: %s\n", f1);
  printf("f2: %s\n", f2);
  printf("f3: %s\n", f3);
}

// Replace first occurence of w by r in s, return new string.
char* replace_word(char const* s, char const* w, char const* r) {
  if (!r || !s || !w || !w[0]) return 0;
  char const* found = find_word(s, w);
  if (!found) return 0;
  size_t slen = strlen(s);
  size_t wlen = strlen(w);
  size_t rlen = strlen(r);
  size_t new_size = slen - wlen + rlen + 1; // + \0
  char* buf = malloc(new_size);
  if (!buf) return 0;
  size_t prefix = found - s;
  size_t suffix = slen - prefix - wlen + 1; // + \0
  memcpy(buf, s, prefix);
  memcpy(buf+prefix, r, rlen);
  memcpy(buf+prefix+rlen, s+prefix+wlen, suffix);
  return buf;
}

void test_replace_word(void) {
  char const* s = "Hello, World!";

  char* x1 = replace_word(s, "World", "Modern C");
  char* x2 = replace_word(s, "World", "C");
  char* x3 = replace_word(s, "World", "");

  printf("x1: %s\n", x1);
  printf("x2: %s\n", x2);
  printf("x3: %s\n", x3);

  free(x1);
  free(x2);
  free(x3);
}

void test_regexp() {
  assert(match("a[A-BC-De]b", "aAb"));
  assert(match("x[[:digit:];[:alpha:]]y", "x3y"));
  assert(!match("x[^[:digit:];[:alpha:]]y", "x3y"));

  char* s1 = regexp_replace("(*) ([[:digit:]])([a-z])", "Hello, 5q", "$0 there, $1-$2");
  assert(!strcmp(s1, "Hello, there, 5-q"));
  free(s1);

  char* s3 = regexp_replace("((abc) (([de])([[:digit:]])))", "abc e5", "$0|$1|$2|$3|$4");
  assert(!strcmp(s3, "abc e5|abc|e5|e|5"));
  free(s3);

  char* s4 = "abc1dd123x";
  saved_match search_res = regexp_search("[0-9][0-9][0-9]", s4);
  assert(search_res.start == s4 + 6);
  assert(search_res.end == s4 + 9);
}

int main(void) {
  test_find_word();
  test_replace_word();
  test_regexp();
  return EXIT_SUCCESS;
}
