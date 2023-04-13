#include <stdlib.h>
#include <string.h>

#include "circular.h"

struct circular {
  size_t start;
  size_t len;
  size_t max_len;
  double* tab;
};

circular* circular_init(circular* c, size_t max_len) {
  if (c) {
    if (max_len) {
      *c = (circular){
        .max_len = max_len,
        .tab = malloc(sizeof(double[max_len])),
      };
      // Allocation failed.
      if (!c->tab) c->max_len = 0;
    } else {
      *c = (circular){ 0 };
    }
  }
  return c;
}

void circular_destroy(circular* c) {
  if (c) {
    free(c->tab);
    circular_init(c, 0);
  }
}

static size_t circular_getpos(circular* c, size_t pos) {
  pos += c->start;
  pos %= c->max_len;
  return pos;
}

double* circular_element(circular* c, size_t pos) {
  double* ret = 0;
  if (c) {
    if (pos < c->max_len) {
      pos = circular_getpos(c, pos);
      ret = &c->tab[pos];
    }
  }
  return ret;
}

circular* circular_append(circular* c, double value) {
  if (c) {
    if (c->len >= c->max_len)
      return 0;
    size_t pos = circular_getpos(c, c->len);
    c->tab[pos] = value;
    c->len += 1;
  }
  return c;
}

double circular_pop(circular* c) {
  double ret = 0;
  if (c && c->len > 0) {
    ret = c->tab[c->start];
    c->len -= 1;
    c->start = (c->start + 1) % c->max_len;
  }
  return ret;
}

circular* circular_new(size_t len) {
  circular* c = malloc(sizeof *c);
  return circular_init(c, len);
}

void circular_delete(circular* c) {
  if (c) {
    circular_destroy(c);
    free(c);
  }
}

size_t circular_getlength(circular* c) {
  return c ? c->len : 0;
}

circular* circular_resize(circular* c, size_t nlen) {
  if (c) {
    size_t len = c->len;
    if (len > nlen) return 0;
    size_t olen = c->max_len;
    if (nlen != olen) {
      size_t ostart = circular_getpos(c, 0);
      size_t nstart = ostart;
      double* otab = c->tab;
      double* ntab;
      if (nlen > olen) {
        ntab = realloc(c->tab, sizeof(double[nlen]));
        if (!ntab) return 0;
        // Two separate chunks
        if (ostart+len > olen) {
          size_t ulen = olen - ostart;
          size_t llen = len - ulen;
          if (llen <= (nlen - olen)) {
            /* Copy the lower one up after the old end. */
            memcpy(ntab + olen, ntab,
                   llen*sizeof(double));
          } else {
            /* Move the upper one up to the new end. */
            nstart = nlen - ulen;
            memmove(ntab + nstart, ntab + ostart,
                    ulen*sizeof(double));
          }
        }
      } else {
        if (ostart+len > olen) {
          // Two separate chunks
          // Move the upper chunk to the new end
          size_t ulen = olen - ostart;
          nstart = nlen - ulen;
          memmove(otab + nstart, otab + ostart,
                  ulen*sizeof(double));
        } else if (ostart+len > nlen) {
          // Contiguous chunk, end doesn't fit
          // Move elements to the beginning
          nstart = 0;
          memmove(otab, otab + ostart, len*sizeof(double));
        }
        ntab = realloc(c->tab, sizeof(double[nlen]));
        if (!ntab) return 0;
      }
      *c = (circular){
        .max_len = nlen,
        .start = nstart,
        .len = len,
        .tab = ntab,
      };
    }
  }
  return c;
}
