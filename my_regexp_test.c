#include <assert.h>
#include <string.h>

#include "my_regexp.h"

void test_match(void) {
  // Literal characters

  assert(match("aabb", "aabb"));
  assert(!match("aabb", "abbb"));
  assert(!match("aabb", "c"));
  assert(!match("aabb", "a"));

  // Anything

  assert(match("a*b", "ab"));
  assert(match("a*b", "afoob"));
  assert(match("a*b", "aabb"));
  assert(!match("a*b", "afooc"));
  assert(!match("a*b", "ac"));
  assert(!match("a*b", "a"));

  // Any character

  assert(match("a?c", "abc"));
  assert(match("a?c", "adc"));
  assert(!match("a?c", "ac"));
  assert(!match("a?c", "ab"));
  assert(!match("a?c", "bc"));

  // Simple brackets

  assert(match("a[cm]at", "acat"));
  assert(match("[cm]at", "cat"));
  assert(match("a[cm]at", "amat"));
  assert(match("[cm]at", "mat"));
  assert(!match("a[cm]at", "ahat"));
  assert(!match("[cm]at", "hat"));
  assert(!match("[cm]at", "at"));
  assert(!match("[cm]at", "ca"));

  // None of

  assert(match("a[^cm]b", "axb"));
  assert(!match("a[^cm]b", "acb"));
  assert(!match("a[^cm]b", "amb"));
  assert(!match("a[^cm]b", "axxb"));

  // Ranges

  assert(match("a[A-C]b", "aAb"));
  assert(match("a[A-C]b", "aBb"));
  assert(match("a[A-C]b", "aCb"));
  assert(!match("a[A-C]b", "aDb"));
  assert(!match("a[A-C]b", "aABb"));

  assert(match("a[A-CD]b", "aAb"));
  assert(match("a[A-CD]b", "aBb"));
  assert(match("a[A-CD]b", "aCb"));
  assert(match("a[A-CD]b", "aDb"));
  assert(!match("a[A-CD]b", "aEb"));

  assert(match("a[A-BC-De]b", "aAb"));
  assert(match("a[A-BC-De]b", "aBb"));
  assert(match("a[A-BC-De]b", "aCb"));
  assert(match("a[A-BC-De]b", "aDb"));
  assert(match("a[A-BC-De]b", "aeb"));
  assert(!match("a[A-BC-De]b", "axb"));
  assert(!match("a[A-BC-De]b", "aABb"));

  assert(match("a[A-Z][0-9]b", "aA0b"));
  assert(match("a[A-Z][0-9]b", "aZ9b"));
  assert(match("a[A-Z][0-9]b", "aF7b"));
  assert(!match("a[A-Z][0-9]b", "a77b"));
  assert(!match("a[A-Z][0-9]b", "aABb"));

  // None of with ranges

  assert(!match("a[^A-C]b", "aAb"));
  assert(!match("a[^A-C]b", "aBb"));
  assert(!match("a[^A-C]b", "aCb"));
  assert(match("a[^A-C]b", "aDb"));

  assert(match("a[A-Z][^0-9b]b", "aACb"));
  assert(!match("a[A-Z][^0-9b]b", "aA0b"));
  assert(!match("a[A-Z][^b0-9]b", "aAbb"));

  // Character classes

  assert(match("[[:digit:]]", "0"));
  assert(match("[[:digit:]]", "9"));
  assert(!match("[[:digit:]]", "a"));
  assert(match("a[[:digit:]]b", "a9b"));
  assert(match("a[[:digit:]x]b", "a9b"));
  assert(match("a[[:digit:]x]b", "axb"));
  assert(!match("a[[:digit:]x]b", "abb"));

  assert(match("[[:alpha:]]", "A"));
  assert(match("[[:alpha:]]", "Z"));
  assert(match("[[:alpha:]]", "a"));
  assert(match("[[:alpha:]]", "z"));
  assert(!match("[[:alpha:]]", "0"));
  assert(match("[[:alpha:][:digit:]]", "A"));
  assert(match("[[:alpha:][:digit:]]", "5"));
  assert(!match("[[:alpha:][:digit:]]", "@"));

  assert(match("x[[:digit:];[:alpha:]]y", "x3y"));
  assert(match("x[[:digit:];[:alpha:]]y", "x;y"));
  assert(match("x[[:digit:];[:alpha:]]y", "xGy"));
  assert(match("x[[:digit:];[:alpha:]]y", "xgy"));
  assert(!match("x[[:digit:];[:alpha:]]y", "x%y"));

  // Character classes (none of)
  assert(match("[^[:digit:]]", "a"));
  assert(!match("[^[:digit:]]", "5"));
  assert(match("x[^[:digit:];[:alpha:]]y", "x@y"));
  assert(!match("x[^[:digit:];[:alpha:]]y", "x3y"));
  assert(!match("x[^[:digit:];[:alpha:]]y", "xSy"));
  assert(!match("x[^[:digit:];[:alpha:]]y", "x;y"));

  // Grouping
  char const* s = "Hello, cat";
  assert(match("*([cm]at)", s));
  assert(saves[0].start == s + 7);
  assert(saves[0].end == s + 10);

  clear_saves();

  char const* s2 = "Hello, 5q";
  assert(match("(*) ([[:digit:]])([a-z])", s2));
  assert(saves[0].start == s2);
  assert(saves[0].end == s2+6);
  assert(saves[1].start == s2+7);
  assert(saves[1].end == s2+8);
  assert(saves[2].start == s2+8);
  assert(saves[2].end == s2+9);

  // Regexp replace

  char* s1 = regexp_replace("(*) ([[:digit:]])([a-z])", "Hello, 5q", "$0 there, $1-$2");
  assert(!strcmp(s1, "Hello, there, 5-q"));
  free(s1);

  char* s3 = regexp_replace("((abc) (([de])([[:digit:]])))", "abc e5", "$0|$1|$2|$3|$4");
  assert(!strcmp(s3, "abc e5|abc|e5|e|5"));
  free(s3);

  // Regexp search

  char* s4 = "abc1dd123x";
  saved_match search_res = regexp_search("[0-9][0-9][0-9]", s4);
  assert(search_res.start == s4 + 6);
  assert(search_res.end == s4 + 9);
}

int main(void) {
  test_match();
}
