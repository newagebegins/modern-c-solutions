#include <stdbool.h>
#include <assert.h>

bool match(char const* r, char const* s);

char const* find_right_bracket(char const* r) {
  while (*r && *r != ']') ++r;
  return r;
}

bool match_bracket(char const* r, char const* s) {
  switch (*r) {
    case ']':
      return false;
    default:
      if (*r == *s) {
        r = find_right_bracket(r);
        assert(r);
        return match(r+1, s+1);
      }
      return match_bracket(r+1, s);
  }
}

bool match(char const* r, char const* s) {
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
    // Match any single character
    case '?': {
      return match(r+1, s+1);
    }
    case '[': {
      return match_bracket(r+1, s);
    }
    case ']': {
      assert(false);
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

  assert(match("a?c", "abc"));
  assert(match("a?c", "adc"));
  assert(!match("a?c", "ac"));
  assert(!match("a?c", "ab"));
  assert(!match("a?c", "bc"));

  assert(match("a[cm]at", "acat"));
  assert(match("[cm]at", "cat"));
  assert(match("a[cm]at", "amat"));
  assert(match("[cm]at", "mat"));
  assert(!match("a[cm]at", "ahat"));
  assert(!match("[cm]at", "hat"));
  assert(!match("[cm]at", "at"));
  assert(!match("[cm]at", "ca"));
}

int main(void) {
  test_match();
}
