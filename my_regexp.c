#include <stdbool.h>
#include <assert.h>

typedef struct match_res match_res;
struct match_res {
  bool matched;
  char const* r;
};

char const* after_bracket(char const* r) {
  int b = 1;
  while (*r) {
    switch (*r) {
    case '[':
      ++b;
      break;
    case ']':
      --b;
      if (!b) {
        return r+1;
      }
      break;
    }
    ++r;
  }
  assert(false);
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

match_res match_char_class(char const* r, char const* s) {
  bool matched = false;
  assert(*r == ':');
  ++r;
  switch (*r) {
  case 'a':
    ++r;
    assert(*r++ == 'l');
    assert(*r++ == 'p');
    assert(*r++ == 'h');
    assert(*r++ == 'a');
    matched = ('a' <= *s && *s <= 'z') || ('A' <= *s && *s <= 'Z');
    break;
  case 'd':
    ++r;
    assert(*r++ == 'i');
    assert(*r++ == 'g');
    assert(*r++ == 'i');
    assert(*r++ == 't');
    matched = ('0' <= *s && *s <= '9');
    break;
  default:
    assert(false);
  }
  assert(*r++ == ':');
  assert(*r++ == ']');
  return (match_res){
    .matched = matched,
    .r = r,
  };
}

match_res match_none_of(char const* r, char const* s) {
  while (*r) {
    switch (*r) {
      case ']':
        return (match_res){
          .matched = true,
          .r = r+1,
        };
      case '[': {
        match_res res = match_char_class(r+1, s);
        if (res.matched) {
          return (match_res){ .matched = false };
        }
        r = res.r;
        break;
      }
      default:
        if (*(r+1) == '-') {
          match_res res = match_range(r, s);
          if (res.matched) {
            return (match_res){ .matched = false };
          }
          r = res.r;
        } else if (*r == *s) {
          return (match_res){ .matched = false };
        } else {
          ++r;
        }
    }
  }
  assert(false);
}

match_res match_bracket(char const* r, char const* s) {
  while (*r && *s) { // todo: remove *s?
    switch (*r) {
    case '^':
      return match_none_of(r+1, s);
    case ']':
      return (match_res){ .matched = false };
    case '[': {
      match_res res = match_char_class(r+1, s);
      r = res.r;
      if (res.matched) {
        res.r = after_bracket(r);
        return res;
      }
      break;
    }
    default:
      if (*(r+1) == '-') {
        match_res res = match_range(r, s);
        r = res.r;
        if (res.matched) {
          res.r = after_bracket(r);
          return res;
        }
      } else if (*r == *s) {
        return (match_res){
          .matched = true,
          .r = after_bracket(r+1),
        };
      } else {
        ++r;
      }
    }
  }
  return (match_res){ .matched = false };
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
        match_res res = match_bracket(r+1, s);
        if (!res.matched) {
          return false;
        }
        r = res.r;
        ++s;
        break;
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
}

int main(void) {
  test_match();
}
