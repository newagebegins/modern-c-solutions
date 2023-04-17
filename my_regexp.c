#include <stdbool.h>
#include <assert.h>

bool match(char const* r, char const* s);
bool match_bracket(char const* r, char const* s);

char const* find_right_bracket(char const* r) {
  while (*r && *r != ']') ++r;
  return r;
}

bool match_none_of(char const* r, char const* s) {
  switch (*r) {
    case ']':
      return match(r+1, s+1);
    default:
      if (*r == *s) {
        return false;
      }
      return match_none_of(r+1, s);
  }
}

bool match_range(char const** r, char const* s) {
  char c1 = **r;
  ++*r;
  assert(**r == '-');
  ++*r;
  char c2 = **r;
  ++*r;
  return (c1 <= *s && *s <= c2);
}

bool match_bracket(char const* r, char const* s) {
  switch (*r) {
    case '^':
      return match_none_of(r+1, s);
    case ']':
      return false;
    default:
      if (*(r+1) == '-') {
        if (match_range(&r, s)) {
          r = find_right_bracket(r);
          assert(*r == ']');
          return match(r+1, s+1);
        }
        return match_bracket(r, s);
      }
      if (*r == *s) {
        r = find_right_bracket(r+1);
        assert(*r == ']');
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
    case ']':
    case '^':
      assert(false);
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
}

int main(void) {
  test_match();
}
