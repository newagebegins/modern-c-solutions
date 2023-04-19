#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

typedef struct match_res match_res;
struct match_res {
  bool matched;
  char const* r;
};

typedef struct saved_match saved_match;
struct saved_match {
  size_t index;
  char const* start;
  char const* end;
};

enum {max_saves = 20};
saved_match saves[max_saves] = {0};
saved_match group_stack[max_saves] = {0};
saved_match* group_sp = group_stack;
size_t group_index = 0;

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
  while (*r) {
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
        saved_match* old_group_sp = group_sp;
        size_t old_group_index = group_index;
        if (match(r+1, s)) {
          return true;
        }
        group_index = old_group_index;
        group_sp = old_group_sp;
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
      case '(':
        group_sp->index = group_index++;
        group_sp->start = s;
        ++group_sp;
        ++r;
        break;
      case ')':
        --group_sp;
        group_sp->end = s;
        saves[group_sp->index] = *group_sp;
        ++r;
        break;
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
  while (*r == ')') {
    // todo: remove duplicate code
    --group_sp;
    group_sp->end = s;
    saves[group_sp->index] = *group_sp;
    ++r;
  }
  if (!*r) {
    return true;
  }
  return false;
}

static void clear_saves() {
  for (size_t i = 0; i < max_saves; ++i) {
    saves[i] = (saved_match){0};
    group_stack[i] = (saved_match){0};
  }
  group_sp = group_stack;
  group_index = 0;
}

char* regexp_replace(char const* r, char const* s, char const* repl) {
  clear_saves();
  char buf[200] = {0};
  char* b = buf;
  if (match(r, s)) {
    while (*repl) {
      if (*repl == '$') {
        ++repl;
        int g = atoi(repl);
        ++repl;
        for (char const* p = saves[g].start; p < saves[g].end; ++p, ++b) {
          *b = *p;
        }
      } else {
        *b = *repl;
        ++b;
        ++repl;
      }
    }
    *b = 0;
  } else {
    strcpy(buf, s);
  }
  char* ret = malloc(strlen(buf) + 1);
  strcpy(ret, buf);
  return ret;
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
}

int main(void) {
  test_match();
}
