#include <assert.h>
#include <string.h>

#include "my_regexp.h"

typedef struct match_res match_res;
struct match_res {
  bool matched;
  char const* r;
};

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

void clear_saves() {
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

saved_match regexp_search(char const* r, char const* s) {
  char buf[100] = {'*', '('};
  char* p = stpcpy(buf+2, r);
  *p = ')';
  ++p;
  *p = '*';
  ++p;
  *p = 0;
  clear_saves();
  if (match(buf, s)) {
    return saves[0];
  }
  return (saved_match){0};
}
