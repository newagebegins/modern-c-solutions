#include <stdbool.h>
#include <assert.h>

bool match(char const* r, char const* s) {
  assert(r);
  assert(s);
  if (!*r) {
    return true;
  }
  if (!*s) {
    return false;
  }
  switch (*r) {
    // Match anything
    case '*': {
      if (match(r+1, s)) {
        return true;
      }
      return match(r, s+1);
    }
    // Match a literal character
    default: {
      if (*r == *s) {
        return match(r+1, s+1);
      }
    }
  }
  return false;
}

void test_match(void) {
  assert(match("aabb", "aabb"));
  assert(!match("aabb", "abbb"));
  assert(!match("aabb", "c"));
  assert(!match("aabb", "a"));

  assert(match("a*b", "ab"));
  assert(match("a*b", "afoob"));
  assert(match("a*b", "aabb"));
  assert(!match("a*b", "afooc"));
  assert(!match("a*b", "ac"));
  assert(!match("a*b", "a"));
}

int main(void) {
  test_match();
}
