#include "rationals.h"

rat rat_get(long long num, unsigned long long denom) {
  rat ret = {
    .sign = (num < 0),
    .num = (num < 0) ? -num : num,
    .denom = denom,
  };
  return ret;
}

rat rat_get_normal(rat x) {
  size_t c = gcd(x.num, x.denom);
  x.num /= c;
  x.denom /= c;
  return x;
}

rat rat_get_extended(rat x, size_t f) {
  x.num *= f;
  x.denom *= f;
  return x;
}

rat rat_get_prod(rat x, rat y) {
  rat ret = {
    .sign = (x.sign != y.sign),
    .num = x.num * y.num,
    .denom = x.denom * y.denom,
  };
  return rat_get_normal(ret);
}

rat rat_get_sum(rat x, rat y) {
  size_t c = gcd(x.denom, y.denom);
  size_t ax = y.denom/c;
  size_t bx = x.denom/c;
  x = rat_get_extended(x, ax);
  y = rat_get_extended(y, bx);
  assert(x.denom == y.denom);

  if (x.sign == y.sign) {
    x.num += y.num;
  } else if (x.num > y.num) {
    x.num -= y.num;
  } else {
    x.num = y.num - x.num;
    x.sign = !x.sign;
  }
  return rat_get_normal(x);
}

void rat_destroy(rat* rp) {
  if (rp) *rp = (rat){ 0 };
}

rat* rat_init(rat* rp,
              long long num,
              unsigned long long denom) {
  if (rp) *rp = rat_get(num, denom);
  return rp;
}

rat* rat_normalize(rat* rp) {
  if (rp) *rp = rat_get_normal(*rp);
  return rp;
}

rat* rat_extend(rat* rp, size_t f) {
  if (rp) *rp = rat_get_extended(*rp, f);
  return rp;
}

// Exs 14, p.152
char const* rat_print(size_t len, char tmp[len], rat const* x) {
  if (len > 0) {
    tmp[0] = x->sign ? '-' : '+';
    snprintf(&tmp[1], len-1, "%zu/%zu", x->num, x->denom);
  }
  return tmp;
}

int main(void) {
  char buf[16];
  rat r1 = rat_get(11, 7);
  printf("r1 = %s\n", rat_print(16, buf, &r1));
  rat r2 = rat_get(-59, 111);
  printf("r2 = %s\n", rat_print(16, buf, &r2));
  return EXIT_SUCCESS;
}
