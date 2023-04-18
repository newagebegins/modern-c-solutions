#include <stdbool.h>
#include <assert.h>

typedef struct match_res match_res;
struct match_res {
  bool matched;
  char const* r;
};

bool match(char const* r, char const* s);
bool match_bracket(char const* r, char const* s);
match_res match_range(char const* r, char const* s);

char const* find_right_bracket(char const* r) {
  while (*r && *r != ']') ++r;
  return r;
}

bool match_none_of(char const* r, char const* s) {
  switch (*r) {
    case ']':
      return match(r+1, s+1);
    default:
      if (*(r+1) == '-') {
        match_res res = match_range(r, s);
        if (res.matched) {
          return false;
        }
        return match_none_of(res.r, s);
      }
      if (*r == *s) {
        return false;
      }
      return match_none_of(r+1, s);
  }
}

match_res match_range(char const* r, char const* s) {
  char c1 = *r;
  ++r;
  assert(*r == '-');
  ++r;
  char c2 = *r;
  ++r;
  return (match_res){
    .matched = (c1 <= *s && *s <= c2),
    .r = r,
  };
}

bool match_bracket(char const* r, char const* s) {
  while (*r && *s) {
    switch (*r) {
    case '^':
      return match_none_of(r+1, s);
    case ']':
      return false;
    default:
      if (*(r+1) == '-') {
        match_res res = match_range(r, s);
        r = res.r;
        if (res.matched) {
          r = find_right_bracket(r);
          assert(*r == ']');
          return match(r+1, s+1);
        }
      } else if (*r == *s) {
        r = find_right_bracket(r+1);
        assert(*r == ']');
        return match(r+1, s+1);
      } else {
        ++r;
      }
    }
  }
  return false;
}

bool match(char const* r, char const* s) {
  while (*r && *s) {
    switch (*r) {
      // Match anything
      case '*': {
        if (match(r+1, s)) {
          return true;
        }
        ++s;
        break;
      }
      // Match any single character
      case '?': {
        ++r;
        ++s;
        break;
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
          ++r;
          ++s;
        } else {
          return false;
        }
      }
    }
  }
  if (!*r) {
    return true;
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

  // None of with ranges

  assert(!match("a[^A-C]b", "aAb"));
  assert(!match("a[^A-C]b", "aBb"));
  assert(!match("a[^A-C]b", "aCb"));
  assert(match("a[^A-C]b", "aDb"));

  assert(match("a[A-Z][^0-9b]b", "aACb"));
  assert(!match("a[A-Z][^0-9b]b", "aA0b"));
  assert(!match("a[A-Z][^b0-9]b", "aAbb"));
}

int main(void) {
  test_match();
}
