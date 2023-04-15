// Challenge 15: Text processing in strings

#include <stdlib.h>
#include <stdio.h>

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

int main(void) {
  test_find_word();
  return EXIT_SUCCESS;
}
