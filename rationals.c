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

rat* rat_sumup(rat* rp, rat y) {
  size_t c = gcd(rp->denom, y.denom);
  size_t ax = y.denom/c;
  size_t bx = rp->denom/c;
  rat_extend(rp, ax);              /*@\label{rat_extend_x}*/
  y = rat_get_extended(y, bx);
  assert(rp->denom == y.denom);

  if (rp->sign == y.sign) {
    rp->num += y.num;
  } else if (rp->num > y.num) {
    rp->num -= y.num;
  } else {
    rp->num = y.num - rp->num;
    rp->sign = !rp->sign;
  }
  return rat_normalize(rp);       /*@\label{rat_normalize_x}*/
}

rat* rat_rma(rat* rp, rat x, rat y) {
  return rat_sumup(rp, rat_get_prod(x, y));
}

// Exs 14, p.152
char const* rat_print(size_t len, char tmp[len], rat const* x) {
  if (len > 0) {
    tmp[0] = x->sign ? '-' : '+';
    snprintf(&tmp[1], len-1, "%zu/%zu", x->num, x->denom);
  }
  return tmp;
}

// Exs 15, p.152
char const* rat_normalize_print(size_t len, char tmp[len], rat const* x) {
  rat_normalize(x);
  return rat_print(len, tmp, x);
}

// Exs 16
rat* rat_dotproduct(rat rp[static 1], size_t n, rat const A[n], rat const B[n]) {
  rat_init(rp, 0, 1);
  for (size_t i = 0; i < n; ++i) {
    rat_rma(rp, A[i], B[i]);
  }
  return rp;
}

int main(void) {
  char buf[16];
  rat r1 = rat_get(11, 7);
  printf("r1 = %s\n", rat_print(16, buf, &r1));
  rat r2 = rat_get(-59, 111);
  printf("r2 = %s\n", rat_print(16, buf, &r2));
  rat r3 = rat_get(-25, 100);
  printf("r3 = %s\n", rat_normalize_print(16, buf, &r3));

  rat A[3];
  A[0] = rat_get(1, 3);
  A[1] = rat_get(-4, 8);
  A[2] = rat_get(25, 3);

  rat B[3];
  B[0] = rat_get(5, 7);
  B[1] = rat_get(5, 77);
  B[2] = rat_get(-64, 8);

  rat dp = {0};
  rat_dotproduct(&dp, 3, A, B);
  assert(dp.sign);
  assert(dp.num == 10235);
  assert(dp.denom == 154);
  printf("dp = %s\n", rat_print(16, buf, &dp));

  return EXIT_SUCCESS;
}
