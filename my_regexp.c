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
  if (*r == *s) {
    return match(r+1, s+1);
  }
  return false;
}

void test_match(void) {
  assert(match("aabb", "aabb"));
  assert(!match("aabb", "abbb"));
  assert(!match("aabb", "c"));
  assert(!match("aabb", "a"));
}

int main(void) {
  test_match();
}
